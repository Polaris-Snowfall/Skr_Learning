// Simple command-line kernel monitor useful for
// controlling the kernel and exploring the system interactively.

#include <inc/stdio.h>
#include <inc/string.h>
#include <inc/memlayout.h>
#include <inc/assert.h>
#include <inc/x86.h>

#include <kern/console.h>
#include <kern/monitor.h>
#include <kern/kdebug.h>
#include <kern/trap.h>

#define CMDBUF_SIZE	80	// enough for one VGA text line


struct Command {
	const char *name;
	const char *desc;
	// return -1 to force monitor to exit
	int (*func)(int argc, char** argv, struct Trapframe* tf);
};

static struct Command commands[] = {
	{ "help", "Display this list of commands", mon_help },
	{ "kerninfo", "Display information about the kernel", mon_kerninfo },
	{ "backtrace", "Prints a backtrace of the stack", mon_backtrace },
	{"showmappings","Display in a useful and easy-to-read format all of the physical page mappings (or lack thereof) that apply to a particular range of virtual/linear addresses in the currently active address space.",mon_showmappings},
	{"ni","execute single-step one instruction",mon_nextstep},
	{"nextstep","execute single-step one instruction",mon_nextstep},
	{"continue","continue execution",mon_continue},

};

/***** Implementations of basic kernel monitor commands *****/

int
mon_help(int argc, char **argv, struct Trapframe *tf)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(commands); i++)
		cprintf("%s - %s\n", commands[i].name, commands[i].desc);
	return 0;
}

int
mon_kerninfo(int argc, char **argv, struct Trapframe *tf)
{
	extern char _start[], entry[], etext[], edata[], end[];

	cprintf("Special kernel symbols:\n");
	cprintf("  _start                  %08x (phys)\n", _start);
	cprintf("  entry  %08x (virt)  %08x (phys)\n", entry, entry - KERNBASE);
	cprintf("  etext  %08x (virt)  %08x (phys)\n", etext, etext - KERNBASE);
	cprintf("  edata  %08x (virt)  %08x (phys)\n", edata, edata - KERNBASE);
	cprintf("  end    %08x (virt)  %08x (phys)\n", end, end - KERNBASE);
	cprintf("Kernel executable memory footprint: %dKB\n",
		ROUNDUP(end - entry, 1024) / 1024);
	return 0;
}

int
mon_backtrace(int argc, char **argv, struct Trapframe *tf)
{
	// Your code here.
	cprintf("Stack backtrace:\n");
	uint32_t ebp = read_ebp();
	uint32_t eip = *(uint32_t*)(ebp+4);
	struct Eipdebuginfo dinfo;
	while(1)
	{
		debuginfo_eip(eip,&dinfo);
		cprintf("  ebp %08x  eip %08x  args %08x %08x %08x %08x %08x\n",ebp,eip,*(uint32_t*)(ebp+8),*(uint32_t*)(ebp+12),*(uint32_t*)(ebp+16),*(uint32_t*)(ebp+20),*(uint32_t*)(ebp+24));
		cprintf("         %s:%d: %.*s+%d\n",dinfo.eip_file,dinfo.eip_line,dinfo.eip_fn_namelen,dinfo.eip_fn_name,eip-dinfo.eip_fn_addr);
		ebp = *(uint32_t*)(ebp);
		if(ebp!=0)
			eip = *(uint32_t*)(ebp+4);
		else
			break;
	}
	return 0;
}

int
mon_showmappings(int argc, char **argv, struct Trapframe *tf)
{
	// Your code here.
	cprintf("VPN range		Entry		Flags		Physical page\n");
	uintptr_t start_va,end_va;
	pte_t* pte_store;
	uint32_t pdx = 0;
	uint32_t flags_pde,flags_pte = 0;
	uint32_t start_vpn,end_vpn = 0;
	uint32_t start_phy,end_phy = 0;
	char flags_buf[11];
	const char flags_str[11] = "-GSDACTUWP";
	flags_buf[0] = '-';
	flags_buf[10] = '\0';
	if(argc!=3&&argc!=1&&argc!=2)
	{
		cprintf("usage: showmappings [start_va] [end_va]\n");
		return 0;
	}

	if(argc==3)
	{
		start_va = ROUNDDOWN(strtol(argv[1],NULL,16),PGSIZE);
		end_va = ROUNDDOWN(strtol(argv[2],NULL,16),PGSIZE);
	}
	else if(argc==2)
	{
		start_va = ROUNDDOWN(strtol(argv[1],NULL,16),PGSIZE);
		end_va = start_va+0x1000;
	}
	else
	{
		start_va = 0;
		end_va = 0xfffff000;
	}
	if(end_va<start_va)
	{
		cprintf("end_va must >= start_va\n");
	}
	for(;start_va<end_va;start_va+=PGSIZE)
	{
		page_lookup(kern_pgdir,(void*)start_va,&pte_store);
		//前者意味着对应页表不存在,后者意味着页表项无对应物理页
		if(pte_store==NULL||*pte_store==0)
			continue;
		else
		{

			if(pdx!=PDX(start_va)||pdx==0)
			{
				pdx = PDX(start_va);
				flags_pde = (kern_pgdir[PDX(start_va)])&0x1ff;
				for(int i = 1;i<10;++i)
				{
					flags_buf[i] = ((flags_pde&0x100)==0)?'-':flags_str[i];
					flags_pde <<=1;
				}
				cprintf("[%05x-%05x]		PDE[%03x]		%s\n",(pdx<<PTSHIFT>>PGSHIFT),(pdx<<PTSHIFT>>PGSHIFT)+0x3ff,pdx,flags_buf);
				//刷新flags_pte
				flags_pte = 0;
			}
			//页面权限改变或物理页不连续,打印一次
			if(flags_pte!=0&&(flags_pte!=((*pte_store)&0x1ff)||(end_phy+1!=(*pte_store>>PGSHIFT)&&end_phy-1!=(*pte_store>>PGSHIFT))))
			{
				for(int i = 1;i<10;++i)
				{
					flags_buf[i] = ((flags_pte&0x100)==0)?'-':flags_str[i];
					flags_pte <<=1;
				}
				cprintf("  [%05x-%05x]		PTE[%03x-%03x]	  %s	%05x-%05x\n",start_vpn,end_vpn,start_vpn&0x3FF,end_vpn&0x3FF,flags_buf,start_phy<end_phy?start_phy:end_phy,start_phy<end_phy?end_phy:start_phy);
				//刷新
				flags_pte = ((*pte_store)&0x1ff);
				start_vpn = start_va>>PGSHIFT;
				end_vpn = start_vpn;
				start_phy = (*pte_store>>PGSHIFT);
				end_phy = start_phy;
			}
			else
			{
				if(flags_pte == 0)
				{
					//刷新
					flags_pte = ((*pte_store)&0x1ff);
					start_vpn = start_va>>PGSHIFT;
					end_vpn = start_vpn;
					start_phy = (*pte_store>>PGSHIFT);
					end_phy = start_phy;
				}
				else
				{
					end_vpn = start_va>>PGSHIFT;
					end_phy = (*pte_store>>PGSHIFT);
				}
			}


		}
	}
	cprintf("\n");

	return 0;
}

int
mon_nextstep(int argc, char **argv, struct Trapframe *tf)
{
	if(!tf)
	{
		panic("empty Trapframe");
	}
	
	cprintf("$rip: %p\n",tf->tf_eip);
	switch(tf->tf_trapno)
	{
		case T_BRKPT:
			tf->tf_eflags |= FL_TF;
			return -1;
		case T_DEBUG:
			if (tf->tf_eflags & FL_TF)
            	return -1;
		default:
			cprintf("nextstep(ni) can only called via int 3(breakpoint exception)\n");
	}
	return 0;
}

int
mon_continue(int argc, char **argv, struct Trapframe *tf)
{
	if(!tf)
	{
		panic("empty Trapframe");
	}

	if(tf->tf_trapno==T_DEBUG||tf->tf_trapno==T_BRKPT)
	{
		if (tf->tf_eflags & FL_TF) 
		{
            tf->tf_eflags &= ~FL_TF;
            return -1;
        }
	}	

	cprintf("continue can only called via breakpoint or debug exception!\n");
	return 0;
}


/***** Kernel monitor command interpreter *****/

#define WHITESPACE "\t\r\n "
#define MAXARGS 16

static int
runcmd(char *buf, struct Trapframe *tf)
{
	int argc;
	char *argv[MAXARGS];
	int i;

	// Parse the command buffer into whitespace-separated arguments
	argc = 0;
	argv[argc] = 0;
	while (1) {
		// gobble whitespace
		while (*buf && strchr(WHITESPACE, *buf))
			*buf++ = 0;
		if (*buf == 0)
			break;

		// save and scan past next arg
		if (argc == MAXARGS-1) {
			cprintf("Too many arguments (max %d)\n", MAXARGS);
			return 0;
		}
		argv[argc++] = buf;
		while (*buf && !strchr(WHITESPACE, *buf))
			buf++;
	}
	argv[argc] = 0;

	// Lookup and invoke the command
	if (argc == 0)
		return 0;
	for (i = 0; i < ARRAY_SIZE(commands); i++) {
		if (strcmp(argv[0], commands[i].name) == 0)
			return commands[i].func(argc, argv, tf);
	}
	cprintf("Unknown command '%s'\n", argv[0]);
	return 0;
}

void
monitor(struct Trapframe *tf)
{
	char *buf;

	cprintf("Welcome to the JOS kernel monitor!\n");
	cprintf("Type 'help' for a list of commands.\n");

	if (tf != NULL)
		print_trapframe(tf);

	while (1) {
		buf = readline("K> ");
		if (buf != NULL)
			if (runcmd(buf, tf) < 0)
				break;
	}
}
