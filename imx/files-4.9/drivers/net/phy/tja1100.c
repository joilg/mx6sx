/* drivers/net/phy/tja1100.c
 *
 * Driver for TJA1100 BroadR-Reach PHY
 *
 * Author: Johannes Ilg
 *
 * Copyright (c) 2015 Continental Automotive GmbH, Regensburg
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 *
 */

#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/mii.h>
#include <linux/phy.h>
#include <linux/ethtool.h>




/*
include <linux/string.h>
include <linux/unistd.h>
include <linux/interrupt.h>
include <linux/delay.h>
include <linux/device.h>
include <linux/netdevice.h>
include <linux/etherdevice.h>
include <linux/ethtool.h>
*/


#undef FAIL_ON_CHECKSUM_ERR	/* fail to configure SFP if checksum bad */
#define PORT_POWER_CONTROL	/* ports can be enabled/disabled via sysfs */
#define PORT_MODE_CONTROL	/* ports 5/6 can have SFP/RJ45 mode forced */

MODULE_DESCRIPTION("TJA1100 driver");
MODULE_AUTHOR("Johannes Ilg <johannes.ilg@continental-corporation.com>");
MODULE_LICENSE("GPL");


//  0x0180  dc60
#define MV_IDENT_MASK		0x0ffffff0
//#define MV_IDENT_MASK		0x00000000
#define MV_IDENT_VALUE		0x0180dc60

#define MII_TJA1100_IR		17	/* Interrupt Status/Control Register */
#define MII_TJA1100_IR_EN_LINK	0x0400	/* IR enable Linkstate */
#define MII_TJA1100_IR_EN_ANEG	0x0100	/* IR enable Aneg Complete */
#define MII_TJA1100_IR_IMASK_INIT	(MII_TJA1100_IR_EN_LINK | MII_TJA1100_IR_EN_ANEG)



static int tja1100_config_init(struct phy_device *pdev)
{
	dev_info(&pdev->dev, "%s\n", __func__);

	pdev->state = PHY_RUNNING;
	printk(KERN_INFO "Registering TJA\n");
	return 0;
}


#define TJA1100_BASIC_STATUS_REG	(0x01)
#define TJA1100_BASIC_CONTROL_REG	(0x00)
#define TJA1100_SPEED_MASK			(0x0200)
#define TJA1100_SPEED_100M			(0x0200)
#define TJA1100_TX_FIFO_MASK		(0x3000)
#define TJA1100_TX_FIFO_DEPTH_8		(0x0000)
#define TJA1100_TX_FIFO_DEPTH_16	(0x1000)
#define TJA1100_INTERFACE_MASK		(0x0007)
#define TJA1100_GMII_INTERFACE		(0x0002)
#define TJA1100_SYS_CLK_EN		(0x01 << 4)


static int tja1100_read_status(struct phy_device *pdev)
{

	int ret;
	u32 val;
	static int speed;
	ret=genphy_read_status(pdev);
	pdev->speed = SPEED_100;
	pdev->duplex = DUPLEX_FULL;
	dev_dbg(&pdev->dev, "TJA1100 read_status  %s:%d--%d", __func__,ret, pdev->speed);
	if (speed != pdev->speed) {
		speed = pdev->speed;
		val = phy_read(pdev, TJA1100_BASIC_CONTROL_REG);
		if ((val & TJA1100_SPEED_MASK) ==
					TJA1100_SPEED_100M) {
			dev_info(&pdev->dev, "%s:%d", __func__,val);
			val |= TJA1100_SPEED_100M;
			phy_write(pdev, TJA1100_BASIC_CONTROL_REG, val);
		}
	}
	return ret;
}



static int tja1100_config_aneg(struct phy_device *pdev)
{
	int ret=0;
	printk(KERN_INFO "TJA1100 config_aneg \n");
	// int ret=genphy_config_aneg(pdev);
	dev_info(&pdev->dev, "%s", __func__);
	return ret;
}


static struct phy_driver tja1100_phy_driver = {
	.name		= "TJA1100",
	.phy_id		= MV_IDENT_VALUE,
	.phy_id_mask	= MV_IDENT_MASK,
	.features	= SUPPORTED_Autoneg | SUPPORTED_TP  | SUPPORTED_MII | PHY_100BT_FEATURES,
	.flags		= PHY_POLL,
	.soft_reset     = &genphy_soft_reset,
	.config_init	= &tja1100_config_init,
	.config_aneg	= &genphy_config_aneg,
//	.config_aneg	= &tja1100_config_aneg,
//	.read_status	= &genphy_read_status,
	.read_status	= &tja1100_read_status,
	.suspend	= genphy_suspend,
	.resume		= genphy_resume,
	.driver		= { .owner = THIS_MODULE,},
};


static int __init tja1100_phy_init(void)
{
/*  UBOOT:
	struct iomuxc *iomuxc_regs = (struct iomuxc *)IOMUXC_BASE_ADDR;
	struct anatop_regs *anatop = (struct anatop_regs *)ANATOP_BASE_ADDR;
	int reg;

	imx_iomux_v3_setup_multiple_pads(fec2_pads, ARRAY_SIZE(fec2_pads));

	// Use 125MHz anatop loopback REF_CLK1 for ENET1
	clrsetbits_le32(&iomuxc_regs->gpr[1], IOMUX_GPR1_FEC2_CLOCK_MUX2_SEL_MASK, 0);

	imx_iomux_v3_setup_multiple_pads(phy2_control_pads,
					 ARRAY_SIZE(phy2_control_pads));

	// Enable the ENET power, active low
	gpio_direction_output(IMX_GPIO_NR(2, 6) , 0);

	// Reset TJA1100 PHY
	gpio_direction_output(IMX_GPIO_NR(5, 15) , 0);
	udelay(500);
	gpio_set_value(IMX_GPIO_NR(5, 14), 1);

	reg = readl(&anatop->pll_enet);
	reg |= BM_ANADIG_PLL_ENET_REF_25M_ENABLE;
	writel(reg, &anatop->pll_enet);

	//return enable_fec_anatop_clock(ENET_125MHZ);
	//int enable_fec_anatop_clock(enum enet_freq freq)

	u32 reg = 0;
	s32 timeout = 100000;

	struct anatop_regs __iomem *anatop =
		(struct anatop_regs __iomem *)ANATOP_BASE_ADDR;

	if (freq < ENET_25MHZ || freq > ENET_125MHZ)
		return -EINVAL;

	reg = readl(&anatop->pll_enet);
	reg &= ~BM_ANADIG_PLL_ENET_DIV_SELECT;
	reg |= freq;

	if ((reg & BM_ANADIG_PLL_ENET_POWERDOWN) ||
	    (!(reg & BM_ANADIG_PLL_ENET_LOCK))) {
		reg &= ~BM_ANADIG_PLL_ENET_POWERDOWN;
		writel(reg, &anatop->pll_enet);
		while (timeout--) {
			if (readl(&anatop->pll_enet) & BM_ANADIG_PLL_ENET_LOCK)
				break;
		}
		if (timeout < 0)
			return -ETIMEDOUT;
	}

	// Enable FEC clock
	reg |= BM_ANADIG_PLL_ENET_ENABLE;
	reg &= ~BM_ANADIG_PLL_ENET_BYPASS;
	writel(reg, &anatop->pll_enet);

	// Set enet ahb clock to 200MHz, pll2_pfd2_396m-> ENET_PODF-> ENET_AHB
	reg = readl(&imx_ccm->chsccdr);
	reg &= ~(MXC_CCM_CHSCCDR_ENET_PRE_CLK_SEL_MASK
		 | MXC_CCM_CHSCCDR_ENET_PODF_MASK
		 | MXC_CCM_CHSCCDR_ENET_CLK_SEL_MASK);
	// PLL2 PFD2
	reg |= (4 << MXC_CCM_CHSCCDR_ENET_PRE_CLK_SEL_OFFSET);
	// Div = 2
	reg |= (1 << MXC_CCM_CHSCCDR_ENET_PODF_OFFSET);
	reg |= (0 << MXC_CCM_CHSCCDR_ENET_CLK_SEL_OFFSET);
	writel(reg, &imx_ccm->chsccdr);

	// Enable enet system clock
	reg = readl(&imx_ccm->CCGR3);
	reg |= MXC_CCM_CCGR3_ENET_MASK;
	writel(reg, &imx_ccm->CCGR3);
	return 0;

*/



	int ret;
	printk(KERN_INFO "INIT TJA1100 Phy driver \n");
	ret=phy_driver_register(&tja1100_phy_driver);
	if (ret < 0) printk(KERN_INFO "ERROR INIT TJA1100 Phy driver \n");
	return ret;

}

static void __exit tja1100_phy_exit(void)
{
	phy_driver_unregister(&tja1100_phy_driver);
}

module_init(tja1100_phy_init);
module_exit(tja1100_phy_exit);

static struct mdio_device_id __maybe_unused tja1100_phy_tbl[] = {
	{ 0x0, 0x0ffffff0 },
	{ }
};

MODULE_DEVICE_TABLE(mdio, tja1100_phy_tbl);



