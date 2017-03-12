KERNEL_LOADADDR=0x10008000

define Build/boot-scr
	rm -f $@.bootscript
	mkimage -A arm -O linux -T script -C none -a 0 -e 0 -d ../$(SUBTARGET)/boot.script $@.bootscript
endef

# SD-Card Images:
# these values are optimized for a 4GB labeled sdcard that actually holds 7744512 sectors of 512 byte
# MBR:            2048 sectors
# Partition 1:   32768 sectors
# Partition 2:   98304 sectors (configurable)

define Image/Build/SDCard-vfat-ext4
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

define Image/Build/SDCard-ext4-ext4
	./gen_sdcard_ext4_ext4.sh \
		$(BIN_DIR)/$(2) \
		$(BIN_DIR)/uboot-mxs-$(4)/uboot-mxs-$(4).sb \
		$(KDIR)/root.$(1) \
		$(CONFIG_TARGET_ROOTFS_PARTSIZE)
	$(call Image/Gzip,$(BIN_DIR)/$(2))
endef

define Build/boot-img
	rm -f $@.boot
	mkfs.fat $@.boot -C 16384
	$(foreach dts,$(DEVICE_DTS), mcopy -i $@.boot $(DTS_DIR)/$(dts).dtb ::$(dts).dtb)
	mcopy -i $@.boot $(IMAGE_KERNEL) ::zImage
	mcopy -i $@.boot $@.bootscript ::boot.scr
endef

define Build/sdcard-img
	ROOTFS_SIZE=$$(( $(CONFIG_TARGET_ROOTFS_PARTSIZE) * 1024 * 2 )); \
	./gen_imx_sdcard_img.sh $@ \
		"$(BIN_DIR)/uboot-imx-imx-riotboard/riotboard-u-boot.imx" \
		c 32768 $@.boot \
		83 $$ROOTFS_SIZE $(IMAGE_ROOTFS)
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

