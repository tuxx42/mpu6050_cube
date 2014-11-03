/*
nrf24l01 lib sample

copyright (c) Davide Gironi, 2012

Released under GPLv3.
Please refer to LICENSE file for licensing information.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

//include uart
#include "uart/uart.h"
#define UART_BAUD_RATE 57600

//include nrf24l01
#include "nrf24l01/nrf24l01.h"

//main here
int main(void) {
	uint8_t i = 0;
	char *wheel = "|/-\\";

	//sending buffer addresses
	uint8_t sendpipe = 0;
	uint8_t addrtx0[NRF24L01_ADDRSIZE] = NRF24L01_ADDRP0;

#if defined(TX)
	long *ptr = 0;
	double qw = 1.0f;
	double qx = 0.0f;
	double qy = 0.0f;
	double qz = 0.0f;
	double roll = 0.0f;
	double pitch = 0.0f;
	double yaw = 0.0f;

	//nrf24l01 variables
	uint8_t bufferout[NRF24L01_PAYLOAD];
#elif defined(RX)
	uint8_t bufferin[NRF24L01_PAYLOAD];
#endif

	//init uart
	uart_init(UART_BAUD_SELECT(UART_BAUD_RATE, F_CPU));
	mpu6050_init();
	nrf24l01_init();
	sei();

	uart_puts("\r\nUART initialized... \r\n");
	_delay_ms(50);
	uart_puts("nrf24l01+ initialized... \r\n");

	//init nrf24l01
	_delay_ms(50);
#if defined(TX)
	mpu6050_dmpInitialize();
	mpu6050_dmpEnable();
	_delay_ms(10);
	uart_puts("mpu6050 initialized... \r\n");
#endif

	//init interrupt
	uart_puts("interrupts enabled... \r\n");

	for(i=0; i<100; i++) {
		uart_putc(wheel[i%4]);
		_delay_ms(10);
		uart_putc('\b');
	}
	uart_puts("\r\n");

#if defined(TX)
	uart_puts("starting as tx...\r\n");
#elif defined(RX)
	uart_puts("starting as rx...\r\n");
#endif

	//setup buffer
#if defined(TX)
	for(i=0; i<sizeof(bufferout); i++)
		bufferout[i] = ' ';
#elif defined(RX)
	for(i=0; i<sizeof(bufferin); i++)
		bufferin[i] = 0;
#endif

#if NRF24L01_PRINTENABLE == 1
	nrf24l01_printinfo(uart_puts, uart_putc);
#endif

	//main loop
	nrf24l01_settxaddr(addrtx0);

#if defined(TX)
	for(;;) {
		//tx
		char pipebuffer[5];
		itoa(sendpipe, pipebuffer, 10);

		if(mpu6050_getQuaternionWait(&qw, &qx, &qy, &qz))
			mpu6050_getRollPitchYaw(qw, qx, qy, qz, &roll, &pitch, &yaw);

		_delay_ms(10);

		//quaternion
		ptr = (long *)(&qw);
		for (i=0; i<4; i++)
			*(bufferout+i) = *ptr>>(i*8);

		ptr = (long *)(&qx);
		for (i=0; i<4; i++)
			*(bufferout+i+4) = *ptr>>(i*8);

		ptr = (long *)(&qy);
		for (i=0; i<4; i++)
			*(bufferout+i+8) = *ptr>>(i*8);

		ptr = (long *)(&qz);
		for (i=0; i<4; i++)
			*(bufferout+i+12) = *ptr>>(i*8);

		// roll pitch yaw
		ptr = (long *)(&roll);
		for (i=0; i<4; i++)
			*(bufferout+i+16) = *ptr>>(i*8);

		ptr = (long *)(&pitch);
		for (i=0; i<4; i++)
			*(bufferout+i+20) = *ptr>>(i*8);

		ptr = (long *)(&yaw);
		for (i=0; i<4; i++)
			*(bufferout+i+24) = *ptr>>(i*8);

		nrf24l01_write(bufferout);
	}
#elif defined(RX)
	for(;;) {
		//rx
		uint8_t pipe = 0;
		if(nrf24l01_readready(&pipe)) { //if data is ready
			//read buffer
			nrf24l01_read(bufferin);

			for (i=0; i < 4*7; i++)
				uart_putc(*(bufferin+i));
			uart_putc('\n');
		}
		_delay_ms(10);
	}
#endif
}
