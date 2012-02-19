FILE=test.c
NAME=test

gcc -c $FILE
ld -o $NAME -Ttext 0xc0004038 -e main $NAME.o
objcopy -R .note -R .comment -S -O binary $NAME $NAME.bin
#/home/caos/Projects/OS/OS/Utilities/InitRD/initrdmaker/maker initrd $NAME.bin


