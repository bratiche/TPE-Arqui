#!/bin/bash
#printf -v macaddr "52:54:%02x:%02x:%02x:%02x" $(( $RANDOM & 0xff)) $(( $RANDOM & 0xff )) $(( $RANDOM & 0xff)) $(( $RANDOM & 0xff ))
#echo $macaddr
qemu-system-x86_64 -hda Image/x64BareBonesImage.qcow2 -m 512 -vga std -netdev user,id=mynet0 -net nic,macaddr=54:52:15:00:56:25,model=rtl8139,netdev=mynet0,vlan=0 
