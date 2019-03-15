/*
 * spi_setup_bcm.c
 *
 *  Created on: March 21, 2018
 *      Author: Ponc Palau
 *
 *  Test spi communication between bcm and nrf24l01 using bcm2835 library.
 *  compile: gcc -o spi_setup_bcm spi_setup_bcm.c -l bcm2835 
 *
 */


#include <stdio.h>
#include <bcm2835.h>


int main(int argc, char **argv){

	if(!bcm2835_init())
	{
		printf("bcm2835_init() failed\n");
		return -1;
	}
	
	//Start SPI operations. Enables pins for SPI interface.
	if(!bcm2835_spi_begin())
	{
		printf("bcm2835_spi_begin() failed\n");
		return -1;
	}
	//Set SPi clock to 8Mhz (7.8MHz actually)
	bcm2835SPIClockDivider(BCM2835_SPI_CLOCK_DIVIDER_32);
	bcm2835_spi_setDataMode(BCM2835_SPI_MODE0);
	//Every new command in the nordic must be started by a high to low transition on CSN
	//Page 47 nordic: Command word: MSBit to LSBit
	bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);
	bcm2835_spi_chipSelect(BCM2835_SPI_CS0);
	bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS0, LOW);
	
	char buf[5];
	buf[0]=0x00;
	buf[1]=0xaa;
	buf[2]=0xaa;
	buf[3]=0xaa;
	buf[4]=0xff;//data to read status reg

    	bcm2835_spi_transfern(buf, 5);
	for(char i=0; i<5;i++){
    	printf("Read back from SPI: 0x%02X.\n", *(buf+i));
    	}
	bcm2835_spi_end();
    	bcm2835_close();
	return 0;

}
