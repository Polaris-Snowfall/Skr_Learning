#include "ns.h"
#include <inc/lib.h>

extern union Nsipc nsipcbuf;

void
output(envid_t ns_envid)
{
	binaryname = "ns_output";

	// LAB 6: Your code here:
	// 	- read a packet from the network server
	//	- send the packet to the device driver
	int r;
	while(true)
	{
		envid_t from_env_store;
		r = ipc_recv(&from_env_store,&nsipcbuf,0);
		if(from_env_store==0)
			panic("output: ipc_recv fail\n");
		if(from_env_store!=ns_envid)
		{
			cprintf("unexpected IPC sender\n");
			continue;
		}
		if((r != NSREQ_OUTPUT))
		{
			cprintf("output: unexpected IPC type!\n");
			continue;
		}       
		while(transpackt(nsipcbuf.pkt.jp_data,nsipcbuf.pkt.jp_len)==-E_TDQ_FULL)
		{
            cprintf("output: the transmit queue is full,please retry\n");
			sys_yield();
			continue;
		}
	}
}
