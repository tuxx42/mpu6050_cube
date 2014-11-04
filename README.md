mpu6050_cube
============

build:
$ make

flashing (e.g. MMCU=atmega328p, F_CPU=16000000L, SERIAL=/dev/ttyUSB0):
$ avrdude -q -p$(MMCU) -carduino -P$(SERIAL)0 -b$(BAUD) -D -Uflash:w:rx_bin.hex:i
