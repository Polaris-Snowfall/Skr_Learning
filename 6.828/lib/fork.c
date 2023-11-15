// implement fork from user space

#include <inc/string.h>
#include <inc/lib.h>

// PTE_COW marks copy-on-write page table entries.
// It is one of the bits explicitly allocated to user processes (PTE_AVAIL).
#define PTE_COW		0x800

//
// Custom page fault handler - if faulting page is copy-on-write,
// map in our own private writable copy.
//
static void
pgfault(struct UTrapframe *utf)
{
	void *addr = (void *) utf->utf_fault_va;
	uint32_t err = utf->utf_err;
	int r;

	// Check that the faulting access was (1) a write, and (2) to a
	// copy-on-write page.  If not, panic.
	// Hint:
	//   Use the read-only page table mappings at uvpt
	//   (see <inc/memlayout.h>).

	// LAB 4: Your code here.
	// cprintf("access %p\n",addr);
	if((err&FEC_WR)==0)
	{
		panic("pgfault: the faulting access was not a write\n");
	}
	pte_t pte = ((pte_t*)UVPT)[PGNUM(addr)];
	if((pte&PTE_COW)==0)
		panic("pgfault: the faulting access was not to a copy-on-write page\n");
	
	// Allocate a new page, map it at a temporary location (PFTEMP),
	// copy the data from the old page to the new page, then move the new
	// page to the old page's address.
	// Hint:
	//   You should make three system calls.

	// LAB 4: Your code here.
	if((r = sys_page_alloc(0,PFTEMP,PTE_W|PTE_U)))
	{
		panic("pgfault: page_alloc error when COW\n");
	}
	memcpy((void*)PFTEMP,ROUNDDOWN(addr,PGSIZE),PGSIZE);
	if(sys_page_map(0,PFTEMP,0,ROUNDDOWN(addr,PGSIZE),PTE_W|(pte&PTE_SYSCALL&~PTE_COW)))
	{
		panic("pgfault: page_map error when COW\n");
	}
	if ((r = sys_page_unmap(0, (void *)PFTEMP)) < 0)
        panic("pgfault: sys_page_unmap() failed: %e\n", r);
}

//
// Map our virtual page pn (address pn*PGSIZE) into the target envid
// at the same virtual address.  If the page is writable or copy-on-write,
// the new mapping must be created copy-on-write, and then our mapping must be
// marked copy-on-write as well.  (Exercise: Why do we need to mark ours
// copy-on-write again if it was already copy-on-write at the beginning of
// this function?)
//
// Returns: 0 on success, < 0 on error.
// It is also OK to panic on error.
//
    static int
    duppage(envid_t ceid, unsigned pn)
    {
        // LAB 4: Your code here.
        int r;
        extern volatile pte_t uvpt[];
        envid_t peid = sys_getenvid();
        intptr_t va = (intptr_t)(pn * PGSIZE);
        if (uvpt[pn] & PTE_SHARE) {
            if ((r = sys_page_map(peid, (void *)va, ceid, (void *)va, uvpt[pn] & PTE_SYSCALL)) < 0)
                return r;
        } else if (uvpt[pn] & (PTE_COW|PTE_W)) {
            if ((r = sys_page_map(peid, (void *)va, ceid, (void *)va, (PTE_COW|PTE_U))) < 0)
                return r;
            if ((r = sys_page_map(peid, (void *)va, peid, (void *)va, (PTE_COW|PTE_U))) < 0)
                return r;
        } else {
            if ((r = sys_page_map(peid, (void *)va, ceid, (void *)va, PTE_U)) < 0)
                return r;
        }
        return 0;
    }

//
// User-level fork with copy-on-write.
// Set up our page fault handler appropriately.
// Create a child.
// Copy our address space and page fault handler setup to the child.
// Then mark the child as runnable and return.
//
// Returns: child's envid to the parent, 0 to the child, < 0 on error.
// It is also OK to panic on error.
//
// Hint:
//   Use uvpd, uvpt, and duppage.
//   Remember to fix "thisenv" in the child process.
//   Neither user exception stack should ever be marked copy-on-write,
//   so you must allocate a new page for the child's user exception stack.
//
envid_t
fork(void)
{
	// LAB 4: Your code here.
	envid_t ceid;
	int result;
	set_pgfault_handler(pgfault);

	if((ceid = sys_exofork())<0)
	{
		return ceid; 
	}
	else if(ceid == 0)
	{
		//child
		ceid = sys_getenvid();
		thisenv = &(envs[ceid&0x3FF]);
		return 0;
	}
	else
	{
		for (uintptr_t va = 0; va < UTOP;) 
		{
	    	extern volatile pde_t uvpd[];
    		extern volatile pte_t uvpt[];
			if ((uvpd[va >> PDXSHIFT] & PTE_P) == 0) 
			{    // page table page not found.
				va += NPTENTRIES * PGSIZE;
				continue;
			}
			if ((uvpt[va >> PTXSHIFT] & PTE_P) == 0) 
			{    // page table entry not found.
				va += PGSIZE;
				continue;
			}
			if (va == UXSTACKTOP - PGSIZE) 
			{    // UXSTACKTOP is not remmaped!
				va += PGSIZE;
				continue;
			}
			// this page should be duppage()d.
			if ((result = duppage(ceid, (unsigned)(va/PGSIZE))) < 0)
			{
				return result;
			}
			va += PGSIZE;
		}

   		 if ((result = sys_page_alloc(ceid, (void *)(UXSTACKTOP-PGSIZE), (PTE_U|PTE_W))) < 0)
        return result;
	extern void _pgfault_upcall(void);
	sys_env_set_pgfault_upcall(ceid,_pgfault_upcall);
		sys_env_set_status(ceid,ENV_RUNNABLE);
		return ceid;
	}
}

// Challenge!
int
sfork(void)
{
	panic("sfork not implemented");
	return -E_INVAL;
}
