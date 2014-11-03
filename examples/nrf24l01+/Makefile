MMCU=atmega328p
CFLAGS+=-Os -DNRF24L01_PRINTENABLE -DF_CPU=16000000L -mmcu=$(MMCU) -Wno-deprecated-declarations #-DEULER #-D__PROG_TYPES_COMPAT__
CC=avr-gcc
OBJCOPY=avr-objcopy
PROG=bin
CROSS_COMPILE=/opt/avoccado/bin/
BAUD=57600
SERIAL=/dev/ttyUSB

rx: uart/uart.o nrf24l01/nrf24l01.o spi/spi.o main.o 
	$(CC) $(CFLAGS) -lm -o $(PROG).elf $^
	$(OBJCOPY) -O ihex -j .text -j .data $(PROG).elf $(PROG).hex

tx: uart/uart.o nrf24l01/nrf24l01.o spi/spi.o main.o 
	$(CC) $(CFLAGS) -lm -o $(PROG).elf $^
	$(OBJCOPY) -O ihex -j .text -j .data $(PROG).elf $(PROG).hex

main.o: main.c
	$(CC) $(CFLAGS) -o $@ -c main.c

uart/uart.o: uart/uart.c
	$(CC) $(CFLAGS) -o $@ -c uart/uart.c

nrf24l01/nrf24l01.o: nrf24l01/nrf24l01.c
	$(CC) $(CFLAGS) -o $@ -c nrf24l01/nrf24l01.c

spi/spi.o: spi/spi.c
	$(CC) $(CFLAGS) -o $@ -c spi/spi.c

clean:
	rm -f */*.o *.o $(PROG).elf $(PROG).hex

flash0:
	$(CROSS_COMPILE)avrdude -q -p$(MMCU) -carduino -P$(SERIAL)0 -b$(BAUD) -D -Uflash:w:$(PROG).hex:i

flash1:
	$(CROSS_COMPILE)avrdude -q -p$(MMCU) -carduino -P$(SERIAL)1 -b$(BAUD) -D -Uflash:w:$(PROG).hex:i

.PHONY: clean flash1 flash2
