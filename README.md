## Jeep

<img src="https://pbs.twimg.com/media/DQJJRqyX0AA78lq.jpg:large" alt="sniff-mod" />

`jeep` is a Metasploit-like framework for CAN interfaces, mostly used in vehicle
and automobile systems. This tool should allow you to interact with your device
in Wireshark manner, implementing both sniffing and replying technique. There is
also an API header file available for reuse and faster module writings.

Code is documented in pseudo manner mostly reducted to Bosch standards. Whole
framework is written in C and is very unstable.

## Install

**For GNU/Linux Operating Systems** 

```
$ git clone https://github.com/duraki/jeep
$ cd jeep 
$ make all
```

**For XNU-based Operating Systems**

```
$ docker-compose up
$ docker exec -it jeep /bin/bash
```

**Build, and compile the program**

```
$ make all
```


## Hardware

To create a virtual net interface doing CANBus transport, we implemented an external hardware module using [vxcan](https://cateee.net/lkddb/web-lkddb/CAN_VXCAN.html). Pre-requisits for this project is an SBC such is Raspberry Pi (Tested) or LeMaker's SBC (Untested). Install Raspbian OS and set default configurations, such is SSH and VNC via `rasp-conf`.

Read more [on Linux Interfaces for Virtual Networking](https://developers.redhat.com/blog/2018/10/22/introduction-to-linux-interfaces-for-virtual-networking#). The default Docker network interface is sitting at `192.168.65.0/24` and using IPAM with `subnet: 127.17.0.0/16`/`gateway: 172.17.0.1`.

`vxcan` is a Linux kernel driver/module that can be used to set up a virtual CAN tunnel across network namespaces. For example it allows you to generate virtual CAN frames on your host and send them to a container; or forward real CAN traffic between a USB-CAN adapter and a container, without exposing the entire host network to the container.

The `vcan-interface` container is sitting at `172.17.0.2/16`. Docker automatically creates a subnet and gateway for the bridge network, and docker run automatically adds containers to it. Any containers on the same network may communicate with one another via IP addresses. You must connect containers with the `--link` option in your docker run command.

You can create an isolated bridge via:

```
$ docker network create --driver bridge isolated_bridge
# ... hash ...

$ docker network inspect isolated_bridge
# ... "Subnet": "172.20.0.0/16",
# ... "Gateway": "172.20.0.1"

$ docker network ls
NETWORK ID     NAME              DRIVER    SCOPE
00478aaa04a5   isolated_bridge   bridge    local
...
```

Then setup additional requirements, a `vcan` tunnel for cross namespace communication, and the driver:

```
pi@raspberrypi:~ $ sudo apt install raspberrypi-kernel-headers can-utils
pi@raspberrypi:~ $ sudo apt install git bc bison flex libssl-dev make
```

Clone this project on your Raspberry Pi disk (via `scp` or `rsync`), and do the following:

```
# => from your Host OS
$ scp -r dev/jeep pi@raspberrypi.local:/home/pi/jeep 

# => on your SCB
pi@raspberrypi:~ $ cd /home/pi/jeep/drivers/vxcan/
pi@raspberrypi:~ $ make clean && make all
  CC [M]  /home/pi/jeep/drivers/vxcan/vxcan.o
  ...

pi@raspberrypi:~ $ sudo chown root:root vxcan.ko 
sudo chmod 0644 vxcan.ko 
sudo mv vxcan.ko "/lib/modules/$(uname -r)/kernel/net/can/" 
sudo depmod -A 
sudo modprobe vxcan 
sudo modprobe can-gw
```

You should now see in `dmesg -w` that the vcan tunnel driver has been loaded:

```
$ dmesg -w
[  758.020199] vxcan: loading out-of-tree module taints kernel.
[  758.020640] vxcan: Virtual CAN Tunnel driver

$ modinfo vxcan # => displays kernel/so module info
```

**Setting up modules autoload**

Let's also add a file to /etc/modules-load.d so that the modules will load on startup. Create /etc/modules-load.d/can.conf and add the following:

```
# cat /etc/modules-load.d/can.conf 
vxcan
can-gw
```

**Smoke Testing of the hardware environment**

In a separate Terminal on a Host OS, start a container and install `canutils` within the container:

```
$ docker run --rm --net=isolated_bridge --net=host -it --name vcan-interface ubuntu:20.04 
# ... 20.04: Pulling from library/ubuntu
# ... 3b65ec22a9e9: Pull complete

root@8b60433051c0:/# apt-get update && apt-get install -y can-utils iproute2 -y
# ... 
```

Lets setup vxcan network and move one end of it, into the container's network namespace:

```
$ docker ps
CONTAINER ID   IMAGE          COMMAND   CREATED         STATUS         PORTS     NAMES
8b60433051c0   ubuntu:20.04   "bash"    2 minutes ago   Up 2 minutes             cantest
49de7daaded5   jeep_jeep      "bash"    8 hours ago     Up 7 seconds             jeep
```

**Removing vxcan kernel module**

```
$ rmmod squashfs.ko
$ rm -rf /lib/modules/<version>/kernel/net/can/vxcan.ko
```

## How

`jeep` works with CAN protcol whcih Bosch defined back in the old days. Using
`jeep` and your car, you may extract interesting info from it, or play with
various things, unlocking your car or even stealing someone elses! (I do not
advice this, I repeat, this is pro-bono educational purpose only).
  
I've talked about vehicle hacking and wrote some research papers on the same
topic which you may read or watch on YouTube. The first attraction `jeep` got is
in late 2015 when I had most time to research these stuff. It was all black on
white, written on paper and some latex docs. The first open-sourcing happened
sometimes ago when I wrote about in on [lobsters](lobste.rs).
  
> I continued working on jeep which is a some sort of WireShark sniffer but for vehicle/car systems (read: hacking tool for your car). It’s based on CAN, supposed to support fd (ext frame struct) from SocketCAN. The reason I’m writing this is because it’s fun and I need a can-utils which is extensible. Jeep should support extending through modules with built-in API. Project goal is to create system similar to Metasploit and stay opensource. This week I’m working on sniff module.

This software requires from you a UNIX-based Linux-powered operating system.
This was tested and developed in RedHat EL, while some features does not work
with CentOS derivant, they do with example Ubuntu. This is all up to you Kernel
and SocketCAN version.

**You are responsible if you destroy your car.**

## Status

This program is in very early phase. I'm currently working on API and `sniff`
module which should be built on top of the same API. Later on, I'll extend this
framework with newest modules.
  
If you are a C developer, be free to jump in and contribute with your lines,
your code is appreciated.

### Resources

* [Container Networking Plugins](https://github.com/containernetworking/plugins)
* [SocketCAN with Docker and Linux (pdf)](https://chemnitzer.linux-tage.de/2021/media/programm/folien/210.pdf)
