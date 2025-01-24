# Script for RCOM demonstration: tux62
# Authors:
# Guilherme Duarte Silva Matos (up202208755@up.pt)
# João Vítor da Costa Ferreira (up202208393@up.pt)

# IPs

bridge60=172.16.66.0/24
tux63eth1=172.16.66.1
tux64eth1=172.16.66.254

bridge61=172.16.67.0/24
tux62eth1=172.16.67.1
tux64eth2=172.16.67.253
routereth2=172.16.67.254

bridgeFTP=172.16.1.0/24
routereth1=172.16.1.61
ftpserver=172.16.1.10



# Reset configuration
systemctl restart networking

# Experience #1
# N/A

# Experience #2
ifconfig eth1 up
ifconfig eth1 ${tux62eth1}/24

# Experience #3
route add -net ${bridge60} gw ${tux64eth2}

# Experience #4
# route add -net $bridge60 gw $tux64eth2
route add -net ${bridgeFTP} gw ${routereth2}
sysctl net.ipv4.conf.eth1.accept_redirects=0
sysctl net.ipv4.conf.all.accept_redirects=0

# Experience #5
echo "nameserver 10.227.20.3" >> /etc/resolv.conf
