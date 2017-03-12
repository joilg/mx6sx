#
# Copyright (C) 2013 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/image.mk

FAT32_BLOCK_SIZE=1024
FAT32_BLOCKS=$(shell echo $$(($(CONFIG_TARGET_BOOTFS_PARTSIZE)*1024*1024/$(FAT32_BLOCK_SIZE))))

define Build/boot-scr
	rm -f $@.bootscript
	mkimage -A arm -O linux -T script -C none -a 0 -e 0 -d ../$(SUBTARGET)/boot.script $@.bootscript
endef

define Build/SDCard-vfat-ext4
	# Build/SDCard-vfat-ext4 $(1),$(2) >  $@ 
	rm -f $(KDIR)/boot.img
	mkfs.fat $(KDIR)/boot.img -C $(FAT32_BLOCKS)

	mcopy -i $(KDIR)/boot.img $(DTS_DIR)/$(3).dtb ::$(3).dtb
	mcopy -i $(KDIR)/boot.img $(BIN_DIR)/$(IMG_PREFIX)-uImage ::uImage

	./gen_sdcard_vfat_ext4.sh \
		$(BIN_DIR)/$(2) \
		$(BIN_DIR)/uboot-mxs-$(4)/uboot-mxs-$(4).sb \
		$(KDIR)/boot.img \
		$(KDIR)/root.$(1) \
		$(CONFIG_TARGET_BOOTFS_PARTSIZE) \
		$(CONFIG_TARGET_ROOTFS_PARTSIZE)
	$(call Image/Gzip,$(BIN_DIR)/$(2))
endef

define Build/SDCard-ext4-ext4
	# Build/SDCard-ext4-ext4 $(1),$(2) >  $@
	./gen_sdcard_ext4_ext4.sh \
		$(BIN_DIR)/$(2) \
		$(BIN_DIR)/uboot-mxs-$(4)/uboot-mxs-$(4).sb \
		$(KDIR)/root.$(1) \
		$(CONFIG_TARGET_ROOTFS_PARTSIZE)
	$(call Image/Gzip,$(BIN_DIR)/$(2))
endef

define Build/boot-img
	# Build/boot-img $(1) > $@ 
	# MBR:           1M = 2048 sectors
	# Partition 1:   $(CONFIG_TARGET_BOOTFS_PARTSIZE)
	rm -f $@.boot
	mkfs.fat $@.boot -C $(FAT32_BLOCKS)
	$(foreach dts,$(DEVICE_DTS), mcopy -i $@.boot $(DTS_DIR)/$(dts).dtb ::$(dts).dtb)
	mcopy -i $@.boot $(IMAGE_KERNEL) ::zImage
	mcopy -i $@.boot $@.bootscript ::boot.scr
endef

define Build/sdcard-img
	# Build/sdcard-img u-boot=$(1)  $(2) $(3) 
	# Partition 1:   $(CONFIG_TARGET_BOOTFS_PARTSIZE)
	# Partition 2:   $(CONFIG_TARGET_ROOTFS_PARTSIZE)
	ROOTFS_SIZE=$$(( $(CONFIG_TARGET_ROOTFS_PARTSIZE) * 1024 * 2 )); \
	./gen_imx_sdcard_img.sh $@ \
		"$(BIN_DIR)/uboot-imx-imx-$(1)/$(1)-u-boot.imx" \
		c $(FAT32_BLOCKS) $@.boot \
		83 $$ROOTFS_SIZE $(IMAGE_ROOTFS)
endef

define Build/bootfs.tar.gz
	# Build/bootfs.tar.gz $(1) > $@ 
	rm -rf $@.boot
	mkdir -p $@.boot
	$(TAR) -C $@.boot -xf $(IMAGE_KERNEL).boot.tar
	$(TAR) -C $@.boot \
		--numeric-owner --owner=0 --group=0 --transform "s,./,./boot/," \
		-czvf $@ .
endef

define Build/boot-overlay
	# Build/boot-overlay $(1)=Eraseblocks $(2) > $@
	rm -rf $@.boot
	mkdir -p $@.boot
	$(CP) $@ $@.boot/$(IMG_PREFIX)-uImage
	ln -sf $(IMG_PREFIX)-uImage $@.boot/uImage
	$(CP) $@ $@.boot/$(IMG_PREFIX)-zImage
	ln -sf $(IMG_PREFIX)-zImage $@.boot/zImage
	$(TAR) -C $@.boot -cf $@.boot.tar .
endef

define Device/Default
  PROFILES := Generic
  FILESYSTEMS := squashfs ext4
  KERNEL_INSTALL := 1
  KERNEL_SUFFIX := -uImage
  KERNEL_NAME := zImage
  KERNEL_PREFIX := $$(IMAGE_PREFIX)
  KERNEL := kernel-bin | uImage none
  IMAGE_NAME = $$(IMAGE_PREFIX)-$$(1).$$(2)
  IMAGES :=
endef


include ../$(SUBTARGET)/image.mk
# $(foreach v, $(.VARIABLES), $(info $(v) = $($(v))))

$(eval $(call BuildImage))
