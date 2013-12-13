/*
 * fsmc.c
 *
 *  Created on: 2013-11-22
 *      Author: lrfy
 */
#include"fsmc.h"
#include"stm32f4xx.h"
#include"stm32f4xx_conf.h"

void fsmc_clk_conf()
{
	  	RCC_AHB3PeriphClockCmd(RCC_AHB3Periph_FSMC, ENABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);
}

void fsmc_gpio_conf()
{
		GPIO_InitTypeDef GPIO_conf;
		///////////////////////////////////////////////////////FSMC_D0-15
		GPIO_PinAFConfig(GPIOD, GPIO_PinSource0, GPIO_AF_FSMC);
		GPIO_PinAFConfig(GPIOD, GPIO_PinSource1, GPIO_AF_FSMC);
		GPIO_PinAFConfig(GPIOD, GPIO_PinSource8, GPIO_AF_FSMC);
		GPIO_PinAFConfig(GPIOD, GPIO_PinSource9, GPIO_AF_FSMC);
		GPIO_PinAFConfig(GPIOD, GPIO_PinSource10, GPIO_AF_FSMC);
		GPIO_PinAFConfig(GPIOD, GPIO_PinSource14, GPIO_AF_FSMC);
		GPIO_PinAFConfig(GPIOD, GPIO_PinSource15, GPIO_AF_FSMC);
		GPIO_PinAFConfig(GPIOE, GPIO_PinSource7, GPIO_AF_FSMC);
		GPIO_PinAFConfig(GPIOE, GPIO_PinSource8, GPIO_AF_FSMC);
		GPIO_PinAFConfig(GPIOE, GPIO_PinSource9, GPIO_AF_FSMC);
		GPIO_PinAFConfig(GPIOE, GPIO_PinSource10, GPIO_AF_FSMC);
		GPIO_PinAFConfig(GPIOE, GPIO_PinSource11, GPIO_AF_FSMC);
		GPIO_PinAFConfig(GPIOE, GPIO_PinSource12, GPIO_AF_FSMC);
		GPIO_PinAFConfig(GPIOE, GPIO_PinSource13, GPIO_AF_FSMC);
		GPIO_PinAFConfig(GPIOE, GPIO_PinSource14, GPIO_AF_FSMC);
		GPIO_PinAFConfig(GPIOE, GPIO_PinSource15, GPIO_AF_FSMC);
		///////////////////////////////////////////////////////FSMC_A16
		GPIO_PinAFConfig(GPIOD, GPIO_PinSource11, GPIO_AF_FSMC);
		///////////////////////////////////////////////////////FSMC_NE1-NOE-NWE
		GPIO_PinAFConfig(GPIOD, GPIO_PinSource4, GPIO_AF_FSMC);
		GPIO_PinAFConfig(GPIOD, GPIO_PinSource5, GPIO_AF_FSMC);
		GPIO_PinAFConfig(GPIOD, GPIO_PinSource7, GPIO_AF_FSMC);
		///////////////////////////////////////////////////////FSMC_D0-15 GPIO_conf
		GPIO_conf.GPIO_Mode=GPIO_Mode_AF;
		GPIO_conf.GPIO_OType=GPIO_OType_PP ;
		GPIO_conf.GPIO_Pin=GPIO_Pin_14 | GPIO_Pin_15 | GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10;
		GPIO_conf.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_conf.GPIO_Speed=GPIO_Speed_100MHz;
		GPIO_Init(GPIOD, &GPIO_conf);
		GPIO_conf.GPIO_Mode=GPIO_Mode_AF;
		GPIO_conf.GPIO_OType=GPIO_OType_PP ;
		GPIO_conf.GPIO_Pin=GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
		GPIO_conf.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_conf.GPIO_Speed=GPIO_Speed_100MHz;
		GPIO_Init(GPIOE, &GPIO_conf);
		///////////////////////////////////////////////////////FSMC_A16 GPIO_conf
		GPIO_conf.GPIO_Mode=GPIO_Mode_AF;
		GPIO_conf.GPIO_OType=GPIO_OType_PP ;
		GPIO_conf.GPIO_Pin=GPIO_Pin_11;
		GPIO_conf.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_conf.GPIO_Speed=GPIO_Speed_100MHz;
		GPIO_Init(GPIOD, &GPIO_conf);
		///////////////////////////////////////////////////////FSMC_Ctrl Pins GPIO_conf
		GPIO_conf.GPIO_Mode=GPIO_Mode_AF;
		GPIO_conf.GPIO_OType=GPIO_OType_PP ;
		GPIO_conf.GPIO_Pin=GPIO_Pin_7 | GPIO_Pin_4 | GPIO_Pin_5;
		GPIO_conf.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_conf.GPIO_Speed=GPIO_Speed_100MHz;
		GPIO_Init(GPIOD, &GPIO_conf);////NE1-NOE-NWE

}

void fsmc_conf()
{
		FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMconf;
		FSMC_NORSRAMTimingInitTypeDef  FSMC_timconf_write;


	    /*-- FSMC Configuration ------------------------------------------------------*/
	    /*----------------------- NOR Bank 1 ----------------------------------------*/
	    /* FSMC_Bank1_NORSRAM1 configuration */
		FSMC_timconf_write.FSMC_AddressSetupTime = 5;
		FSMC_timconf_write.FSMC_AddressHoldTime = 0;
		FSMC_timconf_write.FSMC_DataSetupTime = 9;
		FSMC_timconf_write.FSMC_BusTurnAroundDuration = 0;
		FSMC_timconf_write.FSMC_CLKDivision = 0;
		FSMC_timconf_write.FSMC_DataLatency = 0;
		FSMC_timconf_write.FSMC_AccessMode = FSMC_AccessMode_A;

		/*FSMC_timconf_read.FSMC_AddressSetupTime = 5;
		FSMC_timconf_read.FSMC_AddressHoldTime = 0;
		FSMC_timconf_read.FSMC_DataSetupTime = 9;
		FSMC_timconf_read.FSMC_BusTurnAroundDuration = 0;
		FSMC_timconf_read.FSMC_CLKDivision = 0;
		FSMC_timconf_read.FSMC_DataLatency = 0;
		FSMC_timconf_read.FSMC_AccessMode = FSMC_AccessMode_A;*/
	    /*
	     Color LCD configuration ------------------------------------
	     LCD configured as follow:
	        - Data/Address MUX = Enable
	        - Memory Type = Nor Flash
	        - Data Width = 16bit
	        - Write Operation = Enable
	        - Extended Mode = Enable
	        - Asynchronous Wait = Disable
	    */
	    FSMC_NORSRAMconf.FSMC_Bank = FSMC_Bank1_NORSRAM1;
	    FSMC_NORSRAMconf.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;
	    FSMC_NORSRAMconf.FSMC_MemoryType = FSMC_MemoryType_SRAM;
	    FSMC_NORSRAMconf.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;
	    FSMC_NORSRAMconf.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;
	    FSMC_NORSRAMconf.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
	    FSMC_NORSRAMconf.FSMC_WrapMode = FSMC_WrapMode_Disable;
	    FSMC_NORSRAMconf.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;
	    FSMC_NORSRAMconf.FSMC_WriteOperation = FSMC_WriteOperation_Enable;
	    FSMC_NORSRAMconf.FSMC_WaitSignal = FSMC_WaitSignal_Disable;
	    FSMC_NORSRAMconf.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable;
	    FSMC_NORSRAMconf.FSMC_AsynchronousWait = FSMC_AsynchronousWait_Disable;
	    FSMC_NORSRAMconf.FSMC_WriteBurst = FSMC_WriteBurst_Disable;
	    FSMC_NORSRAMconf.FSMC_ReadWriteTimingStruct = &FSMC_timconf_write;
	    FSMC_NORSRAMconf.FSMC_WriteTimingStruct = &FSMC_timconf_write;

	    FSMC_NORSRAMInit(&FSMC_NORSRAMconf);

	    /* BANK 1 (of NOR/SRAM Bank 1~4) is enabled */
	    FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM1, ENABLE);
}

void fsmc_init()
{
	fsmc_clk_conf();
	fsmc_gpio_conf();
	fsmc_conf();

}
