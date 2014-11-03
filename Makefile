MMCU=atmega328p
CROSS_COMPILE=/opt/avoccado/bin/
F_CPU=16000000L
CFLAGS+=-Os -DNRF24L01_PRINTENABLE -DF_CPU=$(F_CPU) -mmcu=$(MMCU) -Wno-deprecated-declarations #-DEULER #-D__PROG_TYPES_COMPAT__
CC=$(CROSS_COMPILE)avr-gcc
OBJCOPY=avr-objcopy
PROG=bin
BAUD=57600
SERIAL=/dev/ttyUSB

all: rx tx

rx: uart/uart.o nrf24l01/nrf24l01.o spi/spi.o i2chw/twimastertimeout.o \
	mpu6050/mpu6050.o mpu6050/mpu6050dmp6.o
	$(CC) $(CFLAGS) -DRX -o main.o -c main.c
	$(CC) $(CFLAGS) -lm -o rx_$(PROG).elf $^ main.o
	$(OBJCOPY) -O ihex -j .text -j .data rx_$(PROG).elf rx_$(PROG).hex

tx: uart/uart.o nrf24l01/nrf24l01.o spi/spi.o i2chw/twimastertimeout.o \
	mpu6050/mpu6050.o mpu6050/mpu6050dmp6.o
	$(CC) $(CFLAGS) -DTX -o main.o -c main.c
	$(CC) $(CFLAGS) -lm -o tx_$(PROG).elf $^ main.o
	$(OBJCOPY) -O ihex -j .text -j .data tx_$(PROG).elf tx_$(PROG).hex

clean:
	rm -f */*.o *.o *.elf *.hex

flash0:
	$(CROSS_COMPILE)avrdude -q -p$(MMCU) -carduino -P$(SERIAL)0 -b$(BAUD) -D -Uflash:w:$(PROG).hex:i

flash1:
	$(CROSS_COMPILE)avrdude -q -p$(MMCU) -carduino -P$(SERIAL)1 -b$(BAUD) -D -Uflash:w:$(PROG).hex:i

.PHONY: clean flash1 flash2 rx tx
