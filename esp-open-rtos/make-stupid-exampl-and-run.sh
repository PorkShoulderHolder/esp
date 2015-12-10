export PATH=/Volumes/case-sensitive/esp-open-sdk/xtensa-lx106-elf/bin:$PATH
make flash -j4 -C examples/serial_echo ESPPORT=/dev/tty.SLAB_USBtoUART
screen /dev/cu.SLAB_USBtoUART baud_rate 9600