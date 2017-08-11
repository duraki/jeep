FROM centos:centos7
MAINTAINER Halis Duraki <duraki@linuxmail.org>

ENV DEBIAN_FRONTEND noninteractive
RUN yum -y install git net-tools vim gcc gcc-c++ ncurses-devel make wget git iproute

#RUN modprobe vcan
#RUN ip link add dev vcan0 type vcan
#RUN ip link set up vcan0

# Set default command
CMD ["/usr/bin/bash"]
