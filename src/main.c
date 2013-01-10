/**
*****************************************************************************
**
**  File        : main.c
**
**  Abstract    : main function.
**
**  Functions   : main
**
**  Environment : uVision
**                STMicroelectronics STM32F4xx Standard Peripherals Library
**
*****************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"
#include "OV7670.h"
#include "SSD1289.h"

// Co treba urobit
/*
 	PWR_CR = 1 aby haclk_max = 168Mhz
	pozri PXCLK max 58mhz str135

*/


/* Private typedef -----------------------------------------------------------*/
GPIO_InitTypeDef  GPIO_InitStructure;
NVIC_InitTypeDef NVIC_InitStructure;


/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint16_t Color = 0;
/* Private function prototypes -----------------------------------------------*/
void Delay(__IO uint32_t nCount);

uint8_t Adresa = 0, Data = 0, AdresaR = 0;
/* Private functions ---------------------------------------------------------*/

/**
**===========================================================================
**
**  Abstract: main program
**
**===========================================================================
*/
int main(void)
{
// uintXX_t
   	uint8_t pom = 0, dat;
   	uint32_t data = 0; 
   	uint16_t x=0, y=0;
	uint16_t i = 0;
   	uint8_t Adresa = 0, Data = 0, AdresaR = 0;

// Status
	ErrorStatus Stat;
	FlagStatus Flag;
	ReturnState *RetState;
	
	DCMI_TypeDef DCMI_Reg;

	Delay(0x3FFFFF);

	// Initialize Camera Module OV7670 (DCMI, DMA)
	Stat = Camera_Init();
	
	// Initialize of LCD Module with SSD1286 controller	
	LCD_Init_SSD1289();	

	// Clear LCD Module with White background
	LCD_Clear(0x9999);
	
	NVIC_InitStructure.NVIC_IRQChannel = DCMI_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	DCMI_ITConfig(DCMI_IT_FRAME, ENABLE);
	// Enable DCMI Capture mode
 	DCMI_Cmd(ENABLE);
	DCMI_CaptureCmd(ENABLE); 

	// DMA Stream enable
	DMA_Cmd(DMA_CameraToRAM_Stream, ENABLE);

	// Infinite loop
	while(1)
	{
	//	Delay(0x3FF);
	//	LCD_Clear(i);
	//	i++;
//	 	Camera_WriteReg(Adresa, Data);
	//	RetState = Camera_ReadReg(AdresaR);
	}
}




/**
  * @brief  Delay Function.
  * @param  nCount:specifies the Delay time length.
  * @retval None
  */
void Delay(__IO uint32_t nCount)
{
  while(nCount--)
  {
  }
}



#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
	//printf("Wrong parameters value: file %s on line %d\r\n", file, line);
  /* Infinite loop */
  while (1)
  {
  }
}
#endif
