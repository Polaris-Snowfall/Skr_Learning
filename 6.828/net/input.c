#include "ns.h"
#include <inc/lib.h>

#define DESC_BUF_SZ 2048
extern union Nsipc nsipcbuf;

void
input(envid_t ns_envid)
{
	binaryname = "ns_input";
	char buf[DESC_BUF_SZ];
	// LAB 6: Your code here:
	// 	- read a packet from the device driver
	//	- send it to the network server
	// Hint: When you IPC a page to the network server, it will be
	// reading from it for a while, so don't immediately receive
	// another packet in to the same physical page.
	int r;
	while(true)
	{
		r = recvpackt(buf,DESC_BUF_SZ,&(nsipcbuf.pkt.jp_len));
		if(r==-E_INVAL)
		{
			panic("packt data larger than bufsize\n");
		}
		if(r==-E_RDQ_EMPTY)
		{
            // cprintf("input: the recv queue is empty,please wait\n");
			sys_yield();
		};
		memcpy(nsipcbuf.pkt.jp_data,buf,nsipcbuf.pkt.jp_len);

		ipc_send(ns_envid,NSREQ_INPUT,&nsipcbuf,PTE_U);
		sleep(80);
	}
}
