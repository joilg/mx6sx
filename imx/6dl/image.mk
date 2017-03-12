KERNEL_LOADADDR=0x10008000
KERNEL_ENTRY_POINT = 0x10008000

define Device/wandboard
  DEVICE_TITLE := Wandboard Dual
  DEVICE_DTS := imx6dl-wandboard
endef
TARGET_DEVICES += wandboard

define Device/riotboard
	DEVICE_TITLE := RioTBoard 
	DEVICE_DTS := imx6dl-riotboard
	DEVICE_PACKAGES := kmod-thermal-imx kmod-usb-chipidea-imx kmod-usb-mxs-phy uboot-imx-riotboard
	IMAGES := sdcard.img.gz
	IMAGE/sdcard.img.gz := boot-scr | boot-img | sdcard-img riotboard| gzip
	IMAGE_NAME = $$(IMAGE_PREFIX)-$$(2)
endef
TARGET_DEVICES += riotboard
