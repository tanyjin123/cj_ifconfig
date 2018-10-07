/* author:cj
 * time:2018.10.07
 * description:get_ip,get_mac,get_mask,get_gateway
 */
/*#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/ioctl.h>
*/
#include<netinet/if_ether.h>
#include<net/if.h>
#include<arpa/inet.h>

#include "cjifconfig.h"





/* name:get_mac
 * brief:获取网卡的mac地址
 * param：sock ---建立的套接字
 *        device ---网卡的名字
 * return：0 ---成功
 -1 ---失败
 */
static int get_mac(int sock, const char *device)
{
    if(sock < 0 || device == NULL)
    {
        return -1;
    }
    struct ifreq req;
    unsigned char mac_addr[ETH_ALEN];
    int i;
    
    strcpy(req.ifr_name,device);
    if(ioctl(sock,SIOCGIFHWADDR,&req) < 0)
    {
        printf("get mac addr failed\n");
        close(sock);
        return -1;
    }
    else
    {
        memcpy(mac_addr,req.ifr_hwaddr.sa_data,ETH_ALEN);
  //      close(sock);
    }
    printf("mac address is :");
    for(i = 0; i < ETH_ALEN-1; i++)
        printf("%2x:",mac_addr[i]);
    printf("%2x\n",mac_addr[ETH_ALEN-1]);
    return 0;
}

/* name:get_ip
 * brief：获取IP地址
 * param:sock ---建立连接的套接字
         device ---网卡的名字
 * return:0 ---成功
          -1 ---失败
 */
static int get_ip(int sock,const char *device)
{
    /*判断*/
    if(sock < 0 || device == NULL)
    {
        return -1;   //可以看看会不会有更好的写法
    }
    struct ifreq req;
    int i;
    char ip_addr[32];
    struct sockaddr_in *sin;

    strcpy(req.ifr_name,device);        //读取对应的接口
    if(ioctl(sock, SIOCGIFADDR, &req) < 0)  //
    {
        printf("get ip addr failed!\n");
        close(sock);
        return -1;
    }
    else
    {
        sin = (struct sockaddr_in *)&(req.ifr_addr);
        strcpy(ip_addr,inet_ntoa(sin->sin_addr));
        printf("ip address is : %s \n",ip_addr);
//        close(sock);
    }
    return 0;
}

/* name:get_bcast
 * brief：获取广播地址
 * param:sock ---建立连接的套接字
 device ---网卡的名字
 * return:0 ---成功
 -1 ---失败
 */
static int get_bcast(int sock,const char *device)
{
    /*判断*/
    if(sock < 0 || device == NULL)
    {
        return -1;   //可以看看会不会有更好的写法
    }
    struct ifreq req;
    int i;
    char broad_addr[32];
    struct sockaddr_in *sin;
    
    strcpy(req.ifr_name,device);        //读取对应的接口
    if(ioctl(sock, SIOCGIFBRDADDR, &req) < 0)  //
    {
        printf("get broadcast addr failed!\n");
        close(sock);
        return -1;
    }
    else
    {
        sin = (struct sockaddr_in *)&(req.ifr_broadaddr);
        strcpy(broad_addr,inet_ntoa(sin->sin_addr));
        printf("broadcast address is : %s \n",broad_addr);
    //    close(sock);
    }
    return 0;
}


/* name:get_mask
 * brief：获取IP地址
 * param:sock ---建立连接的套接字
         device ---网卡的名字
 * return:0 ---成功
          -1 ---失败
 */
static int get_mask(int sock,const char *device)
{
    /*判断*/
    if(sock < 0 || device == NULL)
    {
        return -1;   //可以看看会不会有更好的写法
    }
    struct ifreq req;
    int i;
    char ip_addr[32];
    struct sockaddr_in *sin;

    strcpy(req.ifr_name,device);        //读取对应的接口
    if(ioctl(sock, SIOCGIFNETMASK, &req) < 0)  //
    {
        printf("get ip addr failed!\n");
        close(sock);
        return -1;
    }
    else
    {
        sin = (struct sockaddr_in *)&(req.ifr_netmask);
        strcpy(ip_addr,inet_ntoa(sin->sin_addr));
        printf("ip address is : %s \n",ip_addr);
      //  close(sock);
    }
    return 0;
}

/* name:get_mtu
 * brief：获取mtu值
 * param:sock ---建立连接的套接字
 device ---网卡的名字
 * return:0 ---成功
 -1 ---失败
 */
static int get_mtu(int sock,const char *device)
{
    /*判断*/
    if(sock < 0 || device == NULL)
    {
        return -1;   //可以看看会不会有更好的写法
    }
    struct ifreq req;
    
    strcpy(req.ifr_name,device);        //读取对应的接口
    if(ioctl(sock, SIOCGIFMTU, &req) < 0)  //
    {
        printf("get mtu failed!\n");
        close(sock);
        return -1;
    }
    else
    {
        printf("mtu is : %d \n",req.ifr_mtu);
        //close(sock);
    }
    return 0;
}

/* name:get_metric
 * brief：获取mtu值
 * param:sock ---建立连接的套接字
 device ---网卡的名字
 * return:0 ---成功
 -1 ---失败
 */
static int get_metric(int sock,const char *device)
{
    /*判断*/
    if(sock < 0 || device == NULL)
    {
        return -1;   //可以看看会不会有更好的写法
    }
    struct ifreq req;
    
    strcpy(req.ifr_name,device);        //读取对应的接口
    if(ioctl(sock, SIOCGIFMETRIC, &req) < 0)  //
    {
        printf("get metric failed!\n");
        close(sock);
        return -1;
    }
    else
    {
        printf("mtu is : %d \n",req.ifr_metric);
        //close(sock);
    }
    return 0;
}

/**
 * name:show_all
 * brief:显示获得的所有数据
 * param:if_name: 网卡的名字
 * return: 0 --- 成功
         -1 --- 失败
 */
int show_all(const char *if_name)
{
    if(if_name == NULL )
    {
        printf("if_name or arguments could be NULL\n");
        return -1;
    }
    
    int sock;
    
    sock = socket(AF_INET,SOCK_DGRAM,0);
    if(sock < 0)
    {
        printf("socket failed\n");
        return -1;
    }

    get_mac(sock, if_name);
    get_ip(sock, if_name);
    get_bcast(sock,if_name);
    get_mask(sock,if_name);
    get_mtu(sock,if_name);
    get_metric(sock,if_name);

    close(sock);
    return 0;
}

/**
 * name:show_ifconfig
 * brief : 显示某个结果
 * param ： if_name ---网卡的名字
 *          op ---ip ,mac, broadcast ,netmask 之一
 * return: 0 ---成功
 *         -1 ---失败
 */
int show_ifconfig(const char *if_name, const char *op)
{
    if(if_name == NULL || op == NULL)
    {
        printf("if_name or arguments could not be null!\n");
        return -1;
    }

    int sock;
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if(sock < 0)
    {
        printf("socket failed!\n");
        return -1;
    }

    if(strcasecmp(op,"ip") == 0)
    {
        get_ip(sock,if_name);
	close(sock);

    }
    else if(strcasecmp(op,"mac") == 0)
    {
        get_mac(sock, if_name);
	 close(sock);
    }
    else if(strcasecmp(op,"netmask") == 0)
    {
        get_mask(sock,if_name);
	 close(sock);
    }
    else if(strcasecmp(op,"bcast") == 0)
    {
        get_bcast(sock,if_name);
	 close(sock);
    }
    else
    {
        printf("input ./cj_ifconfig -h to get more information\n");
    }
    return 0;
}



























