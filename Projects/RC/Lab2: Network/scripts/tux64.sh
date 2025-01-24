# Script for RCOM demonstration: tux64
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
ifconfig eth1 up
ifconfig eth1 ${tux64eth1}/24

# Experience #2
# N/A

# Experience #3
ifconfig eth2 up
ifconfig eth2 ${tux64eth2}/24
sysctl net.ipv4.ip_forward=1
sysctl net.ipv4.icmp_echo_ignore_broadcasts=0   

# Experience #4
route add -net ${bridgeFTP} gw ${routereth2}

# Experience #5
echo "nameserver 10.227.20.3" >> /etc/resolv.conf
