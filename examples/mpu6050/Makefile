MMCU=atmega328p
CFLAGS=-Os -DF_CPU=16000000L -mmcu=$(MMCU) -Wno-deprecated-declarations #-DEULER #-D__PROG_TYPES_COMPAT__
CC=avr-gcc
OBJCOPY=avr-objcopy
PROG=bin
CROSS_COMPILE=/opt/avoccado/bin/
BAUD=57600
SERIAL=/dev/ttyUSB

all: i2chw/twimastertimeout.o mpu6050/mpu6050.o \
	mpu6050/mpu6050dmp6.o uart/uart.o main.o 
	$(CC) $(CFLAGS) -lm -o $(PROG).elf $^
	$(OBJCOPY) -O ihex -j .text -j .data $(PROG).elf $(PROG).hex

main.o: main.c
	$(CC) $(CFLAGS) -o $@ -c main.c

i2chw/twimastertimeout.o: i2chw/twimastertimeout.c
	$(CC) $(CFLAGS) -o $@ -c i2chw/twimastertimeout.c

mpu6050/mpu6050.o: mpu6050/mpu6050.c
	$(CC) $(CFLAGS) -o $@ -c mpu6050/mpu6050.c

mpu6050/mpu6050dmp6.o: mpu6050/mpu6050dmp6.c
	$(CC) $(CFLAGS) -o $@ -c mpu6050/mpu6050dmp6.c

uart/uart.o: uart/uart.c
	$(CC) $(CFLAGS) -o $@ -c uart/uart.c

flash0:
	$(CROSS_COMPILE)avrdude -q -p$(MMCU) -carduino -P$(SERIAL)0 -b$(BAUD) -D -Uflash:w:$(PROG).hex:i

flash1:
	$(CROSS_COMPILE)avrdude -q -p$(MMCU) -carduino -P$(SERIAL)1 -b$(BAUD) -D -Uflash:w:$(PROG).hex:i

clean:
	rm -f */*.o *.o $(PROG).elf $(PROG).hex

.PHONY: clean
