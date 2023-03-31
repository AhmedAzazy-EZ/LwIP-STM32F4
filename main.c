/*
 * maint.c
 *
 * Created: 29-March-23
 *  Author: Ahmed Azazy
 */


/* Imortant links 
 *https://lwip.fandom.com/wiki/LwIP_Application_Developers_Manual
 *https://www.nongnu.org/lwip/2_1_x/group__httpd.html#gac364305cee969a0be43c071722b136e6
*/


#include <stdlib.h>
#include "stm32f4xx_hal.h"
#include "lwip.h"
#include "lwip/ip_addr.h"
#include "tcp_client.h"

HAL_StatusTypeDef GPIO_Init(void);
HAL_StatusTypeDef SPI1_Init(void);

SPI_HandleTypeDef hspi1;

struct ip_addr ip;
struct tcp_pcb my_pcb ;

int main(void)
{

	HAL_Init();
	GPIO_Init();
	HAL_GPIO_WritePin(GPIOA , GPIO_PIN_4 , GPIO_PIN_SET);
	if(SPI1_Init() != HAL_OK)
	{
		//Cannot Init the SPI
		while(1);
	}
	
	//Local Server
	IP4_ADDR(&ip, 192,168,1,7);
	
	LwIP_Init();
	tcp_init();
	while(1)
	{
		LwIP_Pkt_Handle();
		LwIP_Periodic_Handle(LocalTime);
		tcp_request(&ip , &my_pcb);
	}
}

HAL_StatusTypeDef GPIO_Init(void)
{
	__HAL_RCC_GPIOA_CLK_ENABLE();
	GPIO_InitTypeDef GPIO = {0};
	
	
	GPIO.Pin = GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;
	GPIO.Mode = GPIO_MODE_AF_PP;
	GPIO.Pull = GPIO_NOPULL;
	GPIO.Speed = GPIO_SPEED_FREQ_LOW;
	GPIO.Alternate = GPIO_AF5_SPI1;
	HAL_GPIO_Init (GPIOA, &GPIO);
	
	
	GPIO.Pin = GPIO_PIN_4;
	GPIO.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO.Pull = GPIO_NOPULL;
	GPIO.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init (GPIOA, &GPIO);

	return HAL_OK;
}

HAL_StatusTypeDef SPI1_Init(void)
{
	__HAL_RCC_SPI1_CLK_ENABLE();
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_32;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
 return HAL_SPI_Init(&hspi1);
}

void SysTick_Handler(void)
{
	HAL_IncTick();
	//Time_Update() is required by LwIp Lib
	Time_Update();
}