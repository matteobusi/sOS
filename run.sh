#!/bin/sh

MEM=1024
#HDA=./HDD/wsfoxhd.img
GRUB=./GRUB/*
KER=./bin/sos.bin
IRD=./test
FDA=./img/floppy.img

#runs qemu!
#sudo qemu-system-i386 -fda $FDA -m $MEM
echo "# Building floppy image..."
./make_floppy.sh
echo "# Running VirtualMachine..."
#virtualbox --startvm sos --debug --start-running
#qemu-system-i386 -fda $FDA -m $MEM
bochs
echo "# Run finished"
#bochs
