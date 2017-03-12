# openwrt-imx-target
This target feed adds support for the i.MX6 platform to lede/openwrt. 


## Usage
Download openwrt trunk with git clone:

     git clone https://github.com/openwrt/openwrt.git

Add the imx target feed to feeds.conf_:

     echo "src-git imx_target https://github.com/joilg/mx6sx.git" >> feeds.conf

Update and install the target feed:

     scripts/feeds update && scripts/feeds install -p imx_target imx

