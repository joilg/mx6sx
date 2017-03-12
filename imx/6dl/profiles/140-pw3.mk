
define Profile/PW3
  NAME:=Kindle PW3
  PACKAGES:= \
	kmod-thermal-imx kmod-usb-chipidea-imx kmod-usb-mxs-phy \
	kmod-leds-gpio kmod-wdt-imx2 uboot-envtools \
	kmod-qcacld-la \
	car2car
endef

define Profile/PW3/Description

endef

PW3_DTS:= \
	imx6sl-pw3 

$(eval $(call Profile,PW3))
