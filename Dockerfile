FROM centos:centos7
MAINTAINER Halis Duraki <duraki@linuxmail.org>

ENV DEBIAN_FRONTEND noninteractive
RUN yum -y install git net-tools vim gcc gcc-c++ ncurses-devel make wget git

# Set default command
CMD ["/usr/bin/bash"]
