#!/bin/bash
#printf -v macaddr "52:54:%02x:%02x:%02x:%02x" $(( $RANDOM & 0xff)) $(( $RANDOM & 0xff )) $(( $RANDOM & 0xff)) $(( $RANDOM & 0xff ))
#echo $macaddr
#qemu-system-x86_64 -hda Image/x64BareBonesImage.qcow2 -m 512 -vga std  -netdev user,id=mynet0 -net nic,model=rtl8139,macaddr=54:52:15:00:56:24,netdev=mynet0 -net tap,ifname=tap1,script=no,downscript=no
qemu-system-x86_64 -hda Image/x64BareBonesImage.qcow2 -m 512 -vga std -no-kvm-irqchip -device rtl8139,netdev=net0,mac=52:54:AB:CD:EF:13 -netdev tap,id=net0,ifname=tap1
