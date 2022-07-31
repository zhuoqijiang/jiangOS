#! /bin/bash

nasm ./boot/boot.asm
nasm ./boot/loader.asm
make -C ./kernel


if [ -f "./boot/boot" ]
then
    dd if=boot/boot of=os.img bs=512 count=1 seek=0 conv=notrunc
    echo "boot/boot add to soft "
else
    echo "boot no exist"
fi

if [ -f "./boot/loader" ]
then
    dd if=boot/loader of=os.img bs=512 count=4 seek=1 conv=notrunc
    echo "boot/loader add to soft"
else
    echo "loader no exist"
fi

if [ -f "./kernel/kernel" ]
then
    dd if=kernel/kernel of=os.img bs=512 count=255 seek=5 conv=notrunc
    echo "kernel/kernel add to soft"
else
    echo "kernel no exist"
fi

