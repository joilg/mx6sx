SUBTARGET:=6sx
BOARDNAME:=Freescale i.MX 6 SoloX based Boards
CPU_TYPE:=cortex-a9
CPU_SUBTYPE:=neon
FEATURES:=audio display fpu gpio pcie rtc usb usbgadget squashfs targz nand ubifs

KERNELNAME:=zImage dtbs

define Target/Description
	Freescale i.Mx6 SoloX
endef

