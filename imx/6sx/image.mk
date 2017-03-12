KERNEL_LOADADDR=0x10008000
KERNEL_ENTRY_POINT = 0x10008000


define Device/sdb
	DEVICE_TITLE := IMX6SX-SDB-A 
	DEVICE_DTS := imx6sx-sdb
	DEVICE_PACKAGES := kmod-thermal-imx kmod-usb-chipidea-imx kmod-usb-mxs-phy uboot-imx-sdb
	IMAGES := sdcard.img.gz $$(2)
	IMAGE/sdcard.img.gz := boot-scr | boot-img | sdcard-img | gzip
	IMAGE_NAME = $$(IMAGE_PREFIX)-$$(2)
endef
TARGET_DEVICES += sdb

define Device/c2c
	DEVICE_TITLE := IAM-WLAN-BT
	KERNEL_LOADADDR=0x80008000
	KERNEL_ENTRY_POINT=0x80008000
	DEVICE_DTS := imx6sx-c2c
	SUPPORTED_DEVICES := $(1)
	KERNEL += | boot-overlay 
	DEVICE_PACKAGES := kmod-thermal-imx kmod-usb-chipidea-imx kmod-usb-mxs-phy uboot-imx-c2c
	IMAGES := qspi.ubi bootfs.tar.gz sdcard.img.gz 
	IMAGE/bootfs.tar.gz := bootfs.tar.gz | install-dtb
	IMAGE/qspi.ubi := append-ubi
	IMAGE/sdcard.img.gz := boot-scr | boot-img | sdcard-img mx6sxc2c $$(1) $$(2)  | gzip
	IMAGE_NAME = $$(IMAGE_PREFIX)-$$(2)
	PAGESIZE := 1
	BLOCKSIZE := 128k
	MKUBIFS_OPTS := -m $$(PAGESIZE) -e 262016
	UBINIZE_OPTS := -E 5
endef
TARGET_DEVICES += c2c
