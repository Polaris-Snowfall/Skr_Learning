#include <kern/e1000.h>
#include <kern/pmap.h>
#include <inc/string.h>

// LAB 6: Your driver code here

volatile void* e1000;

__attribute__((__aligned__(16)))
struct e1000_tx_desc tds[NTRANS_DESC] = {0};

__attribute__((__aligned__(PGSIZE)))
char tdbufs[NTRANS_DESC][DESC_BUF_SZ] = {0};

__attribute__((__aligned__(16)))
struct e1000_rx_desc rds[NRECV_DESC] = {0};

__attribute__((__aligned__(PGSIZE)))
char rdbufs[NRECV_DESC][DESC_BUF_SZ] = {0};

inline uint32_t tdh()
{
    return *(uint32_t*)(e1000+E1000_TDH);
}

inline uint32_t tdt()
{
    return *(uint32_t*)(e1000+E1000_TDT);
}

inline uint32_t rdh()
{
    return *(uint32_t*)(e1000+E1000_RDH);
}

inline uint32_t rdt()
{
    return *(uint32_t*)(e1000+E1000_RDT);
}

int tx_init()
{
    for(int i = 0;i<NTRANS_DESC;++i)
    {
        tds[i].addr = PADDR(tdbufs[i]);
    }

    *(uint32_t*)(e1000+E1000_TDBAL) = PADDR(tds);
    *(uint32_t*)(e1000+E1000_TDBAH) = 0;
    *(uint32_t*)(e1000+E1000_TDLEN) = TDLEN;
    *(uint32_t*)(e1000+E1000_TDH) = 0;
    *(uint32_t*)(e1000+E1000_TDT) = 0;
    *(uint32_t*)(e1000+E1000_TCTL) = E1000_TCTL_EN|
                                    E1000_TCTL_PSP|
                                (E1000_TCTL_COLD&(0x40<<E1000_TCTL_COLD_SHIFT));
                                
    *(uint32_t*)(e1000+E1000_TIPG) = 10 | (8 << 10) | (12 << 20);

    return 0;
}

int rx_init()
{
    for(int i = 0;i<NRECV_DESC;++i)
    {
        rds[i].addr = PADDR(rdbufs[i]);
        rds[i].status &= ~E1000_RXD_STAT_DD;
    }


    *(uint32_t*)(e1000+E1000_RAL) = MAC_IPL;
    *(uint32_t*)(e1000+E1000_RAH) = MAC_IPH|E1000_RAH_AV;

    for(int i = 0;i<128;++i)
        ((uint32_t*)(e1000+E1000_MTA))[i] = 0;

    *(uint32_t*)(e1000+E1000_RDBAL) = PADDR(rds);
    *(uint32_t*)(e1000+E1000_RDBAH) = 0;

    *(uint32_t*)(e1000+E1000_RDLEN) = NRECV_DESC*DESC_BUF_SZ;
    *(uint32_t*)(e1000+E1000_RDH) = 0;
    *(uint32_t*)(e1000+E1000_RDT) = NRECV_DESC-1;
    
    *(uint32_t*)(e1000+E1000_RCTL) = (E1000_RCTL_EN | E1000_RCTL_SECRC | E1000_RCTL_BAM | 0<<RCTL_BSIZE_SHIFT) & (~E1000_RCTL_LPE);

    return 0;
}

int e1000_attach(struct pci_func *pcif)
{
    int r;
    pci_func_enable(pcif);

    e1000 = mmio_map_region(pcif->reg_base[0],pcif->reg_size[0]);
    cprintf("Device e1000 attached, status:%p\n",*(uint32_t*)(e1000+E1000_STATUS));

    if(rx_init())
        panic("e1000_attach: rx_init fail\n");    
    if(tx_init())
        panic("e1000_attach: tx_init fail\n");
    
    char buf[DESC_BUF_SZ];

    return 0;
}

int tx_pkt(char* buf,size_t nbytes)
{
    if(nbytes>DESC_BUF_SZ)
        panic("tx_pkt: invalid packet size\n");
    
    if((tds[tdt()].cmd&E1000_TXD_CMD_RS))
    {
        if(!(tds[tdt()].status&E1000_TXD_STAT_DD))
        {
            return -E_TDQ_FULL;
        }
    }
    memcpy(tdbufs[tdt()],buf,nbytes);
    tds[tdt()].length = (uint16_t)nbytes;
    tds[tdt()].cmd |= (E1000_TXD_CMD_RS|E1000_TXD_CMD_EOP);
    tds[tdt()].status &= (~E1000_TXD_STAT_DD);
    
    *(uint32_t*)(e1000+E1000_TDT) = (tdt()+1)%NTRANS_DESC;
    return 0;
}

int rx_pkt(char* buf,size_t max_bytes)
{
    uint32_t next = (rdt()+1)%NRECV_DESC;
    if((rds[next].status&E1000_RXD_STAT_DD)==0)
        return -E_RDQ_EMPTY;
    uint16_t lenth = rds[next].length;
    if(lenth>max_bytes)
        return -E_INVAL;
    memcpy(buf,rdbufs[next],lenth);
    rds[next].status &= (~E1000_RXD_STAT_DD);
    *(uint32_t*)(e1000+E1000_RDT) = next;
    return lenth;
}