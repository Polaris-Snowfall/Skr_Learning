#ifndef JOS_KERN_E1000_H
#define JOS_KERN_E1000_H

#include <kern/pci.h>
#include <inc/error.h>
#define PCI_E1000_VENDOR_ID 0x8086
#define PCI_E1000_DEVICE_ID 0x100E

struct e1000_rx_desc
{
    uint64_t addr;
    uint16_t length;
    uint16_t pkt_cks;
    uint8_t status;
    uint8_t errors;
    uint16_t special;
};

/* Transmit Descriptor */
struct e1000_tx_desc
{
	uint64_t addr;
	uint16_t length;
	uint8_t cso;
	uint8_t cmd;
	uint8_t status;
	uint8_t css;
	uint16_t special;
};

#define TRANS_DESC_SZ sizeof(struct e1000_tx_desc) 
#define NTRANS_DESC 128
#define NRECV_DESC 128
#define TDLEN TRANS_DESC_SZ*NTRANS_DESC
#define DESC_BUF_SZ 2048

/* Register Set. (82543, 82544)
 *
 * Registers are defined to be 32 bits and  should be accessed as 32 bit values.
 * These registers are physically located on the NIC, but are mapped into the
 * host memory address space.
 *
 * RW - register is both readable and writable
 * RO - register is read only
 * WO - register is write only
 * R/clr - register is read only and is cleared when read
 * A - register array
 */
#define E1000_STATUS   0x00008  /* Device Status - RO */
#define E1000_TDBAL    0x03800  /* TX Descriptor Base Address Low - RW */
#define E1000_TDBAH    0x03804  /* TX Descriptor Base Address High - RW */
#define E1000_TDLEN    0x03808  /* TX Descriptor Length - RW */
#define E1000_TDH      0x03810  /* TX Descriptor Head - RW */
#define E1000_TDT      0x03818  /* TX Descripotr Tail - RW */
#define E1000_TCTL     0x00400  /* TX Control - RW */
#define E1000_TCTL_EXT 0x00404  /* Extended TX Control - RW */
#define E1000_TIPG     0x00410  /* TX Inter-packet gap -RW */
#define E1000_RDBAL    0x02800  /* RX Descriptor Base Address Low - RW */
#define E1000_RDBAH    0x02804  /* RX Descriptor Base Address High - RW */
#define E1000_RDLEN    0x02808  /* RX Descriptor Length - RW */
#define E1000_RDH      0x02810  /* RX Descriptor Head - RW */
#define E1000_RDT      0x02818  /* RX Descriptor Tail - RW */
#define E1000_RDTR     0x02820  /* RX Delay Timer - RW */
#define E1000_RDBAL0   E1000_RDBAL /* RX Desc Base Address Low (0) - RW */
#define E1000_RDBAH0   E1000_RDBAH /* RX Desc Base Address High (0) - RW */#define E1000_RDLEN0   E1000_RDLEN /* RX Desc Length (0) - RW */
#define E1000_RDH0     E1000_RDH   /* RX Desc Head (0) - RW */
#define E1000_RDT0     E1000_RDT   /* RX Desc Tail (0) - RW */
#define E1000_RDTR0    E1000_RDTR  /* RX Delay Timer (0) - RW */
#define E1000_MTA      0x05200  /* Multicast Table Array - RW Array */
#define E1000_RAL       0x05400  /* Receive Address - RW Array */
#define E1000_RAH       0x05404  /* Receive Address - RW Array */
#define E1000_RCTL     0x00100  /* RX Control - RW */

/* Transmit Control */
#define E1000_TCTL_RST    0x00000001    /* software reset */
#define E1000_TCTL_EN     0x00000002    /* enable tx */
#define E1000_TCTL_BCE    0x00000004    /* busy check enable */
#define E1000_TCTL_PSP    0x00000008    /* pad short packets */
#define E1000_TCTL_CT     0x00000ff0    /* collision threshold */
#define E1000_TCTL_COLD   0x003ff000    /* collision distance */
#define E1000_TCTL_SWXOFF 0x00400000    /* SW Xoff transmission */
#define E1000_TCTL_PBE    0x00800000    /* Packet Burst Enable */
#define E1000_TCTL_RTLC   0x01000000    /* Re-transmit on late collision */
#define E1000_TCTL_NRTU   0x02000000    /* No Re-transmit on underrun */
#define E1000_TCTL_MULR   0x10000000    /* Multiple request support */
#define E1000_TCTL_COLD_SHIFT 12 


/* Transmit Descriptor bit definitions */
#define E1000_TXD_CMD_RS     0x8 /* Report Status */
#define E1000_TXD_CMD_EOP    0x1 /* End of Packet */
#define E1000_TXD_STAT_DD    0x1 /* Descriptor Done */

// #define E_TDQ_FULL 16
// #define E_RDQ_EMPTY 17

/* Receive Control */
#define E1000_RCTL_EN             0x00000002    /* enable */
#define E1000_RCTL_BAM            0x00008000    /* broadcast enable */
#define E1000_RCTL_SECRC          0x04000000    /* Strip Ethernet CRC */
#define E1000_RCTL_FLXBUF_SHIFT   27            /* Flexible buffer shift */
#define RCTL_BSIZE_SHIFT    16
#define E1000_RCTL_LPE            0x00000020    /* long packet enable */

/* Receive Descriptor bit definitions */
#define E1000_RXD_STAT_DD       0x01    /* Descriptor Done */
#define E1000_RXD_STAT_EOP      0x02    /* End of Packet */

#define MAC_IPL 0x12005452
#define MAC_IPH 0x5634

#define E1000_RAH_AV  0x80000000        /* Receive descriptor valid */


int e1000_attach(struct pci_func *pcif);
int tx_pkt(char* buf,size_t nbytes);
int rx_pkt(char* buf,size_t max_bytes);
#endif  // SOL >= 6
