export PATH=/Volumes/case-sensitive/esp-open-sdk/xtensa-lx106-elf/bin:$PATH
make
python ../esptool/esptool.py --port=/dev/cu.SLAB_USBtoUART write_flash 0x0 ./bin/0x00000.bin 0x10000 ./bin/0x10000.bin -fs 32m -fm dio -ff 40m
screen /dev/cu.SLAB_USBtoUART baud_rate 9600