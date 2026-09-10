// SPDX-License-Identifier: GPL-2.0
/*
 * e1000e.c - Minimal PCI driver skeleton for Intel(R) e1000e Gigabit NICs.
 *
 * This only claims the device, maps BAR0 (the MMIO register space), and
 * reads back the STATUS register to confirm the card responds. No
 * net_device, IRQ, or ring setup is wired up yet.
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/pci.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/ethtool.h>
#include <linux/interrupt.h>
#include <linux/version.h>
#include "mye1000e.h"

#define DRV_NAME "e1000e_waterdog"
#define DRV_VERSION "0.1"
#define NIC_MSIX_VECTORS (5)
#define E1000_IVAR_VAL_RX0 (0x0 | 0x80) /* Vector 0 + Enable (bit 7) */
#define E1000_IVAR_VAL_RX1 ((0x0 | 0x80) << 8) /* Vector 1 + Enable */
#define E1000_IVAR_VAL_TX0 ((0x1 | 0x80) << 16) /* Vector 2 + Enable */
#define E1000_IVAR_VAL_TX1 ((0x1 | 0x80) << 24) /* Vector 3 + Enable */
#define E1000_IVAR_VAL_OTHER (0x2 | 0x80) /* Vector 4 + Enable */

struct msix_entry msix_entries[NIC_MSIX_VECTORS];

#define DEBUG 1

struct nic_priv {
	struct pci_dev *pdev;
	struct net_device *ndev;
	int irq;
	spinlock_t lock; /* protects the device state */
	void __iomem *hw_addr; /* BAR0 mapped registers */
	void __iomem *hw_flash_addr; /* BAR1 mapped registers */
	void __iomem *hw_io_addr; /* BAR2 mapped registers */
	resource_size_t bar0_start;
	resource_size_t bar0_len;
	resource_size_t bar1_start;
	resource_size_t bar1_len;
	int num_vecs;
	u8 msix_enabled;
};

static irqreturn_t nic_msix_rx_isr(int irq, void *dev_id)
{
	// struct net_device *ndev = dev_id;
	// struct nic_priv *priv = netdev_priv(ndev);

	return IRQ_HANDLED;
}

static irqreturn_t nic_msix_tx_isr(int irq, void *dev_id)
{
	// struct net_device *ndev = dev_id;
	// struct nic_priv *priv = netdev_priv(ndev);

	return IRQ_HANDLED;
}

static irqreturn_t nic_msix_other_isr(int irq, void *dev_id)
{
	// struct net_device *ndev = dev_id;
	// struct nic_priv *priv = netdev_priv(ndev);

	return IRQ_HANDLED;
}

static int nic_open(struct net_device *ndev)
{
	struct nic_priv *priv = netdev_priv(ndev);
	int err, num_vecs;
	u32 ivar;

	netdev_info(ndev, "nic_open called\n");
	// 1.Request the IRQ for the device and register the interrupt handler
	num_vecs = pci_alloc_irq_vectors(priv->pdev, NIC_MSIX_VECTORS,
					 NIC_MSIX_VECTORS,
					 PCI_IRQ_MSIX | PCI_IRQ_MSI);
	if (num_vecs < 0) {
		dev_err(&priv->pdev->dev,
			"Failed to allocate MSI-X vectors: %d\n", num_vecs);
		return num_vecs;
	}
	priv->num_vecs = num_vecs;
	priv->msix_enabled = 1;

	/* 1. 2-Queue RSS */
	/* Bit 0:1 = 01b, Enable RSS with 2 Queues */
	iowrite32(0x00000001, priv->hw_addr + E1000_MRQC);

	ivar = E1000_IVAR_VAL_RX0 | E1000_IVAR_VAL_RX1 | E1000_IVAR_VAL_TX0 |
	       E1000_IVAR_VAL_TX1;
	iowrite32(ivar, priv->hw_addr + E1000_IVAR);
	iowrite32(E1000_IVAR_VAL_OTHER, priv->hw_addr + E1000_IVAR_MISC);

	// 2. Register the interrupt handler for the allocated IRQ
	/* Rx */
	err = request_irq(pci_irq_vector(priv->pdev, 0), nic_msix_rx_isr, 0,
			  "mye1000e-rx0", priv);
	if (err)
		goto err_req_rx0;
	err = request_irq(pci_irq_vector(priv->pdev, 1), nic_msix_rx_isr, 0,
			  "mye1000e-rx1", priv);
	if (err)
		goto err_req_rx1;

	/* Tx */
	err = request_irq(pci_irq_vector(priv->pdev, 2), nic_msix_tx_isr, 0,
			  "mye1000e-tx0", priv);
	if (err)
		goto err_req_tx0;
	err = request_irq(pci_irq_vector(priv->pdev, 3), nic_msix_tx_isr, 0,
			  "mye1000e-tx1", priv);
	if (err)
		goto err_req_tx1;

	/* Link/Other (Vector 2) */
	err = request_irq(pci_irq_vector(priv->pdev, 4), nic_msix_other_isr, 0,
			  "mye1000e-other", priv);
	if (err)
		goto err_req_other;

	return 0;

err_req_other:
	free_irq(pci_irq_vector(priv->pdev, 3), priv);
err_req_tx1:
	free_irq(pci_irq_vector(priv->pdev, 2), priv);
err_req_tx0:
	free_irq(pci_irq_vector(priv->pdev, 1), priv);
err_req_rx1:
	free_irq(pci_irq_vector(priv->pdev, 0), priv);
err_req_rx0:

	iowrite32(0, priv->hw_addr + E1000_IVAR);
	pci_free_irq_vectors(priv->pdev);
	return err;
}
static int nic_stop(struct net_device *ndev)
{
	struct nic_priv *priv = netdev_priv(ndev);

	iowrite32(0xFFFFFFFF, priv->hw_addr + 0x000D8); /* E1000_EIMC */
	iowrite32(0, priv->hw_addr + E1000_IVAR);

	// 2. Disable the device interrupts
	free_irq(pci_irq_vector(priv->pdev, 4), priv);
	free_irq(pci_irq_vector(priv->pdev, 3), priv);
	free_irq(pci_irq_vector(priv->pdev, 2), priv);
	free_irq(pci_irq_vector(priv->pdev, 1), priv);
	free_irq(pci_irq_vector(priv->pdev, 0), priv);

	iowrite32(0, priv->hw_addr + E1000_MRQC);

	// 1. Free the IRQs for the device
	pci_free_irq_vectors(priv->pdev);

	return 0;
}
static netdev_tx_t nic_start_xmit(struct sk_buff *skb, struct net_device *ndev)
{
	return NETDEV_TX_OK;
}
static void nic_set_rx_mode(struct net_device *ndev)
{
}
static void nic_tx_timeout(struct net_device *ndev, unsigned int txqueue)
{
}
static int nic_set_mac_address(struct net_device *ndev, void *addr)
{
	return 0;
}

static const struct net_device_ops nic_netdev_ops = {
	.ndo_open = nic_open,
	.ndo_stop = nic_stop,
	.ndo_start_xmit = nic_start_xmit,
	.ndo_set_rx_mode = nic_set_rx_mode,
	.ndo_tx_timeout = nic_tx_timeout,
	.ndo_set_mac_address = nic_set_mac_address,
	.ndo_validate_addr = eth_validate_addr,
};

static void nic_get_drvinfo(struct net_device *ndev,
			    struct ethtool_drvinfo *info)
{
	struct nic_priv *priv = netdev_priv(ndev);

	strscpy(info->driver, DRV_NAME, sizeof(info->driver));
	strscpy(info->version, DRV_VERSION, sizeof(info->version));
	strscpy(info->bus_info, pci_name(priv->pdev), sizeof(info->bus_info));
}

static const struct ethtool_ops nic_ethtool_ops = {
	.get_drvinfo = nic_get_drvinfo,
	.get_link = ethtool_op_get_link,
};

static int e1000e_probe(struct pci_dev *pdev, const struct pci_device_id *ent)
{
	struct net_device *ndev;
	struct nic_priv *priv;
	int err;
	u32 status;

	// 1. Enable the PCI device
	err = pci_enable_device(pdev);
	if (err)
		return dev_err_probe(&pdev->dev, err,
				     "pci_enable_device failed\n");

	// 2. Request the PCI regions (BARs) for the device
	err = pci_request_regions(pdev, DRV_NAME);
	if (err)
		goto err_disable_device;

	// 3. Set the device as a bus master to allow DMA
	pci_set_master(pdev);

	// 4. Set the DMA mask to 64 bits and ensure coherent DMA
	err = dma_set_mask_and_coherent(&pdev->dev, DMA_BIT_MASK(32));
	if (err) {
		dev_err(&pdev->dev, "no usable DMA configuration\n");
		goto err_release_regions;
	}

	// 5. Allocate a net_device structure with space for our private data
	ndev = alloc_etherdev(sizeof(*priv));
	if (!ndev) {
		err = -ENOMEM;
		goto err_release_regions;
	}
	// 6. Initialize the private data structure
	SET_NETDEV_DEV(ndev, &pdev->dev);
	priv = netdev_priv(ndev);
	priv->pdev = pdev;
	priv->ndev = ndev;
	priv->irq = pdev->irq;
	spin_lock_init(&priv->lock);

	// 7. Map the BAR0 MMIO region into kernel virtual address space
	priv->hw_addr = pci_iomap(pdev, 0, pci_resource_len(pdev, 0));
	if (!priv->hw_addr) {
		err = -EIO;
		goto err_free_netdev;
	}
	priv->bar0_start = pci_resource_start(pdev, 0);
	priv->bar0_len = pci_resource_len(pdev, 0);

	priv->hw_flash_addr = pci_iomap(pdev, 1, pci_resource_len(pdev, 1));
	if (!priv->hw_flash_addr) {
		err = -EIO;
		goto err_unmap_bar0;
	}
	priv->bar1_start = pci_resource_start(pdev, 1);
	priv->bar1_len = pci_resource_len(pdev, 1);

	priv->hw_io_addr = pci_iomap(pdev, 2, pci_resource_len(pdev, 2));
	if (!priv->hw_io_addr) {
		err = -EIO;
		goto err_unmap_bar1;
	}

	status = ioread32(priv->hw_addr + E1000_STATUS);
	dev_info(
		&pdev->dev,
		"%s v%s: found device %04x:%04x at %s, BAR0=%p, STATUS=0x%08x\n",
		DRV_NAME, DRV_VERSION, pdev->vendor, pdev->device,
		pci_name(pdev), priv->hw_addr, status);

	// 8. Store the private data pointer in the PCI device structure for later retrieval
	pci_set_drvdata(pdev, priv);

	eth_hw_addr_random(ndev);

	// 9. Initialize the net_device structure and register it with the kernel
	ndev->netdev_ops = &nic_netdev_ops;
	ndev->ethtool_ops = &nic_ethtool_ops;

	// 10. Register the net_device with the kernel networking subsystem
	err = register_netdev(ndev);
	if (err) {
		dev_err(&pdev->dev, "register_netdev failed: %d\n", err);
		goto err_unmap_bar2;
	}

	return 0;

err_unmap_bar2:
	if (priv->hw_io_addr)
		pci_iounmap(pdev, priv->hw_io_addr);

/* BAR4 mapping failed:
 * BAR4 was not successfully mapped,
 * so only BAR1 and BAR0 need cleanup.
 */
err_unmap_bar1:
	if (priv->hw_flash_addr)
		pci_iounmap(pdev, priv->hw_flash_addr);

/* BAR1 mapping failed:
 * BAR1 was not successfully mapped,
 * so only BAR0 needs cleanup.
 */
err_unmap_bar0:
	pci_iounmap(pdev, priv->hw_addr);

err_free_netdev:
	free_netdev(ndev);

err_release_regions:
	pci_release_regions(pdev);

err_disable_device:
	pci_disable_device(pdev);

	return err;
}

static void e1000e_remove(struct pci_dev *pdev)
{
	struct nic_priv *priv = pci_get_drvdata(pdev);

	// 10. Unregister the net_device from the kernel networking subsystem
	unregister_netdev(priv->ndev);

	pci_iounmap(pdev, priv->hw_addr);
	pci_iounmap(pdev, priv->hw_flash_addr);
	pci_iounmap(pdev, priv->hw_io_addr);

	// 5. Free the net_device structure
	free_netdev(priv->ndev);

	// 2. Release the PCI regions (BARs) for the device
	pci_release_regions(pdev);

	// 1. Disable the PCI device
	pci_disable_device(pdev);
}

static const struct pci_device_id e1000e_pci_tbl[] = {
	{ PCI_VDEVICE(INTEL, E1000_DEV_ID_82571EB_COPPER) },
	{ PCI_VDEVICE(INTEL, E1000_DEV_ID_82572EI_COPPER) },
	{ PCI_VDEVICE(INTEL, E1000_DEV_ID_82573E) },
	{ PCI_VDEVICE(INTEL, E1000_DEV_ID_82573L) },
	{ PCI_VDEVICE(INTEL, E1000_DEV_ID_82574L) },
	{ PCI_VDEVICE(INTEL, E1000_DEV_ID_82574LA) },
	{ PCI_VDEVICE(INTEL, E1000_DEV_ID_82583V) },
	{ PCI_VDEVICE(INTEL, E1000_DEV_ID_ICH8_IGP_M_AMT) },
	{ PCI_VDEVICE(INTEL, E1000_DEV_ID_ICH9_IGP_AMT) },
	{ PCI_VDEVICE(INTEL, E1000_DEV_ID_ICH10_R_BM_LM) },
	{ PCI_VDEVICE(INTEL, E1000_DEV_ID_PCH_M_HV_LM) },
	{ PCI_VDEVICE(INTEL, E1000_DEV_ID_PCH2_LV_LM) },
	{ PCI_VDEVICE(INTEL, E1000_DEV_ID_PCH_LPT_I217_LM) },
	{ PCI_VDEVICE(INTEL, E1000_DEV_ID_PCH_LPTLP_I218_LM) },
	{
		0,
	}
};
MODULE_DEVICE_TABLE(pci, e1000e_pci_tbl);

static struct pci_driver e1000e_pci_driver = {
	.name = DRV_NAME,
	.id_table = e1000e_pci_tbl,
	.probe = e1000e_probe,
	.remove = e1000e_remove,
};

module_pci_driver(e1000e_pci_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("waterdog <waterdog@google.com>");
MODULE_DESCRIPTION("Minimal PCI probe driver for Intel e1000e NICs");
MODULE_VERSION(DRV_VERSION);
