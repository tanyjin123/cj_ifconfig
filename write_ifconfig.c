/*set mac ,set ip ,set gateway
 */
//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
#include <unistd.h>
#include <ctype.h>

#include <sys/socket.h>

#include <sys/types.h>
//#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <net/if_arp.h>
#include <net/route.h>
#include <linux/if_vlan.h>
#include <linux/sockios.h>
#include <fcntl.h>
#include <errno.h>
#include "cjifconfig.h"

static int is_valid_ip(const char *ip)
{
    if(ip == NULL || strlen(ip) < 7) //0.0.0.0
        return -1;
    
    int len = strlen(ip);
    const char *start = ip;
    const char *end = ip + len -1;
    int ret = 0;
    int dot_count = 0;
    int cur_val = 0;
    int val = -1;

    //去掉空格
    while(end > start && isspace(*end))
    {
        --end;
    }
    while(start < end && isspace(*start))
    {
        ++start;
    }
    //排错
    if(start >= end || isdigit(*start) == 0 || isdigit(*end) == 0)
        return -1;

    //正式开始判断
    while(start <= end)
    {
        while(start <= end && '0' <= *start && *start <= '9')
        {
            cur_val = 10 * cur_val + (*start - '0');
            if(cur_val < 0 || cur_val > 255)
            {
                ret = -1;
                break;
            }
            val = cur_val;
            ++start;
        }
        if(start <= end)
        {
            if(*start == '.' && val != -1)
            {
                ++start;
                ++dot_count;
                cur_val = 0;
                val = -1;
            }
            else
            {
                ret = -1;
                break;
            }
        }
        else
        {
            break;
        }
    }
    if(ret == 0 && dot_count !=3)
    {
        ret = -1;
    }
    return ret;
}
static int is_valid_mac(const char *mac)
{
    if(mac == NULL || strlen(mac) < 17) //6*2 + 5 +1 = 18
    {
        return -1;
    }

    int len = strlen(mac);
    int ret = 0;
    int num_count = 0;
    int colon_count = 0;
    const char *start = mac;
    const char *end = mac + len -1;

    while (start < end && isspace(*start))
    {
        ++start;
    }
    while( end > start && isspace(*end))
    {
        --end;
    }
    while((end - start) != 16 || isxdigit(*start) == 0 || isxdigit(*end) == 0)
    {
        return -1;
    }
    while(start <= end)
    {
        while(start <= end && isxdigit(*start))
        {
            ++start;
            if (++num_count > 2)
            {
                ret = -1;
                break;
            }
        }
        if(start <= end)
        {
            if (*start == ':' && num_count == 2)
            {
                ++colon_count;
                num_count = 0;
                ++start;
            }
            else
            {
                ret = -1;
                break;
            }
        }
        else 
        {
            break;
        }
    }
    if (ret == 0 && colon_count != 5)
    {
        ret = -1;
    }
    return ret;
}

static int set_ip(int sock, struct ifreq *ifr , struct sockaddr *sa)
{
    if(sock < 0 || ifr == NULL || sa == NULL)
    {
        return -1;
    }
    int ret = 0;
    memcpy((char *)&ifr->ifr_addr,(char *)sa , sizeof(struct sockaddr));

    if (ioctl(sock,SIOCSIFADDR,ifr) < 0)
    {
        fprintf(stderr, "SIOCSIFADDR error: %s\n", strerror(errno));
        ret = -1;
    }
    return ret;
}

static int set_netmask(int sock, struct ifreq *ifr, struct sockaddr *sa)
{
    if (sock < 0 || ifr == NULL || sa == NULL)
    {
        return -1;
    }

    int ret = 0;
    memcpy((char *)&ifr->ifr_netmask,(char *)sa,sizeof(struct sockaddr));

    if (ioctl(sock, SIOCSIFNETMASK,ifr) < 0)
    {
        ret = -1;
    }
    return ret;
}

static int set_gateway(int sock, struct ifreq *ifr, struct rtentry *rt)
{
    if (sock < 0 || ifr == NULL || rt == NULL)
    {
        return -1;
    }

    int ret = 0;
    if (ioctl(sock , SIOCADDRT,rt) < 0)
    {
        ret = -1;
    }
    return ret;
}

int ifconfig(const char *if_name, const char *op, char *argv)
{
    if (if_name == NULL || op == NULL || argv == NULL)
    {
        printf("if_name or arguments could not be NULL!\n");
        return -1;
    }

    struct sockaddr_in sin;
    char host[128];
    struct ifreq ifr;
    int sock;
    int ret;

    snprintf(ifr.ifr_name, IFNAMSIZ, "%s", if_name);
    ifr.ifr_hwaddr.sa_family = ARPHRD_ETHER;

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0)
    {
        return -1;
    }

    if(strcasecmp(op, "ip") == 0)
    {
        if(is_valid_ip(argv) != 0)
        {
            close(sock);
            return -1;
        }
        inet_pton(AF_INET,argv, &sin.sin_addr.s_addr);
        sin.sin_family = AF_INET;
        ret = set_ip(sock,&ifr,(struct sockaddr*)&sin);
    }
    else if (strcasecmp(op , "netmask") == 0)
    {
        if(is_valid_ip(argv) != 0)
        {
            close(sock);
            return -1;
        }
        inet_pton(AF_INET,argv,&sin.sin_addr.s_addr);
        sin.sin_family = AF_INET;
        ret = set_netmask(sock, &ifr,(struct sockaddr *)&sin);
    }
    else if(strcasecmp(op,"gateway") == 0 || strcasecmp(op,"gw") == 0)
    {
        if(is_valid_ip(argv) != 0)
        {
            close(sock);
            return -1;
        }
        struct rtentry rt;
        memset(&rt, 0, sizeof(struct rtentry));

        inet_pton(AF_INET,argv,&sin.sin_addr.s_addr);
        sin.sin_family = AF_INET;
        sin.sin_port = 0;

        memcpy(&rt.rt_gateway,&sin, sizeof(struct sockaddr_in));
        ((struct sockaddr_in *)&rt.rt_dst)->sin_family = AF_INET;
        ((struct sockaddr_in *)&rt.rt_genmask)->sin_family = AF_INET;
        rt.rt_flags = RTF_GATEWAY;
        ret = set_gateway(sock,&ifr,&rt);
     }
    else
    {
        printf("Not supported temporarily!\n");
    }
    close(sock);
    return ret;
}



