#!/bin/sh
echo "Hello, World! /dev/console" > /dev/console
echo "Hello, World! /dev/tty1" > /dev/tty1
echo "Hello, World! /tmp/boot.txt" > /tmp/boot.txt

modprobe cdc_ether
modprobe usbnet
modprobe mii
modprobe xt_conntrack
modprobe ipt_MASQUERADE
modprobe nf_nat_masquerade_ipv4
modprobe nf_conntrack_netlink
modprobe nfnetlink
modprobe xt_addrtype
modprobe iptable_filter
modprobe iptable_nat
modprobe nf_conntrack_ipv4
modprobe nf_defrag_ipv4
modprobe nf_nat_ipv4
modprobe nf_nat
modprobe nf_conntrack
modprobe br_netfilter
modprobe bridge
modprobe stp
modprobe llc
modprobe exfat
modprobe gpio_keys
modprobe dwc2
modprobe sch_fq_codel
modprobe ip_tables
modprobe x_tables
modprobe ipv6