#include "cjifconfig.h"

 /*函数：main()
 * 检查程序的执行权限，检查命令行参数格式
 * 允许调用的格式：
 * ./cj_ifconfig eth0 -g: 输出ip，mac，bcast，netmask，mtu，metric
 * ./cj_ifconfig eth0 -g ip: 获得IP地址
 * ./cj_ifconfig eth0 -g mac: 获得mac地址
 * ./cj_ifconfig eth0 -g bcast: 
 * ./cj_ifconfig eth0 -g netmask:
 * ./cj_ifconfig eth0 -s ip "192.168.10.11"
 * ./cj_ifconfig eth0 -s netmask "255.255.255.0"
 * ./cj_ifconfig eth0 -s getaway "192.168.19.254"
 * ./cj_ifconfig -h
 */

int main(int argc, char * argv[])
{
    char *Devicename;
    char *op;
    char *param;
    if(argc < 2)
    {
        printf("Usage: ./cj_ifconfig [-h]/devicename [-g/s] [ip/mac/bcast....]");
        exit(-1);
    }
    if(strcmp("-h",argv[1]) == 0)
    {
        printf("Usage:\n");
        printf("      eth0 -g: get all the information\n");
        printf("      eth0 -g ip: get ip address\n");
        printf("      eth0 -g mac: get mac address\n");
        printf("      eth0 -g bcast: get broadcast address\n");
        printf("      eth0 -g netmask: get netmaskn");
        printf("      eth0 -s ip [ip address]: set ip address\n");
        printf("      eth0 -s netmask [netmask]: set  netmask\n");
        printf("      eth0 -s getaway [getaway]: set gateway\n");
    }
    
    Devicename = argv[1];
    op = argv[3];
    param = argv[4];
    if(strcmp("-s", argv[2]) == 0)
    {
        if(getuid() != 0)
        {
            printf("抱歉，执行该命令必须是root权限\n");
        }
        else
        {
            /*set param*/
            ifconfig(Devicename,op,param);
	}
    }
    else if(strcmp("-g", argv[2]) == 0)
    {
        if(argc == 3)
        {
            /*显示所有的*/
            show_all(Devicename);
        }
        else if(argc == 4)
        {
            /*显示指定参数*/
            show_ifconfig(Devicename,op);
        }
    }
    return 0;
}
