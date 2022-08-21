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

