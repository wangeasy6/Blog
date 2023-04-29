# VPN搭建
##### 使用 VPS 搭建一个 VPN 服务器

### CentOS & pptp
* 安装pptp
    yum -y update
    yum -y install ppp pptpd
* 添加转发功能
    vim /etc/sysctl.conf
    找到 net.ipv4.ip_forward = 0 改为 net.ipv4.ip_forward = 1
    sysctl -p 命令使其生效
* 设置 pptpd 分配给 客户端ip
    vim /etc/pptpd.conf
    localip ""
    remoteip 192.168.0.200-254
    * localip 是你机子的IP地址，remoteip 是即将分配给登录用户的虚拟ip地址
* 设置 DNS 地址
    vim /etc/ppp/options.pptpd
    ms-dns 8.8.8.8
    ms-dns 8.8.4.4
    * Google 的 DNS
* 设置账号和密码
    vim /etc/ppp/chap-secrets
    添加：
    test pptpd test *
    * 账号 协议 密码 分配的IP地址*表示任意IP

* 启动服务：
    首先是启动iptables
    service iptables start
    点开那个脚本添加转发规则不然上不了网（等学了iptables我再改成配置文件版本的
    ./snat.sh
    service pptpd start
