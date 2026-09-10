/* SPDX-License-Identifier: GPL-2.0 */
/*
 * e1000e.h - Intel(R) PRO/1000 PCI Express Gigabit Ethernet register map.
 *
 * Register offsets (from the start of BAR0 MMIO space) and bit definitions
 * for the e1000e family: 82571/82572/82573/82574/82583, ICH8-ICH10, and the
 * PCH/PCH2/PCH_LPT LOM parts. Values follow Intel's public datasheets for
 * these controllers.
 */
#ifndef _E1000E_H_
#define _E1000E_H_

/* =======================================================================
 * General / Control
 * ===================================================================== */
#define E1000_CTRL 0x00000 /* Device Control */
#define E1000_STATUS 0x00008 /* Device Status */
#define E1000_EECD 0x00010 /* EEPROM/Flash Control */
#define E1000_EERD 0x00014 /* EEPROM Read */
#define E1000_CTRL_EXT 0x00018 /* Extended Device Control */
#define E1000_FLA 0x0001C /* Flash Access */
#define E1000_MDIC 0x00020 /* MDI Control */
#define E1000_SCTL 0x00024 /* SerDes Control */
#define E1000_FCAL 0x00028 /* Flow Control Address Low */
#define E1000_FCAH 0x0002C /* Flow Control Address High */
#define E1000_FCT 0x00030 /* Flow Control Type */
#define E1000_VET 0x00038 /* VLAN EtherType */
#define E1000_FCTTV 0x00170 /* Flow Control Transmit Timer Value */
#define E1000_TXCW 0x00178 /* Tx Configuration Word (legacy SerDes) */
#define E1000_RXCW 0x00180 /* Rx Configuration Word (legacy SerDes) */
#define E1000_LEDCTL 0x00E00 /* LED Control */
#define E1000_PBA 0x01000 /* Packet Buffer Allocation */
#define E1000_PBS 0x01008 /* Packet Buffer Size */
#define E1000_EEMNGCTL 0x01010 /* Manageability EEPROM Control */
#define E1000_FLASHT 0x01028 /* Flash Timer */
#define E1000_EEWR 0x0102C /* EEPROM Write */
#define E1000_FLSWCTL 0x01030 /* Flash Software Control */
#define E1000_FLSWDATA 0x01034 /* Flash Software Data */
#define E1000_FLSWCNT 0x01038 /* Flash Software Counter */
#define E1000_FLOP 0x0103C /* Flash Opcode */
#define E1000_PBECCSTS 0x01100 /* Packet Buffer ECC Status */
#define E1000_FACTPS 0x05B30 /* Function Active/Power State to Manageability */
#define E1000_SWSM 0x05B50 /* Software Semaphore */
#define E1000_FWSM 0x05B54 /* Firmware Semaphore */
#define E1000_HICR 0x08F00 /* Host Interface Control */

/* CTRL register bits */
#define E1000_CTRL_FD 0x00000001 /* Full duplex */
#define E1000_CTRL_GIO_MASTER_DISABLE 0x00000004
#define E1000_CTRL_ASDE 0x00000020 /* Auto-speed detect enable */
#define E1000_CTRL_SLU 0x00000040 /* Set link up */
#define E1000_CTRL_ILOS 0x00000080 /* Invert loss-of-signal */
#define E1000_CTRL_SPD_SEL 0x00000300 /* Speed select mask */
#define E1000_CTRL_SPD_10 0x00000000
#define E1000_CTRL_SPD_100 0x00000100
#define E1000_CTRL_SPD_1000 0x00000200
#define E1000_CTRL_FRCSPD 0x00000800 /* Force speed */
#define E1000_CTRL_FRCDPX 0x00001000 /* Force duplex */
#define E1000_CTRL_LRST 0x00000008 /* Link reset */
#define E1000_CTRL_RST 0x04000000 /* Global device reset */
#define E1000_CTRL_RFCE 0x08000000 /* Receive flow control enable */
#define E1000_CTRL_TFCE 0x10000000 /* Transmit flow control enable */
#define E1000_CTRL_VME 0x40000000 /* VLAN mode enable */
#define E1000_CTRL_PHY_RST 0x80000000 /* PHY reset */

/* STATUS register bits */
#define E1000_STATUS_FD 0x00000001 /* Full duplex */
#define E1000_STATUS_LU 0x00000002 /* Link up */
#define E1000_STATUS_FUNC_MASK 0x0000000C
#define E1000_STATUS_TXOFF 0x00000010 /* Tx paused */
#define E1000_STATUS_SPEED_MASK 0x000000C0
#define E1000_STATUS_SPEED_10 0x00000000
#define E1000_STATUS_SPEED_100 0x00000040
#define E1000_STATUS_SPEED_1000 0x00000080
#define E1000_STATUS_ASDV 0x00000300 /* Auto-speed detect value */
#define E1000_STATUS_PHYRA 0x00000400 /* PHY reset asserted */
#define E1000_STATUS_GIO_MASTER_ENABLE 0x00080000

/* CTRL_EXT register bits */
#define E1000_CTRL_EXT_SDP4_DATA 0x00000010
#define E1000_CTRL_EXT_ASDCHK 0x00001000
#define E1000_CTRL_EXT_EE_RST 0x00002000
#define E1000_CTRL_EXT_SPD_BYPS 0x00008000
#define E1000_CTRL_EXT_RO_DIS 0x00020000
#define E1000_CTRL_EXT_DMA_DYN_CLK_EN 0x00080000
#define E1000_CTRL_EXT_LINK_MODE_MASK 0x00C00000
#define E1000_CTRL_EXT_LINK_MODE_PCIE_SERDES 0x00C00000
#define E1000_CTRL_EXT_EIAME 0x01000000
#define E1000_CTRL_EXT_IAME 0x08000000
#define E1000_CTRL_EXT_PBA_CLR 0x80000000

/* =======================================================================
 * EEPROM / Flash / MDI (PHY) access
 * ===================================================================== */
#define E1000_EECD_SK 0x00000001 /* Clock */
#define E1000_EECD_CS 0x00000002 /* Chip select */
#define E1000_EECD_DI 0x00000004 /* Data in */
#define E1000_EECD_DO 0x00000008 /* Data out */
#define E1000_EECD_FWE_MASK 0x00000030 /* Flash write enable */
#define E1000_EECD_REQ 0x00000040 /* EEPROM access request */
#define E1000_EECD_GNT 0x00000080 /* EEPROM access grant */
#define E1000_EECD_PRES 0x00000100 /* EEPROM present */
#define E1000_EECD_SIZE_EX_MASK 0x00007800

#define E1000_EERD_START 0x00000001
#define E1000_EERD_DONE 0x00000002
#define E1000_EERD_ADDR_SHIFT 2
#define E1000_EERD_ADDR_MASK 0x0000FFFC
#define E1000_EERD_DATA_SHIFT 16
#define E1000_EERD_DATA_MASK 0xFFFF0000

#define E1000_MDIC_DATA_MASK 0x0000FFFF
#define E1000_MDIC_REG_SHIFT 16
#define E1000_MDIC_REG_MASK 0x001F0000
#define E1000_MDIC_PHY_SHIFT 21
#define E1000_MDIC_PHY_MASK 0x03E00000
#define E1000_MDIC_OP_WRITE 0x04000000
#define E1000_MDIC_OP_READ 0x08000000
#define E1000_MDIC_READY 0x10000000
#define E1000_MDIC_INT_EN 0x20000000
#define E1000_MDIC_ERROR 0x40000000

/* =======================================================================
 * Interrupts
 * ===================================================================== */
#define E1000_ICR 0x000C0 /* Interrupt Cause Read */
#define E1000_ITR 0x000C4 /* Interrupt Throttling Rate */
#define E1000_ICS 0x000C8 /* Interrupt Cause Set */
#define E1000_IMS 0x000D0 /* Interrupt Mask Set/Read */
#define E1000_IMC 0x000D8 /* Interrupt Mask Clear */
#define E1000_IAM 0x000E0 /* Interrupt Acknowledge Auto Mask */
#define E1000_EIAC 0x000DC /* Extended Interrupt Auto Clear (82574) */
#define E1000_IVAR 0x000E4 /* Interrupt Vector Allocation (82574) */
#define E1000_IVAR_MISC \
	0x000E8 /* Interrupt Vector Allocation for Misc (82574) */
#define E1000_EITR(_n) (0x000E8 + (_n) * 4) /* Extended ITR, 82574: 0-4 */
#define E1000_GPIE 0x01514 /* General Purpose Interrupt Enable */

/* Interrupt Cause / Mask bits (ICR/ICS/IMS/IMC common layout) */
#define E1000_ICR_TXDW 0x00000001 /* Tx desc written back */
#define E1000_ICR_TXQE 0x00000002 /* Tx queue empty */
#define E1000_ICR_LSC 0x00000004 /* Link status change */
#define E1000_ICR_RXSEQ 0x00000008 /* Rx sequence error */
#define E1000_ICR_RXDMT0 0x00000010 /* Rx desc min threshold */
#define E1000_ICR_RXO 0x00000040 /* Rx overrun */
#define E1000_ICR_RXT0 0x00000080 /* Rx timer interrupt */
#define E1000_ICR_MDAC 0x00000200 /* MDIO access complete */
#define E1000_ICR_RXCFG 0x00000400 /* Rx /C/ ordered set */
#define E1000_ICR_GPI_EN0 0x00000800
#define E1000_ICR_GPI_EN1 0x00001000
#define E1000_ICR_GPI_EN2 0x00002000
#define E1000_ICR_GPI_EN3 0x00004000
#define E1000_ICR_TXD_LOW 0x00008000
#define E1000_ICR_SRPD 0x00010000 /* Small receive packet detect */
#define E1000_ICR_ACK 0x00020000 /* Receive ACK frame */
#define E1000_ICR_MNG 0x00040000 /* Manageability event */
#define E1000_ICR_ECCER 0x00400000 /* ECC error */
#define E1000_ICR_INT_ASSERTED 0x80000000 /* PCIe interrupt asserted (82574+) */

/* =======================================================================
 * Receive
 * ===================================================================== */
#define E1000_RCTL 0x00100 /* Rx Control */
#define E1000_ERT 0x02008 /* Early Rx Threshold */
#define E1000_FCRTL 0x02160 /* Flow Control Rx Threshold Low */
#define E1000_FCRTH 0x02168 /* Flow Control Rx Threshold High */
#define E1000_PSRCTL 0x02170 /* Packet Split Receive Control */
#define E1000_DRXMXOD 0x02540 /* DMA Rx Max Outstanding Data */
#define E1000_RXPBS 0x02404 /* Rx Packet Buffer Size */
#define E1000_RXCSUM 0x05000 /* Rx Checksum Control */
#define E1000_RLPML 0x05004 /* Rx Long Packet Max Length */
#define E1000_RFCTL 0x05008 /* Receive Filter Control */
#define E1000_MTA 0x05200 /* Multicast Table Array, 128 entries */
#define E1000_RA 0x05400 /* Receive Address, 16 entries (RAL/RAH pairs) */
#define E1000_VFTA 0x05600 /* VLAN Filter Table Array, 128 entries */
#define E1000_MRQC 0x05818 /* Multiple Rx Queue Control */
#define E1000_RSSIM 0x05864 /* RSS Interrupt Mask */
#define E1000_RSSIR 0x05868 /* RSS Interrupt Request */
#define E1000_RETA 0x05C00 /* Redirection Table, 32 entries */
#define E1000_RSSRK 0x05C80 /* RSS Random Key, 10 entries */

/* Per-queue Rx descriptor ring registers, queue n = 0 or 1 */
#define E1000_RDBAL(_n) (0x02800 + (_n) * 0x100) /* Rx Desc Base Addr Low */
#define E1000_RDBAH(_n) (0x02804 + (_n) * 0x100) /* Rx Desc Base Addr High */
#define E1000_RDLEN(_n) (0x02808 + (_n) * 0x100) /* Rx Desc Length */
#define E1000_RDH(_n) (0x02810 + (_n) * 0x100) /* Rx Desc Head */
#define E1000_RDT(_n) (0x02818 + (_n) * 0x100) /* Rx Desc Tail */
#define E1000_RXDCTL(_n) (0x02828 + (_n) * 0x100) /* Rx Desc Control */
#define E1000_RDTR 0x02820 /* Rx Delay Timer (legacy, queue 0) */
#define E1000_RADV 0x0282C /* Rx Interrupt Absolute Delay Timer */
#define E1000_RSRPD 0x02C00 /* Rx Small Packet Detect Interrupt */

/* RCTL bits */
#define E1000_RCTL_EN 0x00000002 /* Receiver enable */
#define E1000_RCTL_SBP 0x00000004 /* Store bad packets */
#define E1000_RCTL_UPE 0x00000008 /* Unicast promiscuous */
#define E1000_RCTL_MPE 0x00000010 /* Multicast promiscuous */
#define E1000_RCTL_LPE 0x00000020 /* Long packet enable */
#define E1000_RCTL_LBM_NONE 0x00000000 /* No loopback */
#define E1000_RCTL_LBM_MAC 0x00000040 /* MAC loopback */
#define E1000_RCTL_RDMTS_HALF 0x00000000
#define E1000_RCTL_MO_SHIFT 12 /* Multicast offset */
#define E1000_RCTL_BAM 0x00008000 /* Broadcast accept mode */
#define E1000_RCTL_SZ_2048 0x00000000
#define E1000_RCTL_SZ_1024 0x00010000
#define E1000_RCTL_SZ_512 0x00020000
#define E1000_RCTL_SZ_256 0x00030000
#define E1000_RCTL_SZ_16384 0x00010000 /* with BSEX set */
#define E1000_RCTL_SZ_8192 0x00020000 /* with BSEX set */
#define E1000_RCTL_SZ_4096 0x00030000 /* with BSEX set */
#define E1000_RCTL_VFE 0x00040000 /* VLAN filter enable */
#define E1000_RCTL_CFIEN 0x00080000 /* Canonical form indicator enable */
#define E1000_RCTL_CFI 0x00100000
#define E1000_RCTL_DPF 0x00400000 /* Discard pause frames */
#define E1000_RCTL_PMCF 0x00800000 /* Pass MAC control frames */
#define E1000_RCTL_BSEX 0x02000000 /* Buffer size extension */
#define E1000_RCTL_SECRC 0x04000000 /* Strip Ethernet CRC */

/* =======================================================================
 * Transmit
 * ===================================================================== */
#define E1000_TCTL 0x00400 /* Tx Control */
#define E1000_TCTL_EXT 0x00404 /* Tx Control Extended */
#define E1000_TIPG 0x00410 /* Tx Inter-Packet Gap */
#define E1000_AIT 0x00458 /* Adaptive IFS Throttle */
#define E1000_TXPBS 0x03404 /* Tx Packet Buffer Size */
#define E1000_TXDMAC 0x03000 /* Tx DMA Control */
#define E1000_TDFH 0x03410 /* Tx Data FIFO Head */
#define E1000_TDFT 0x03418 /* Tx Data FIFO Tail */
#define E1000_TDFHS 0x03420 /* Tx Data FIFO Head Saved */
#define E1000_TDFTS 0x03428 /* Tx Data FIFO Tail Saved */
#define E1000_TDFPC 0x03430 /* Tx Data FIFO Packet Count */
#define E1000_TARC(_n) (0x03840 + (_n) * 0x100) /* Tx Arbitration Count */

/* Per-queue Tx descriptor ring registers, queue n = 0 or 1 */
#define E1000_TDBAL(_n) (0x03800 + (_n) * 0x100) /* Tx Desc Base Addr Low */
#define E1000_TDBAH(_n) (0x03804 + (_n) * 0x100) /* Tx Desc Base Addr High */
#define E1000_TDLEN(_n) (0x03808 + (_n) * 0x100) /* Tx Desc Length */
#define E1000_TDH(_n) (0x03810 + (_n) * 0x100) /* Tx Desc Head */
#define E1000_TDT(_n) (0x03818 + (_n) * 0x100) /* Tx Desc Tail */
#define E1000_TXDCTL(_n) (0x03828 + (_n) * 0x100) /* Tx Desc Control */
#define E1000_TIDV 0x03820 /* Tx Interrupt Delay Value (legacy, queue 0) */
#define E1000_TADV 0x0382C /* Tx Interrupt Absolute Delay Timer */
#define E1000_TSPMT 0x03830 /* TCP Segmentation Pad and Min Threshold */

/* TCTL bits */
#define E1000_TCTL_EN 0x00000002 /* Transmit enable */
#define E1000_TCTL_PSP 0x00000008 /* Pad short packets */
#define E1000_TCTL_CT_SHIFT 4 /* Collision threshold */
#define E1000_TCTL_COLD_SHIFT 12 /* Collision distance */
#define E1000_TCTL_SWXOFF 0x00400000 /* Software XOFF transmission */
#define E1000_TCTL_RTLC 0x01000000 /* Re-transmit on late collision */

/* Tx/Rx descriptor status bits (legacy descriptor format) */
#define E1000_TXD_STAT_DD 0x00000001 /* Descriptor done */
#define E1000_TXD_STAT_EC 0x00000002 /* Excess collisions */
#define E1000_TXD_STAT_LC 0x00000004 /* Late collision */
#define E1000_TXD_CMD_EOP 0x01 /* End of packet */
#define E1000_TXD_CMD_IFCS 0x02 /* Insert FCS */
#define E1000_TXD_CMD_IC 0x04 /* Insert checksum */
#define E1000_TXD_CMD_RS 0x08 /* Report status */
#define E1000_TXD_CMD_RPS 0x10 /* Report packet sent */
#define E1000_TXD_CMD_DEXT 0x20 /* Descriptor extension (0 = legacy) */
#define E1000_TXD_CMD_VLE 0x40 /* Add VLAN */
#define E1000_TXD_CMD_IDE 0x80 /* Interrupt delay enable */

#define E1000_RXD_STAT_DD 0x01 /* Descriptor done */
#define E1000_RXD_STAT_EOP 0x02 /* End of packet */
#define E1000_RXD_STAT_IXSM 0x04 /* Ignore checksum */
#define E1000_RXD_STAT_VP 0x08 /* VLAN packet */
#define E1000_RXD_STAT_TCPCS 0x20 /* TCP checksum calculated */
#define E1000_RXD_STAT_IPCS 0x40 /* IP checksum calculated */
#define E1000_RXD_STAT_PIF 0x80 /* Passed in-exact filter */
#define E1000_RXD_ERR_CE 0x01 /* CRC error */
#define E1000_RXD_ERR_SE 0x02 /* Symbol error */
#define E1000_RXD_ERR_SEQ 0x04 /* Sequence error */
#define E1000_RXD_ERR_CXE 0x10 /* Carrier extension error */
#define E1000_RXD_ERR_TCPE 0x20 /* TCP/UDP checksum error */
#define E1000_RXD_ERR_IPE 0x40 /* IP checksum error */
#define E1000_RXD_ERR_RXE 0x80 /* Rx data error */

/* =======================================================================
 * Wake-up / Management
 * ===================================================================== */
#define E1000_WUC 0x05800 /* Wakeup Control */
#define E1000_WUFC 0x05808 /* Wakeup Filter Control */
#define E1000_WUS 0x05810 /* Wakeup Status */
#define E1000_MANC 0x05820 /* Management Control */
#define E1000_IPAV 0x05838 /* IP Address Valid */
#define E1000_IP4AT 0x05840 /* IPv4 Address Table */
#define E1000_IP6AT 0x05880 /* IPv6 Address Table */
#define E1000_WUPL 0x05900 /* Wakeup Packet Length */
#define E1000_WUPM 0x05A00 /* Wakeup Packet Memory, 128 bytes */
#define E1000_FFLT 0x05F00 /* Flexible Filter Length Table */
#define E1000_HOST_IF 0x08800 /* Host Interface memory, 512 bytes */

/* =======================================================================
 * Statistics (all read-clear-on-read 32-bit counters unless noted)
 * ===================================================================== */
#define E1000_CRCERRS 0x04000 /* CRC Error Count */
#define E1000_ALGNERRC 0x04004 /* Alignment Error Count */
#define E1000_SYMERRS 0x04008 /* Symbol Error Count */
#define E1000_RXERRC 0x0400C /* Rx Error Count */
#define E1000_MPC 0x04010 /* Missed Packets Count */
#define E1000_SCC 0x04014 /* Single Collision Count */
#define E1000_ECOL 0x04018 /* Excessive Collisions Count */
#define E1000_MCC 0x0401C /* Multiple Collision Count */
#define E1000_LATECOL 0x04020 /* Late Collisions Count */
#define E1000_COLC 0x04028 /* Collision Count */
#define E1000_DC 0x04030 /* Defer Count */
#define E1000_TNCRS 0x04034 /* Tx-No CRS */
#define E1000_SEC 0x04038 /* Sequence Error Count */
#define E1000_CEXTERR 0x0403C /* Carrier Extension Error Count */
#define E1000_RLEC 0x04040 /* Receive Length Error Count */
#define E1000_XONRXC 0x04048 /* XON Received Count */
#define E1000_XONTXC 0x0404C /* XON Transmitted Count */
#define E1000_XOFFRXC 0x04050 /* XOFF Received Count */
#define E1000_XOFFTXC 0x04054 /* XOFF Transmitted Count */
#define E1000_FCRUC 0x04058 /* FC Received Unsupported Count */
#define E1000_PRC64 0x0405C /* Packets Rx (64 bytes) */
#define E1000_PRC127 0x04060 /* Packets Rx (65-127 bytes) */
#define E1000_PRC255 0x04064 /* Packets Rx (128-255 bytes) */
#define E1000_PRC511 0x04068 /* Packets Rx (256-511 bytes) */
#define E1000_PRC1023 0x0406C /* Packets Rx (512-1023 bytes) */
#define E1000_PRC1522 0x04070 /* Packets Rx (1024-max bytes) */
#define E1000_GPRC 0x04074 /* Good Packets Rx Count */
#define E1000_BPRC 0x04078 /* Broadcast Packets Rx Count */
#define E1000_MPRC 0x0407C /* Multicast Packets Rx Count */
#define E1000_GPTC 0x04080 /* Good Packets Tx Count */
#define E1000_GORCL 0x04088 /* Good Octets Rx Count Low */
#define E1000_GORCH 0x0408C /* Good Octets Rx Count High */
#define E1000_GOTCL 0x04090 /* Good Octets Tx Count Low */
#define E1000_GOTCH 0x04094 /* Good Octets Tx Count High */
#define E1000_RNBC 0x040A0 /* Rx No Buffers Count */
#define E1000_RUC 0x040A4 /* Rx Undersize Count */
#define E1000_RFC 0x040A8 /* Rx Fragment Count */
#define E1000_ROC 0x040AC /* Rx Oversize Count */
#define E1000_RJC 0x040B0 /* Rx Jabber Count */
#define E1000_MGTPRC 0x040B4 /* Management Packets Rx Count */
#define E1000_MGTPDC 0x040B8 /* Management Packets Dropped Count */
#define E1000_MGTPTC 0x040BC /* Management Packets Tx Count */
#define E1000_TORL 0x040C0 /* Total Octets Rx Low */
#define E1000_TORH 0x040C4 /* Total Octets Rx High */
#define E1000_TOTL 0x040C8 /* Total Octets Tx Low */
#define E1000_TOTH 0x040CC /* Total Octets Tx High */
#define E1000_TPR 0x040D0 /* Total Packets Rx */
#define E1000_TPT 0x040D4 /* Total Packets Tx */
#define E1000_PTC64 0x040D8 /* Packets Tx (64 bytes) */
#define E1000_PTC127 0x040DC /* Packets Tx (65-127 bytes) */
#define E1000_PTC255 0x040E0 /* Packets Tx (128-255 bytes) */
#define E1000_PTC511 0x040E4 /* Packets Tx (256-511 bytes) */
#define E1000_PTC1023 0x040E8 /* Packets Tx (512-1023 bytes) */
#define E1000_PTC1522 0x040EC /* Packets Tx (1024-max bytes) */
#define E1000_MPTC 0x040F0 /* Multicast Packets Tx Count */
#define E1000_BPTC 0x040F4 /* Broadcast Packets Tx Count */
#define E1000_TSCTC 0x040F8 /* TCP Segmentation Context Tx Count */
#define E1000_TSCTFC 0x040FC /* TCP Segmentation Context Tx Fail Count */
#define E1000_IAC 0x04100 /* Interrupt Assertion Count */
#define E1000_ICRXPTC 0x04104 /* Interrupt Cause Rx Pkt Timer Expire Count */
#define E1000_ICRXATC 0x04108 /* Interrupt Cause Rx Abs Timer Expire Count */
#define E1000_ICTXPTC 0x0410C /* Interrupt Cause Tx Pkt Timer Expire Count */
#define E1000_ICTXATC 0x04110 /* Interrupt Cause Tx Abs Timer Expire Count */
#define E1000_ICTXQEC 0x04118 /* Interrupt Cause Tx Queue Empty Count */
#define E1000_ICTXQMTC 0x0411C /* Interrupt Cause Tx Queue Min Thresh Count */
#define E1000_ICRXDMTC 0x04120 /* Interrupt Cause Rx Desc Min Thresh Count */
#define E1000_ICRXOC 0x04124 /* Interrupt Cause Receiver Overrun Count */

/* =======================================================================
 * PCI / Function-level identifiers used for pci_device_id tables
 * ===================================================================== */
#ifndef PCI_VENDOR_ID_INTEL
#define PCI_VENDOR_ID_INTEL 0x8086
#endif

/* A representative subset of e1000e-supported device IDs. */
#define E1000_DEV_ID_82571EB_COPPER 0x105E
#define E1000_DEV_ID_82571EB_FIBER 0x105F
#define E1000_DEV_ID_82571EB_SERDES 0x1060
#define E1000_DEV_ID_82572EI_COPPER 0x107D
#define E1000_DEV_ID_82572EI_FIBER 0x107E
#define E1000_DEV_ID_82572EI_SERDES 0x107F
#define E1000_DEV_ID_82573E 0x108B
#define E1000_DEV_ID_82573E_IAMT 0x108C
#define E1000_DEV_ID_82573L 0x109A
#define E1000_DEV_ID_82574L 0x10D3
#define E1000_DEV_ID_82574LA 0x10F6
#define E1000_DEV_ID_82583V 0x150C
#define E1000_DEV_ID_ICH8_IGP_M_AMT 0x1049
#define E1000_DEV_ID_ICH8_IGP_AMT 0x104A
#define E1000_DEV_ID_ICH8_IFE 0x104C
#define E1000_DEV_ID_ICH8_82567V_3 0x1501
#define E1000_DEV_ID_ICH9_IGP_AMT 0x10BD
#define E1000_DEV_ID_ICH9_BM 0x10E5
#define E1000_DEV_ID_ICH10_R_BM_LM 0x10CB
#define E1000_DEV_ID_ICH10_R_BM_LF 0x10CC
#define E1000_DEV_ID_PCH_M_HV_LM 0x10EA
#define E1000_DEV_ID_PCH_M_HV_LC 0x10EB
#define E1000_DEV_ID_PCH_D_HV_DM 0x10EF
#define E1000_DEV_ID_PCH_D_HV_DC 0x10F0
#define E1000_DEV_ID_PCH2_LV_LM 0x1502
#define E1000_DEV_ID_PCH2_LV_V 0x1503
#define E1000_DEV_ID_PCH_LPT_I217_LM 0x153A
#define E1000_DEV_ID_PCH_LPT_I217_V 0x153B
#define E1000_DEV_ID_PCH_LPTLP_I218_LM 0x155A
#define E1000_DEV_ID_PCH_LPTLP_I218_V 0x1559

#endif /* _E1000E_H_ */
