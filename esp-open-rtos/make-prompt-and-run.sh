export PATH=/Volumes/case-sensitive/esp-open-sdk/xtensa-lx106-elf/bin:$PATH
make flash -j4 -C examples/prompt ESPPORT=/dev/tty.SLAB_USBtoUART
if [ $? -eq 0 ]; then
    screen /dev/cu.SLAB_USBtoUART baud_rate 115200
fi
