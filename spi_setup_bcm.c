
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
	
	
	// Commands below write the config register (00000) to 0x0a
	spi_txbuff[0]=0x20;
	spi_txbuff[1]=0x0a;
	bcm2835_spi_transfern((char *) spi_txbuff,2);
	
	/*
	// Commands below read the config register (00000) and the RF_STATUS (value=00000111) by def.
	spi_txbuff[0]=0x00;
	spi_txbuff[1]=0x06;
	bcm2835_spi_transfernb((char *) spi_txbuff, (char *) spi_rxbuff, 4);
	
	for(uint8_t i =0; i<3;i++){
		printf("Read from SPI: 0x%02X\n", *(spi_rxbuff+i));
	}
	*/
	/******************** ++ CODE TO READ RX_FIFO ++ **********************/
	// Read FIFO status:
	uint8_t comm_to_read_rx_fifo[2];
	comm_to_read_rx_fifo[0]=0x17;
	*(spi_rxbuff+1)=0x11; //para que entre
	while((*(spi_rxbuff+1))==0x11){

		bcm2835_spi_transfernb((char *) comm_to_read_rx_fifo, (char * ) spi_rxbuff, 2);
		//bcm2835_spi_transfern((char *) comm_to_read_rx_fifo, 2);
		printf("Fifo status = 0x%02X\n", *(spi_rxbuff+1));
	}
	/*
	// Read RX_FIFO
	spi_txbuff[0]=0x61; // Read rx_fifo command(pg. 48)
	bcm2835_spi_transfernb((char *) spi_txbuff, (char * ) spi_rxbuff, 32);
	for(uint8_t i=0; i<32;i++){
		printf("Fifo status = 0x%02X\n", *(spi_rxbuff+i));
	}
	*/
	bcm2835_spi_end();
    	bcm2835_close();
	return 0;
}

