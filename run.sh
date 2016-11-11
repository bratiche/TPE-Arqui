#!/bin/bash
qemu-system-x86_64 -hda Image/x64BareBonesImage.qcow2 -m 512 -vga std -netdev user,id=mynet0 -net nic,macaddr=52:54:00:12:34:56,model=rtl8139,netdev=mynet0 