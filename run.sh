#!/bin/sh

MEM=1024
KER=./bin/voxos.bin
#IRD=./initrd.vos --> can be added, see version 1.7
#runs qemu!
qemu -kernel $KER -m $MEM
#bochs
