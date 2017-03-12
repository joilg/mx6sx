# openwrt-imx-target
This target feed adds support for the i.MX6 platform to lede/openwrt. 

## Pre-Built Package Repository
Pre-compiled rootfs and packages are available. See the [Wiki](https://github.com/openwrt-zipit/openwrt-imx-target/wiki) for more information about installation and general usage.

## Prerequisites
See the [OpenWrt Wiki page](https://wiki.openwrt.org/doc/howto/buildroot.exigence) for host system prerequisites

## Usage
Download openwrt trunk with git and checkout commit 1e22c9b9eb691878156dfe32fb1e117737f1d248:

     git clone https://github.com/openwrt/openwrt.git openwrt-zipit
     cd openwrt-zipit
     git checkout 1e22c9b9eb691878156dfe32fb1e117737f1d248

Add the imx target feed to _feeds.conf_:

     echo "src-git imx_target https://github.com/joilg/mx6sx.git" > feeds.conf

Update and install the target feed:

     scripts/feeds update && scripts/feeds install -p imx_target imx

Copy the _feeds.conf_ and default config files:

     cp feeds/imx_target/feeds.conf ./feeds.conf
     cp feeds/imx_target/zipit_openwrt_defconfig ./.config

Update the feeds again:

     scripts/feeds update && scripts/feeds install -a

