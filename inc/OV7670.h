/**
  ******************************************************************************
  * @file    OV7670.h
  * @author  Juraj
  * @version 
  * @date    Jul 17, 2011
  * @brief	 Control of  CMOS VGA Camera Module OV7670
  ******************************************************************************

  ******************************************************************************
  */

#ifndef OV7670_H_
#define OV7670_H_


/* Includes -------------------------------------------------- ----------------*/
#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"


/* Exported typedef -------------------------------------------------- ---------*/
// Direction configuration of GPIO SIO_D pin
typedef enum
{
	OUT = 0,
	IN = 1,
}Direction;

/* Exported define -------------------------------------------------- ----------*/
#define Error			0
#define Success			1

// Pin define of SCCB/I2C interface
#define SIO_C_Pin  	    GPIO_Pin_5		//PA5
#define SIO_D_Pin    	GPIO_Pin_7		//PA7
#define SCCB_Port		GPIOA
#define SCCB_Clock		RCC_AHB1Periph_GPIOA
// SIO_C SCCB/I2C clock
#define SIO_C_Hi()     	SCCB_Port->BSRRL = SIO_C_Pin;
#define SIO_C_Lo()     	SCCB_Port->BSRRH =  SIO_C_Pin;
// SIO_D SCCB/I2C data
#define SIO_D_Hi()     	SCCB_Port->BSRRL = SIO_D_Pin;
#define SIO_D_Lo()     	SCCB_Port->BSRRH =  SIO_D_Pin;
// Read state of SIO_D of SCCB/I2C
#define SIO_D_STATE	 	SCCB_Port->IDR&(SIO_D_Pin)

// Number of register to change value when initialize Camera Module
#define RegNum		134
#define CHANGE_REG	167

// If Verify is defined, initialize of Camera Module registers is verified with
// data stored in InitBuffer
#define Camera_Verify		ENABLE
// Choose between normal and debug camera operation
#define	Camera_Debug		ENABLE
#define Debug_Register_Num 7

#define picture_x 176 
#define picture_y 144 

/* Exported macro -------------------------------------------------- -----------*/

// Macro for retur Data and State of function
typedef struct
{
	uint8_t Data;			// Data as return value

	ErrorStatus State;		// Return State of function
}ReturnState;

/* Exported variables -------------------------------------------------- -------*/


/* Exported constants ---------------------------------------------------------*/

// The period of the delay will depend on the system operating frequency. The following
// value has been set for system running at 168 MHz.
#define DCMI_TIMEOUT_MAX         10000
// maximal timeout
#define TIMEOUT_MAX              10000
// Define startup delay for HSI (High speed internal RC oscillator)
#define HSI_STARTUP_TIMEOUT		 10000

// Address of Camera module (OV7670) for SCCB/I2C interface
#define ReadAddress		0x43
#define WriteAddress	0x42

// Address of peripheral for DMA channel
#define DCMI_DR_ADDRESS       0x50050028
#define FSMC_LCD_ADDRESS      0x68000002

// Na pokusy
#define BuffSize		48000

// DMA Stream parameters definitions. You can modify these parameters to select
// a different DMA Stream and/or channel.
// But note that only DMA2 Streams are capable of Memory to Memory transfers.
#define DMA_CameraToRAM_Stream   		DMA2_Stream7
#define DMA_Camera_Channel         		DMA_Channel_1
#define DMA_Camera_STREAM_CLOCK    		RCC_AHB1Periph_DMA2
#define DMA_Camera_STREAM_IRQ      		DMA2_Stream7_IRQn
#define DMA_Camera_IT_TCIF         		DMA_IT_TCIF7
#define DMA_Camera_STREAM_IRQHANDLER    DMA2_Stream7_IRQHandler

// Camera Module (OV7670/OV7171) Register address
#define OV7670_COM7		0x12

/* Exported function prototypes -----------------------------------------------*/
uint8_t SCCB_Read(void);
ErrorStatus camera_Init(void);
ErrorStatus SCCB_Write(uint8_t Data);
uint8_t Camera_WriteReg(uint8_t Address, uint8_t Value);
ReturnState *Camera_ReadReg(uint8_t Address);
void Camera_Reset(void);

/* Exported functions -------------------------------------------------- -------*/

#endif /* OV7670_H_ */
