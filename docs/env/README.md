## Environment

The system is a container, transferring process from host device to cube. Uses a set of booted images in virtual machine & custom Linux Kernel it's possible to operate on CAN interface. This requires a sort of building custom image using [linuxkit/linuxkit](/linuxkit/linuxkit) and running it in isolated environment.

If you are on MacOS, you might need bits of code to compile the image. Docker for Mac (from Moby) is running virtual machine natively but their code is propriety. Docker for Mac is using MacOS supplied Hypervisor framework. 

```
$ file ~/Library/Containers/com.docker.docker/Data/com.docker.driver.amd64-linux/tty
com.docker.driver.amd64-linux/tty: character special (16/1)

$ ls ~/Library/Containers/com.docker.docker/Data/com.docker.driver.amd64-linux/tty
-> /dev/ttys001
```

Docker is installed on our machine. Obviously, we need an `iso` or bootable image, from which Docker boots containers. This is achieved using "small" (~1.4GB) Linux image running as host. It sounds dirty to run [MacOS Host]->[Linux Tunnel]->[Containers] but it's still the fastest way to share kernel and fs.


```
$ file ~/Applications/Docker.app/Contents/Resources/linuxkit/docker-for-mac.iso
docker-for-mac.iso: ISO 9660 CD-ROM filesystem data 'ISOIMAGE' (bootable)
```

Now we need LinuxKit, a tool to build small & secure isolated Linux OS. This will allow us to completely build our kernel image with specifically requested modules, in our case CAN. Since Linux Kernel v4.12, this feature implements namespaces meaning we can recompile the network system in container environment. This was not possible prior to Linux Kernel v4.12, we had to relay on tunneling through virtual machines (VM, etc.).

For official documentation, reffer to:

* [Compiling lk/linuxkit Kernel](https://github.com/linuxkit/linuxkit/blob/master/docs/kernels.md#compiling-external-kernel-modules)
* [Kernel config](kernel/modus.yml)

### Building custom kernel

If we are building new Linux Kernel to work with `jeep`, we may use kconfig from LinuxKit to create our custom configuration. This is required if we explicilty want to build newest kernel, if not you may skip this step.

```
# clone and make docker image for kconfig
$ cd /tmp && git clone https://github.com/linuxkit/linuxkit && cd /tmp/linuxkit/kernel
$ export KERNEL_VERSIONS="4.18.1" && make kconfig 
...

# creates new image
$ docker build --no-cache -f Dockerfile.kconfig --build-arg KERNEL_VERSI
ONS="4.18.1" -t linuxkit/kconfig .

$ docker run --rm -ti -v $(pwd):/src linuxkit/kconfig

# configure the new kernel by choice
$ cd linux-4.x/
$ make menuconfig
  ... configure the kernel 
$ cp .config /src/config-4.x-x86_64-custom
$ exit
``` 

### LinuxKit (custom)

You need to download application for building and the associated kernel version. Be careful if there are any changes inside other virtual machines.

```
# Download linuxkit through `brew` or `apt-get` 
$ brew install linuxkit
#    or 
$ go get -u github.com/linuxkit/linuxkit/src/cmd/linuxkit

# Download Linux Kernel version, the one we will compile
$ wget https://mirrors.edge.kernel.org/pub/linux/kernel/v4.x/linux-4.18.1.tar.xz


```

