GRUB=./GRUB
KER=./bin/sos.bin
IRD=./test
FDA=./img/floppy.img

PASS=caos@home

#creates the image
echo "#     Mounting image"
echo $PASS | sudo -S mount -o loop $FDA fda
if [ ! -d "fda/boot" ]; then
    echo "#    Creating boot"
    echo $PASS | sudo -S mkdir fda/boot
fi
if [ ! -d "fda/boot/grub" ]; then
    echo "#    Creating fda/boot/grub"
    echo $PASS | sudo -S mkdir fda/boot/grub
fi

echo "#     Copying Kernel"
echo $PASS | sudo -S cp $KER fda/kernel
echo "#     Copying InitRD"
echo $PASS | sudo -S cp $IRD fda/initrd
echo "#     Copying GRUB"
echo $PASS | sudo -S cp $GRUB/* ./fda/boot/grub/
echo "#     Unmounting"
echo $PASS | sudo -S umount $FDA
