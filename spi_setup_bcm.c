
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
	//Page 47 nordic: Command word: MSBit to LSBit
	bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);
	bcm2835_spi_chipSelect(BCM2835_SPI_CS0);
	//Every new command in the nordic must be started by a high to low transition on CSN
	bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS0, LOW);
	
	uint8_t spi_txbuff[32+1];
	uint8_t spi_rxbuff[32+1]; //+1 for the status 
	
	/*
	// Commands below write the config register (00000) to 0x0f
	spi_txbuff[0]=0x20;
	spi_txbuff[1]=0x0f;
	bcm2835_spi_transfern((char *) spi_txbuff,2);
	*/

	/*
	// Commands below read the config register (00000)
	spi_txbuff[0]=0x00;
	spi_txbuff[1]=0x00;
	*/
	
	bcm2835_spi_transfernb((char *) spi_txbuff, (char *) spi_rxbuff, 3);
	for(uint8_t i =0; i<3;i++){
		printf("Read from SPI: 0x%02X\n", *(spi_rxbuff+i));
	}
	bcm2835_spi_end();
    	bcm2835_close();
	return 0;
}

