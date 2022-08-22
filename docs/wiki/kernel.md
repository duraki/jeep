## Kernel

### Hotfixing old Kernel version with newest Kernel Headers

Since `raspberrypi-kernel-headers` installs latest available Raspberry OS kernel headers. Note that you can always [compile and build Linux Kernel for Raspberry Pi manually](https://www.raspberrypi.com/documentation/computers/linux_kernel.html#building). We need to symlink those to our current Kernel version (usually older, if not `apt upgrade`):

```
# => current kernel version
pi@raspberrypi:~/jeep/drivers/vxcan $ uname -r
5.15.32-v7+

# => list all installed kernel header files
pi@raspberrypi:/lib/modules $ ls -la
...
drwxr-xr-x  4 root root 4096 kol  22 02:07 5.15.32-v7+    # => our current kernel ver.
drwxr-xr-x  2 root root 4096 kol  22 01:56 5.15.56-v7+    # => latest available kernel ver. (via apt)

# => we need to softlink the latest available kernel header build/ dir to our kernel version
# => this symlinks "5.15.56-v7l+/build/" directory to our current kernel version "5.15.32-v7+/build"
pi@raspberrypi:/lib/modules $ sudo ln -s /lib/modules/5.15.56-v7l+/build/ /lib/modules/5.15.32-v7+/build

# => check if everything is correct
pi@raspberrypi:/usr/lib/modules/5.15.32-v7+ $ ls -la | grep build
lrwxrwxrwx  1 root root     32 kol  22 02:25 build -> /lib/modules/5.15.56-v7l+/build/

# => copy a custom Module.symvers for our Kernel version 
pi@raspberrypi:~ $ cp /lib/modules/5.15.32-v7+/build/Module.symvers /home/pi/jeep/drivers/vxcan/Module.custom.symvers
```

**Continue building `vxcan` kernel module**

With-in your raspberry-pi $TERM shell, continue building included `vxcan` Kernel module:

```
pi@raspberrypi:~/jeep/drivers/vxcan $ make all
make -C /lib/modules/5.15.32-v7+/build M=/home/pi/jeep/drivers/vxcan modules
make[1]: ... „/usr/src/linux-headers-5.15.56-v7l+“
  CC [M]  /home/pi/jeep/drivers/vxcan/vxcan.o
  MODPOST /home/pi/jeep/drivers/vxcan/Module.symvers
  CC [M]  /home/pi/jeep/drivers/vxcan/vxcan.mod.o
  LD [M]  /home/pi/jeep/drivers/vxcan/vxcan.ko
make[1]: ... „/usr/src/linux-headers-5.15.56-v7l+“

pi@raspberrypi:~/jeep/drivers/vxcan $ sudo chown root:root vxcan.ko
pi@raspberrypi:~/jeep/drivers/vxcan $ sudo chmod 0644 vxcan.ko
pi@raspberrypi:~/jeep/drivers/vxcan $ sudo cp vxcan.ko /lib/modules/5.15.32-v7+/kernel/net/can/

# => use dmesg with the following command to exec can/vxcan
# $ dmesg -w
# => [ 6157.400706] can: controller area network core
# => [ 6157.400823] NET: Registered PF_CAN protocol family
# => [ 6157.401725] can: netlink gateway - max_hops=1 

pi@raspberrypi:~/jeep/drivers/vxcan $ sudo depmod -A

pi@raspberrypi:~/jeep/drivers/vxcan $ lsmod 
Module                  Size  Used by
can_gw                 24576  0
can                    28672  1 can_gw
vxcan                  ...

# use `modinfo vxcan` and `modinfo -F vermagic vxcan` to display module details
# the vermagic should match the Kernel we are building for.

pi@raspberrypi:~/jeep/drivers/vxcan $ file /lib/modules/5.15.32-v7+/kernel/net/can/vxcan.ko
/lib/modules/5.15.32-v7+/kernel/net/can/vxcan.ko: ELF 32-bit LSB relocatable, ARM, EABI5 version 1 (SYSV)ž
# => ... BuildID[sha1]=1e5c9b9bd53666432836670e912afce345b8cb63, not stripped ...

pi@raspberrypi:/lib/modules/5.15.32-v7+/kernel/net/can $ modprobe --dump-modversions vxcan.ko
0x85ccbf3a	module_layout
0x59e1530e	ethtool_op_get_ts_info
0xc545c45c	rtnl_link_unregister
...
```

It is important to keep all `module_layout` of a vxcan module, as per default Kernel layout:

```
pi@raspberrypi:~ $ modprobe --dump-modversions /lib/modules/5.15.32-v7+/kernel/net/can/can.ko | grep module_layout
0x7c30cdd7	module_layout  # => correct
pi@raspberrypi:~ $ modprobe --dump-modversions /lib/modules/5.15.32-v7+/kernel/net/can/can-gw.ko | grep module_layout
0x7c30cdd7	module_layout  # => correct
pi@raspberrypi:~ $ modprobe --dump-modversions /lib/modules/5.15.32-v7+/kernel/net/can/vxcan.ko | grep module_layout
0x85ccbf3a	module_layout  # => incorrect
```

If the module keeps setting different module_layout, you can use the following oneliner to hotpatch directly in the vxcan kernel object:

```
Table:
  Correct MODULE_LAYOUT
    0X7C30CDD7 => (LE) D7 CD 30 7C

  Incorrect MODULE_LAYOUT
    0x85CCBF3A => (LE) 3A BF CC 85

  * - ... (where LE => little-endian) ...
```

**Replace bytes in Kernel Module**

```
# => ... optionally, make a vxcan.ko copy (cp vxcan.ko vxcan.ko.bak) ...

$ hexdump -C /lib/modules/5.15.32-v7+/kernel/net/can/vxcan.ko | grep --ignore-case module_layou -C2 
# => incorrect module_layout
00000c30  cc 00 00 00 00 01 00 00  30 01 00 00 40 03 00 00  |........0...@...|
00000c40  94 03 00 00 70 04 00 00  10 00 00 00 3a bf cc 85  |....p.......:...|
00000c50  6d 6f 64 75 6c 65 5f 6c  61 79 6f 75 74 00 00 00  |module_layout...|

# replacing incorrect module_layout with the correct one 
$ sed 's/\x3A\xBF\xCC\x85/\xD7\xCD\x30\x7C/g' vxcan.ko > temp; mv temp vxcan.ko

# confirming the change in the binary bytes
$ hexdump -C vxcan.ko | grep --ignore-case "d7 cd 30 7c"
00000c40  94 03 00 00 70 04 00 00  10 00 00 00 d7 cd 30 7c  |....p.........0||
```

**Removing vxcan kernel module**

```
$ rmmod squashfs.ko
$ rm -rf /lib/modules/<version>/kernel/net/can/vxcan.ko
```

### Building Kernel from Source

This part is required if you are using a hardware gateway interface, such is SCB (Raspberry Pi + Raspbian OS).

```
# => install prerequisits
$ sudo apt install git bc bison flex libssl-dev make

# => get the sources
$ git clone --depth=1 https://github.com/raspberrypi/linux
$ cd linux/

# => build config
$ KERNEL=kernel7 # => for RPI 2 & 3(+)
$ make bcm2709_defconfig
  # ...
  # HOSTCC  scripts/kconfig/util.o
  # HOSTLD  scripts/kconfig/conf
  #
  # configuration written to .config

# => customising LOCALVERSION (not required)
$ cat .config | grep LOCALVERSION
  # CONFIG_LOCALVERSION="-v7"
```

Build 32bit Kernel Version:

```
$ make -j4 zImage modules dtbs
...

$ sudo make modules_install
...

$ sudo cp arch/arm/boot/dts/*.dtb /boot/
$ sudo cp arch/arm/boot/dts/overlays/*.dtb* /boot/overlays/
$ sudo cp arch/arm/boot/dts/overlays/README /boot/overlays/
$ sudo cp arch/arm/boot/zImage /boot/$KERNEL.img
```

### Using Kernel Version from Source

If you want Kernel Headers for specific version, use the instructions from **Building Kernel from Source** to download Kernel Header files, and then use the following to build custom modules based on your preferred Kernel Version choice.

```
$ uname -r
5.15.32-v7+

$ git clone ....
$ cd linux/
$ git checkout rpi-5.15.y

```
