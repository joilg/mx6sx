#
# Copyright (C) 2013-2014 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
include $(TOPDIR)/rules.mk

ARCH:=arm
BOARD:=imx
BOARDNAME:=Qualcomm - Freescale i.MX Processor Family Devices
FEATURES:=audio display fpu gpio pcie rtc usb usbgadget squashfs targz nand ubifs
MAINTAINER:=Johannes Ilg <joilg@users.noreply.github.com>
CPU_TYPE:=cortex-a9
CPU_SUBTYPE:=neon

SUBTARGETS:=6q 6dl 6sx 7sx  8

KERNEL_PATCHVER:=4.9

define Target/Description
	Build firmware image for Qualcomm i.MXxx SoC devices.
	See <subtarget>/target.mk Description for Devices
endef

include $(INCLUDE_DIR)/target.mk

KERNELNAME:=zImage dtbs

DEFAULT_PACKAGES += uboot-envtools

$(eval $(call BuildTarget))
