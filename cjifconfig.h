#ifndef CJIFCONFIG_H
#define CJIFCONFIG_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/ioctl.h>
#include<net/route.h>
#include<net/if.h>

static int get_mac(int sock, const char *device);
static int get_ip(int sock, const char *device);
static int get_bcast(int sock, const char *device);
static int get_mask(int sock, const char *device);
static int get_mtu(int sock, const char *device);
static int get_metric(int sock, const char *device);
int show_all(const char *if_name);
int show_ifconfig(const char *if_name, const char *op);

static int is_valid_ip(const char *ip);
static int is_valid_mac(const char *mac);
static int set_ip(int sock, struct ifreq *ifr , struct sockaddr *sa);
static int set_netmask(int sock, struct ifreq *ifr, struct sockaddr *sa);
static int set_gateway(int sock, struct ifreq *ifr, struct rtentry *rt);
int ifconfig(const char *if_name, const char *op, char *argv);

#endif
