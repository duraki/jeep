# jeep - docker image

# as of linux.4.12 we have can namespace available
# https://git.kernel.org/pub/scm/linux/kernel/git/stable/linux.git/log/net/can?h=linux-4.12.y  

# xxx: requires container namespace; linuxkit/linuxkit#3172
#      so not available yet (09-18)

FROM ubuntu:18.04
MAINTAINER Halis Duraki <duraki@linuxmail.org>

ENV DEBIAN_FRONTEND noninteractive
RUN apt-get -y update
RUN apt-get -y install bash build-essential git net-tools vim gcc ncurses-dev make wget git module-init-tools can-utils libx11-dev libc6-dev libncurses5-dev ia32-libs

#RUN modprobe vcan
#RUN ip link add dev vcan0 type vcan
#RUN ip link set up vcan0

# Set default command
# CMD ["sh"]
