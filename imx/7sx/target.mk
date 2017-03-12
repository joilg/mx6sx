SUBTARGET:=7sx
BOARDNAME:=Freescale i.MX 7 SoloX based Boards
CPU_TYPE:=cortex-a9
CPU_SUBTYPE:=neon
FEATURES:=audio display fpu gpio pcie rtc usb usbgadget squashfs targz nand ubifs

KERNELNAME:=zImage dtbs

define Target/Description
	Freescale i.Mx7 SoloX
endef

