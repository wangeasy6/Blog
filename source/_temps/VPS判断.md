如何判断VPS是基于哪种虚拟技术？Xen、OpenVZ、Xen HVM、KVM还是VMware

2011年03月16日 上午 | 作者：VPS侦探

对于VPS新手来说，怕被无良的奸商给忽悠，下的Xen的却给的OpenVZ的，如何来判断自己买的VPS是那种虚拟技术的，下面VPS侦探整理一些常见的方法。

1、通过系统上的相关目录或文件判断

执行：ls /proc/ ，一般Xen的VPS，/proc目录下面会有xen的目录，openvz的会有vz目录。

2、执行：free -m 看内存，openvz的没有swap，当然也有xen的没有swap，但是xen的是可以加的，openvz不行。KVM是全虚拟化也是可以自己添加swap的。

3、执行：uname -a  有些xen的VPS里面会显示有xen。

4、执行：ifconfig 查看网卡，openvz的一般都是venet0:* ，xen、kvm的一般都是eth*。

5、通过VPS控制面板查看，像SolusVM、vePortal控制面板上都显示虚拟技术。

6、使用专门的软件：virt-what ，virt-what是一个判断当前环境所使用的虚拟技术的脚本，常见的虚拟技术基本上都能正常识别出来。

可以执行如下命令安装(需要安装好gcc、make)：

wget http://people.redhat.com/~rjones/virt-what/files/virt-what-1.15.tar.gz
tar zxvf virt-what-1.15.tar.gz
cd virt-what-1.15/
./configure
make && make install

再运行 virt-what ，脚本就会判断出当前环境所使用的虚拟技术，VPS侦探用手头上的VPS测试成功率还是比较高的。
