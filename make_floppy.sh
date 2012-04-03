GRUB=./GRUB
KER=./bin/sos.bin
IRD=./test
FDA=./img/floppy.img

#creates the image
echo "#     Mounting image"
sudo mount -o loop $FDA fda
if [ ! -d "fda/boot" ]; then
    echo "#    Creating boot"
    sudo mkdir fda/boot
fi
if [ ! -d "fda/boot/grub" ]; then
    echo "#    Creating fda/boot/grub"
    sudo mkdir fda/boot/grub
fi

echo "#     Copying Kernel"
sudo cp $KER fda/kernel
echo "#     Copying InitRD"
sudo cp $IRD fda/initrd
echo "#     Copying GRUB"
sudo cp $GRUB/* ./fda/boot/grub/
echo "#     Unmounting"
sudo umount $FDA