
define Profile/MFG-Tool
  NAME:=Freescale Manufacturing Tool requirements 
  PACKAGES:= \
	kmod-thermal-imx kmod-usb-chipidea-imx kmod-usb-mxs-phy \
	kmod-wdt-imx2 uboot-envtools \
    	imx-uuc \
    	util-linux \
    	coreutils \
    	dosfstools \
    	mmc-utils \
    	base-files \
    	base-passwd \
    	imx-kobs \
    	mtd-utils \
    	mtd-utils-ubifs \

endef

define Profile/MFG-Tool/Description
	Small image to be used with Manufacturing Tool 
	(mfg-tool) in a production environment.
endef

C2C_DTS:= \
	imx6sl-mfg

$(eval $(call Profile,MFG-Tool))
