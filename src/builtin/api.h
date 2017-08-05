/**
 *
 * Main API lib from `jeep` system, allows you to easily communicate with
 * CAN interface and other COM/searial devices. Inter-communication with CAN
 * frame is also possible.
 *
 * All builtin modules are written and documented under this particular API.
 * Go and explore how car is made.
 *
 * api.h
 *
 * Copyright (c) 2017 Halis Duraki. All rights reserved.
 *
 * Author:
 * Halis Duraki <duraki.halis@nsoft.com> 
 *
 **/

#ifndef JEEP_API
#define JEEP_API 

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <ctype.h>
#include <libgen.h>
#include <time.h>
#include <errno.h>

#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/uio.h>
#include <linux/if.h>

#include <linux/if_packet.h>
#include <linux/if_ether.h>
#include <linux/can.h>
#include <linux/can/raw.h>

#include <arpa/inet.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <linux/if_link.h>

#define MAXIFACE 30     /* max. number of all interfaces */
#define MAXSOCK 16      /* max. number of CAN interfaces */
#define MAXIFNAMES 30   /* size of receive name index */
#define ANYDEV "*"      /* name of interface to receive from any CAN interface */
#define ANL "\r\n"      /* newline in ASC mode */

#define SILENT_INI 42 /* detect user setting on commandline */
#define SILENT_OFF 0  /* no silent mode */
#define SILENT_ANI 1  /* silent mode with animation */
#define SILENT_ON  2  /* silent mode (completely silent) */

const int canfd_on = 1;

struct interface { /* defines a single interface struct */
    int  id;            /* interface id */
    char *ifname[50];   /* interface name */
    int  r_bytes;       /* rec bytes */
    int  r_packets;     /* rec packets */
    int  t_bytes;       /* trans bytes */
    int  t_packets;     /* trans packets */
};

struct device_i { /* defines a list of interfaces */
    int id;
    struct interface ifr;
};

struct device_i ldevice[MAXIFACE]; 

/**
 * Get all CAN interfaces
 *
 * This was two-way method to detect `if` using getifaddrs but I though about 
 * giving it a * try to reuse `/proc/net/dev`. At least for a test support 
 * one could bring virtual device and work in ascii.
 */
struct device_i *
get_interface_list() 
{
    FILE *fp=fopen("/proc/net/dev", "r");
    char buf[200], ifname[20];
    unsigned long int r_bytes, t_bytes, r_packets, t_packets;

	/* ignore table head */
    for (int i = 0; i < 2; i++) {
        fgets(buf, 200, fp);
    }

    int ifn = 0;
    while (fgets(buf, 200, fp)) {
        ifn++;

        /* extract from ep */
        sscanf(buf, "%[^:]: %lu %lu %*lu %*lu %*lu %*lu %*lu %*lu %lu %lu",
                    ifname, &r_bytes, &r_packets, &t_bytes, &t_packets);

        struct interface iface;
        strcpy(iface.ifname, ifname);
        iface.id = ifn;
        iface.r_bytes = r_bytes;
        iface.r_packets = r_packets;
        iface.t_bytes = t_bytes;
        iface.t_packets = t_packets;

        /* fillset */
        ldevice[ifn].id = ifn;
        ldevice[ifn].ifr = iface;
    }

    fclose(fp); /* close fh */
	return ldevice;

}

/*
 * Create socket.
 *
 * Initialize a new raw socket with for communication. Initialize any device
 * with creating socket before anything else.
 */
int
create_socket()
{
    int s; /* socket idx */
    s = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_CAN)); /* create socket */

    return s;
}

int
bind_socket(int irf, int socket, struct sockaddr_ll sl)
{
	sl.sll_family = AF_PACKET;
    sl.sll_ifindex = irf;
    sl.sll_protocol = htons(ETH_P_CAN);

    if (bind(&socket, (struct sockaddr *) &sl, sizeof(sl)) < 0) {
        perror("bind"); 
        return -1;
    }

    return 0;

}

#endif
