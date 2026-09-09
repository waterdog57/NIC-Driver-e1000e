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
};

static int nic_open(struct net_device *ndev)
{
	return 0;
}
static int nic_stop(struct net_device *ndev)
{
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
	err = dma_set_mask_and_coherent(&pdev->dev, DMA_BIT_MASK(64));
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

	pci_set_drvdata(pdev, priv);

	ndev->netdev_ops = &nic_netdev_ops;
	ndev->ethtool_ops = &nic_ethtool_ops;

	return 0;

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

	pci_iounmap(pdev, priv->hw_addr);
	pci_iounmap(pdev, priv->hw_flash_addr);
	pci_iounmap(pdev, priv->hw_io_addr);

	// 5. Free the net_device structure
	kfree(priv);

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
