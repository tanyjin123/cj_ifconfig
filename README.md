# cj_ifconfig

这是我第一次写程序</br>
参考了好多博客，最终实现了这个程序</br>
目的是在linux环境下，实现简单的ifconfig功能</br>
编译：</br>
&nbsp;&nbsp;&nbsp;&nbsp;gcc write_ifconfig.c read_ifconfig.c main.c -o cj_ifconfig </br>
运行</br>
&nbsp;&nbsp;&nbsp;&nbsp;./cj_ifconfig -h 获得帮助</br>
具体可以参考main.c文件</br>

还需要进一步改善的有：</br>
1 获得ipv6的地址</br>
2 实现获得所有网卡的这些信息</br>
3 在设置ip地址等信息时，现在是临时设置，未来争取能进一步的实现永久设置</br>
4 可以通过读取文件来设置这些信息 </br>

勤能补拙是良训，加油啊
