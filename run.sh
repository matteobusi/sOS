#!/bin/sh

MEM=64
#HDA=./HDD/wsfoxhd.img
GRUB=./GRUB/*
KER=./bin/sos.bin
IRD=./test.bin
FDA=./img/floppy.img

#runs qemu!
#sudo qemu-system-i386 -fda $FDA -m $MEM

echo "# Running VirtualMachine..."
#virtualbox --startvm sos --debug --start-running
qemu-system-i386 -kernel $KER -initrd $IRD -m $MEM
echo "# Run finished"
#bochs
