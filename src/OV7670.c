/**
  ******************************************************************************
  * @file    OV7670.c
  * @author  Juraj
  * @version 
  * @date    Jul 17, 2011
  * @brief	 Control of  CMOS VGA Camera Module OV7670
  ******************************************************************************

  ******************************************************************************
  */

/* Includes -------------------------------------------------- ----------------*/
#include "OV7670.h"
#include "OV7670config.c"


/* Private typedef -------------------------------------------------- ---------*/
/* Private define -------------------------------------------------- ----------*/
/* Private macro -------------------------------------------------- -----------*/
/* Private variables -------------------------------------------------- -------*/
uint8_t hodnota[4];
__IO uint16_t RAM_Buffer[BuffSize];

const uint8_t Camera_REG[CHANGE_REG][2]=
{  
	{0x3a, 0x04},
	{0x40, 0xd0},
	{0x12, 0x14},
	{0x32, 0x80},
	{0x17, 0x16},
	{0x18, 0x04},
	{0x19, 0x02},
	{0x1a, 0x7b},
	{0x03, 0x06},
	{0x0c, 0x00}, //0x00
	{0x3e, 0x00},
	{0x70, 0x00},
	{0x71, 0x00},
	{0x72, 0x11},
	{0x73, 0x00},
	{0xa2, 0x02},
	{0x11, 0x40},
	{0x7a, 0x20},
	{0x7b, 0x1c},
	{0x7c, 0x28},
	{0x7d, 0x3c},
	{0x7e, 0x55},
	{0x7f, 0x68},
	{0x80, 0x76},
	{0x81, 0x80},
	{0x82, 0x88},
	{0x83, 0x8f},
	{0x84, 0x96},
	{0x85, 0xa3},
	{0x86, 0xaf},
	{0x87, 0xc4},
	{0x88, 0xd7},
	{0x89, 0xe8},
	{0x13, 0xe0},
	{0x00, 0x00},
	{0x10, 0x00},
	{0x0d, 0x00},
	{0x14, 0x20},
	{0xa5, 0x05},
	{0xab, 0x07},
	{0x24, 0x75},
	{0x25, 0x63},
	{0x26, 0xA5},
	{0x9f, 0x78},
	{0xa0, 0x68},
	{0xa1, 0x03},
	{0xa6, 0xdf},
	{0xa7, 0xdf},
	{0xa8, 0xf0},
	{0xa9, 0x90},
	{0xaa, 0x94},
	{0x13, 0xe5},
	{0x0e, 0x61},
	{0x0f, 0x4b},
	{0x16, 0x02},
	{0x1e, 0x07},//0x07,
	{0x21, 0x02},
	{0x22, 0x91},
	{0x29, 0x07},
	{0x33, 0x0b},
	{0x35, 0x0b},
	{0x37, 0x1d},
	{0x38, 0x71},
	{0x39, 0x2a},
	{0x3c, 0x78},
	{0x4d, 0x40},
	{0x4e, 0x20},
	{0x69, 0x0c},
	{0x6b, 0x60},
	{0x74, 0x19},
	{0x8d, 0x4f},
	{0x8e, 0x00},
	{0x8f, 0x00},
	{0x90, 0x00},
	{0x91, 0x00},
	{0x92, 0x00},
	{0x96, 0x00},
	{0x9a, 0x80},
	{0xb0, 0x84},
	{0xb1, 0x0c},
	{0xb2, 0x0e},
	{0xb3, 0x82},
	{0xb8, 0x0a},
	{0x43, 0x14},
	{0x44, 0xf0},
	{0x45, 0x34},
	{0x46, 0x58},
	{0x47, 0x28},
	{0x48, 0x3a},
	{0x59, 0x88},
	{0x5a, 0x88},
	{0x5b, 0x44},
	{0x5c, 0x67},
	{0x5d, 0x49},
	{0x5e, 0x0e},
	{0x64, 0x04},
	{0x65, 0x20},
	{0x66, 0x05},
	{0x94, 0x04},
	{0x95, 0x08},
	{0x6c, 0x0a},
	{0x6d, 0x55},
	{0x6e, 0x11},
	{0x6f, 0x9f},
	{0x6a, 0x40},
	{0x01, 0x40},	//0x40
	{0x02, 0x40},
	{0x13, 0xe7},
	{0x15, 0x00},
	{0x4f, 0x80},
	{0x50, 0x80},
	{0x51, 0x00},
	{0x52, 0x22},
	{0x53, 0x5e},
	{0x54, 0x80},
	{0x58, 0x9e},	
	{0x41, 0x08},
	{0x3f, 0x00},
	{0x75, 0x05},
	{0x76, 0xe1},
	{0x4c, 0x00},
	{0x77, 0x01},
	{0x3d, 0xc2},	
	{0x4b, 0x09},
	{0xc9, 0x60},
	{0x41, 0x38},
	{0x56, 0x40},	
	{0x34, 0x11},
	{0x3b, 0x02},
	{0xa4, 0x89},
	{0x96, 0x00},
	{0x97, 0x30},
	{0x98, 0x20},
	{0x99, 0x30},
	{0x9a, 0x84},
	{0x9b, 0x29},
	{0x9c, 0x03},
	{0x9d, 0x4c},
	{0x9e, 0x3f},
	{0x78, 0x04},	
	{0x79, 0x01},
	{0xc8, 0xf0},
	{0x79, 0x0f},
	{0xc8, 0x00},
	{0x79, 0x10},
	{0xc8, 0x7e},
	{0x79, 0x0a},
	{0xc8, 0x80},
	{0x79, 0x0b},
	{0xc8, 0x01},
	{0x79, 0x0c},
	{0xc8, 0x0f},
	{0x79, 0x0d},
	{0xc8, 0x20},
	{0x79, 0x09},
	{0xc8, 0x80},
	{0x79, 0x02},
	{0xc8, 0xc0},
	{0x79, 0x03},
	{0xc8, 0x40},
	{0x79, 0x05},
	{0xc8, 0x30},
	{0x79, 0x26},
	{0x09, 0x01},	 //0x03
	{0x55, 0x00},
	{0x56, 0x40},	
	{0x3b, 0x42},	
};

const uint8_t InitBuffer2[4][2]=
{
	{0x11, 0xc0},
	{0x12, 0x02},
	{0x70, 0xba},
	{0x71, 0x35},
};

/* Private function prototypes -----------------------------------------------*/
static void Camera_HW_Init(void);
static void Delay_us(uint16_t Time);
static void SCCB_Start(void);
static void SCCB_Stop(void);
static void NoAck(void);
static void SIO_D_Dir(Direction Dir);
static ErrorStatus OV7670_XCLK_Conf(void);

/* Private functions -------------------------------------------------- -------*/


/**
  * @brief 	Initialize CMOS VGA Camera Module OV7670
  * @param	None
  * @retval	ERROR, SUCCESS
 */
ErrorStatus Camera_Init(void)
{
	uint8_t i;
	ReturnState *RetState;
	ErrorStatus Status = ERROR;

	// Initialize GPIO(DCMI) pins, MCO1(PA8) as output clock for XCLK
	Camera_HW_Init();

	// Reset all Camera Module registers to the default value
	Camera_Reset();

// Initialize Camera Module registers
// Choose between normal and debug camera operation
#ifdef Camera_Debug
	// for debug operation
	for(i = 0; i < 4; i++)
	{
		Camera_WriteReg(InitBuffer2[i][0], InitBuffer2[i][1]);
	}
#else
	// for normal operation
	for(i = 0; i < CHANGE_REG; i++)
	{
		Camera_WriteReg(Camera_REG[i][0], Camera_REG[i][1]);	 
	}
#endif

// Verify written data into register in Camera Module if is defined Camera_Verify
#ifdef Camera_Verify
	// Choose between normal and debug camera operation
	// for debug operation
	#ifdef Camera_Debug
		for(i = 0; i < 4; i++)
		{
			RetState = Camera_ReadReg(InitBuffer2[i][0]);
			if(RetState->Data == InitBuffer2[i][1] && RetState->State == SUCCESS)
				Status = SUCCESS;
			else
				Status = ERROR;
		}
	// for normal operation
	#else
		for(i = 0; i < CHANGE_REG; i++)
		{
			RetState = Camera_ReadReg(Camera_REG[i][0]);
			if(RetState->Data == Camera_REG[i][1] && RetState->State == SUCCESS)
				Status = SUCCESS;
			else
				Status = ERROR;
		}
	#endif
#else
	Status = SUCCESS;
#endif


	return(Status);
}



/**
  * @brief  Resets the OV7670 camera.
  * @param  None
  * @retval None
  */
void Camera_Reset(void)
{
	Camera_WriteReg(OV7670_COM7, 0x80);
}



/**
  * @brief 	Write data to specific register in Camera Module OV7670
  * @param	Address: address of register
  * 		Value: value of data to be written into register
  * @retval	ERROR, SUCCESS
 */
ErrorStatus Camera_WriteReg(uint8_t Address, uint8_t Value)
{
	// Create start condition on SCCB/I2C interface
	SCCB_Start();
	// Write data (Address of slave device for Write) on SCCB/I2C interface
	if((SCCB_Write(WriteAddress)) == Error)
	{
		SCCB_Stop();			// Create stop condition on SCCB/I2C interface
		return(ERROR);			// Return error and cancel the communication
	}
	Delay_us(100);
	// Write data (Address of register in Camera Module)on SCCB/I2C interface
	if((SCCB_Write(Address)) == Error)
	{
		SCCB_Stop();			// Create stop condition on SCCB/I2C interface
		return(ERROR);			// Return error and cancel the communication
	}
	// Write data (Data to write into register in Camera Module)on SCCB/I2C interface
	if((SCCB_Write(Value)) == Error)
	{
		SCCB_Stop();			// Create stop condition on SCCB/I2C interface
		return(ERROR);			// Return error and cancel the communication
	}
	// Create stop condition on SCCB/I2C interface
	SCCB_Stop();

	return(SUCCESS);
}



/**
  * @brief 	Read data form specific register in Camera Module OV7670
  * @param	Address: address of register in Camera Module
  * @retval	ReturnState.Data -> Return read data from register
  * 		ReturnState.State -> Return state of the transmission
 */
ReturnState *Camera_ReadReg(uint8_t Address)
{
	ReturnState returnState;

	// Structure for return Data and State (Success/Error)
	returnState.Data = 0x00;
	returnState.State = ERROR;

	// Create start condition on SCCB/I2C interface
	SCCB_Start();
	// Write data (Address of slave device for Write) on SCCB/I2C interface
	if(Error == (SCCB_Write(WriteAddress)))
	{
		SCCB_Stop();			// Create stop condition on SCCB/I2C interface
		return(&returnState);	// Return error and cancel the communication
	}
	Delay_us(100);
	// Write data (Address of register in Camera Module)on SCCB/I2C interface
	if(Error == (SCCB_Write(Address)))
	{
		SCCB_Stop();			// Create stop condition on SCCB/I2C interface
		return(&returnState);	// Return error and cancel the communication
	}
	// Create stop condition on SCCB/I2C interface
	SCCB_Stop();
	// Delay for SCCB/I2C
	Delay_us(100);
	// Create start condition on SCCB/I2C interface
	SCCB_Start();
	// Write data (Address of slave device for Read) on SCCB/I2C interface
	if(Error == (SCCB_Write(ReadAddress)))
	{
		SCCB_Stop();			// Create stop condition on SCCB/I2C interface
		return(&returnState);	// Return error and cancel the communication
	}
	Delay_us(500);

	// Received data from Camera Module (SCCB/I2C)
	returnState.Data = SCCB_Read();
	// No acknowlage on SCCB/I2C interface
	NoAck();
	// Create stop condition on SCCB/I2C interface
	SCCB_Stop();
	// If everything is done correct return success
	returnState.State = SUCCESS;

	return(&returnState);
}



/**
  * @brief 	Change configuration I/O pin SIO_D as OUT/IN
  * @param	Configure SIO_D pin as IN/OUT
  * 		@arg IN = 1, OUT = 0
  * @retval	None
 */
static void SIO_D_Dir(Direction Dir)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	// Configure SIO_D pin as Input
	if(Dir == 1)
	{
		GPIO_InitStructure.GPIO_Pin =  SIO_D_Pin;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_Init(SCCB_Port, &GPIO_InitStructure);
	}
	// Configure SIO_D pin as Output
	if(Dir == 0)
	{
		GPIO_InitStructure.GPIO_Pin =  SIO_D_Pin;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_Init(SCCB_Port, &GPIO_InitStructure);
	}
}



/**
  * @brief 	Initialize SCCB/I2C for CMOS VGA module OV7670
  * 		and MCO1 (PA8) XCLK for OV7670 as clock out for Camera Module (XCLK pin)
  * @param	None
  * @retval	None
 */
static void Camera_HW_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	DCMI_InitTypeDef DCMI_InitStructure;
	DMA_InitTypeDef  DMA_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	__IO uint32_t    Timeout = TIMEOUT_MAX;

	RCC_AHB1PeriphClockCmd(SCCB_Clock, ENABLE);

	// Configure SIO_C and SIO_D as OUT (for SCCB/I2C)
	GPIO_InitStructure.GPIO_Pin =  SIO_C_Pin | SIO_D_Pin;

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;		// PullUp is include on module
	GPIO_Init(SCCB_Port, &GPIO_InitStructure);

	// Configure MCO0(PA8) as clock out for Camera Module (XCLK pin)
	OV7670_XCLK_Conf();

	// Configures the DCMI GPIOs to interface with the OV7670 camera module
	// Enable DCMI GPIOs clocks
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB |
	                       RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOE,
	                       ENABLE);

	// Connect DCMI pins to AF13
	// PORTA
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource4, GPIO_AF_DCMI);		// HSYNC
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_DCMI);		// PCLK
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_DCMI);		// D0
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_DCMI);	// D1

	// PORTB
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_DCMI); 	// D5
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_DCMI);		// VSYNC
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_DCMI); 	// D6
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_DCMI);		// D7				   -

	// PORTC
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_DCMI);	// D4

	// PORTE
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource0, GPIO_AF_DCMI);		// D2
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource1, GPIO_AF_DCMI);		// D3

	// DCMI GPIO configuration
	// D0..D1(PA9/10), HSYNC(PA4), PCLK(PA6)
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_6 | GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	// D5..D7(PB6/8/9), VSYNC(PB7)
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	// D4(PC11)
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	// D2..D3(PE0/1)
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
//---------------------------------------------------------------------------------------
//	GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_DCMI); //d0
//	GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_DCMI);	//d1
//	GPIO_PinAFConfig(GPIOC, GPIO_PinSource8, GPIO_AF_DCMI);	//d2

//---------------------------------------------------------------------------------------
	// Configures the DCMI to interface with the OV7670 camera module
	// Enable DCMI clock
	RCC_AHB2PeriphClockCmd(RCC_AHB2Periph_DCMI, ENABLE);

	// Reinitialize
	DCMI_DeInit();

	// DCMI configuration
	DCMI_InitStructure.DCMI_CaptureMode = DCMI_CaptureMode_Continuous;
	DCMI_InitStructure.DCMI_SynchroMode = DCMI_SynchroMode_Hardware;
	DCMI_InitStructure.DCMI_PCKPolarity = DCMI_PCKPolarity_Rising;
	DCMI_InitStructure.DCMI_VSPolarity = DCMI_VSPolarity_High;		
	DCMI_InitStructure.DCMI_HSPolarity = DCMI_HSPolarity_Low;
	DCMI_InitStructure.DCMI_CaptureRate = DCMI_CaptureRate_All_Frame;
	DCMI_InitStructure.DCMI_ExtendedDataMode = DCMI_ExtendedDataMode_8b;

	DCMI_Init(&DCMI_InitStructure);
	
 
	// Configures the DMA2 to transfer Data from DCMI
	// Enable DMA2 clock
	RCC_AHB1PeriphClockCmd(DMA_Camera_STREAM_CLOCK, ENABLE);

	// DMA2 Stream1 Configuration
	DMA_DeInit(DMA_CameraToRAM_Stream);
	// Check if the DMA Stream is disabled before enabling it.
	// Note that this step is useful when the same Stream is used multiple times:
	// enabled, then disabled then re-enabled... In this case, the DMA Stream disable
	// will be effective only at the end of the ongoing data transfer and it will
	// not be possible to re-configure it before making sure that the Enable bit
	// has been cleared by hardware. If the Stream is used only once, this step might
	// be bypassed.
	while (DMA_GetCmdStatus(DMA_CameraToRAM_Stream) != DISABLE)
	{
	}
	DMA_InitStructure.DMA_Channel = DMA_Camera_Channel;
	DMA_InitStructure.DMA_PeripheralBaseAddr = DCMI_DR_ADDRESS;
//	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&RAM_Buffer;
	DMA_InitStructure.DMA_Memory0BaseAddr = 0x60020000;

	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
//	DMA_InitStructure.DMA_BufferSize = BuffSize;
	DMA_InitStructure.DMA_BufferSize = 1;

	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
//	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;

	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Enable;
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;

	DMA_Init(DMA_CameraToRAM_Stream, &DMA_InitStructure);

	// Enable DMA Stream Transfer Complete interrupt 
// 	DMA_ITConfig(DMA_CameraToRAM_Stream, DMA_IT_TC , ENABLE);

	// Check if the DMA Stream has been effectively enabled.
	// The DMA Stream Enable bit is cleared immediately by hardware if there is an
	// error in the configuration parameters and the transfer is no started (ie. when
	// wrong FIFO threshold is configured ...)
	Timeout = TIMEOUT_MAX;
	while ((DMA_GetCmdStatus(DMA_CameraToRAM_Stream) != ENABLE) && (Timeout-- > 0))
	{
	}

	// Check if a timeout condition occurred
	if (Timeout == 0)
	{
	  // Manage the error: to simplify the code enter an infinite loop
	  while (1)
	  {
		  // Dopísa program
	  }
	}
 
//	// Enable the DMA Stream IRQ Channel
//	NVIC_InitStructure.NVIC_IRQChannel = DMA_Camera_STREAM_IRQ;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStructure);

}



/**
  * @brief 	Turn on clock (XCLK from Camera Module) PA8 (MCO1),
  * 		Configure the PA8 as alternate function and as clock source for
  * 		MCO0 is used HSI (High Speed Internal RC oscillator)
  * @param	None
  * @retval	None
 */
void XCLK_ON(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	// Output HSI clock on MCO1 pin(PA8) ****************************************/
	// Enable the GPIOA peripheral
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	// Connect MCO1 pin to AF0
	// Connect to AF0 is default after reset
	// GPIO_PinAFConfig(GPIOA, GPIO_PinSource8, GPIO_AF_MCO);

	// Configure MCO1 pin(PA8) in alternate function
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	// HSI clock selected to output on MCO1 pin(PA8)
	RCC_MCO1Config(RCC_MCO1Source_HSI, RCC_MCO1Div_1);
}




/**
  * @brief 	Turn off clock (XCLK from Camera Module) PA8 (MCO0),
  * 		Configure the PA8 as output
  * @param	None
  * @retval	None
 */
void XCLK_OFF(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}



/**
  * @brief 	Read data form  SCCB/I2C interface
  * @param	None
  * @retval	Data: received data from SCCB/I2C interface
 */
uint8_t SCCB_Read(void)
{
	uint8_t Data, i;

	// Write to Data zero for correct return data
	Data = 0;
	// Configure SIO_D of SCCB/I2C interface as input for read
	SIO_D_Dir(IN);
	// Delay for  SCCB/I2C interface
	Delay_us(500);
	// Read data from SCCBI/I2C interface
	for(i=8;i>0;i--)
	{
		Delay_us(500);							// Delay for  SCCB/I2C
		SIO_C_Hi();								// Clock high on SIO_C
		Delay_us(500);
		Data = Data << 1;						// Rotate Data << 1
		if(SIO_D_STATE){Data = Data + 1;}		// Read SIO_D pin value
		SIO_C_Lo();								// Clock low on SIO_C
		Delay_us(500);
	}
	// Return received data from SCCBI/I2C interface
	return(Data);
}



/**
  * @brief  Write data on SCCB/I2C interface
  * @param  Data: data for write on SCCBI/I2C interface
  * @retval Ack: ERROR, SUCCESS
  */
ErrorStatus SCCB_Write(uint8_t Data)
{
	uint8_t i;
	ErrorStatus Ack;

	// Configure SIO_D of SCCB/I2C interface as output for write
	SIO_D_Dir(OUT);

	// Write data bit by bit on SCCB/I2C
	for(i=0;i<8;i++)
	{
		if((Data & 0x80) == 0x80)		// If bit in Data is high, write high on SCCB/I2C
		{
			SIO_D_Hi();
		}
		else							// If bit in Data is low, write low on SCCB/I2C
		{
			SIO_D_Lo();
		}
		Data <<= 1;						// Rotate Data for write next bit
		Delay_us(500);

		// Create clock pulse on SCCB/I2C
		// ___  On SIO_C pin (SCCB clock)
		//    \___
		SIO_C_Hi();
		Delay_us(500);
		SIO_C_Lo();
		Delay_us(500);
	}

	// Read acknowladge from Camera Module to confirm received data
	Delay_us(100);
	// Configure SIO_D of SCCB/I2C interface as input for read
	SIO_D_Dir(IN);
	Delay_us(500);
	SIO_C_Hi();
	Delay_us(500);
	// If acknowladge is OK return SUCCESS else if is incorrect return ERROR
	if(SIO_D_STATE){Ack = ERROR;}
	else {Ack = SUCCESS;}
	// Pulse on SCCB/I2C fall down from high
	SIO_C_Lo();
	Delay_us(500);
	// Configure SIO_D of SCCB/I2C interface back to output for write
	SIO_D_Dir(OUT);

	return (Ack);
}



/**
  * @brief  Create on  SCCB/I2C interface "START" condition of transmission
  * 	             |-> Start
  * 			  ___|
  * 		SIO_D    \______
  * 			  ______
  * 		SIO_C       \___
  * @param  None
  * @retval None
  */
static void SCCB_Start(void)
{
	SIO_D_Dir(OUT);		// Configure SIO_D of SCCB/I2C interface as output for write
	SIO_D_Hi();
	Delay_us(500);

	SIO_C_Hi();
	Delay_us(500);

	SIO_D_Lo();
	Delay_us(500);

	SIO_C_Lo();
	Delay_us(500);
}



/**
  * @brief  Create on  SCCB/I2C interface "STOP" condition of transmission
  * 	             |-> Stop
  * 				 |	 __
  * 		SIO_D ______/  \__
  * 			      ________
  * 		SIO_C ___/
  * @param  None
  * @retval None
  */
static void SCCB_Stop(void)
{
	SIO_D_Dir(OUT);		// Configure SIO_D of SCCB/I2C interface as output for write
	SIO_D_Lo();
	Delay_us(500);

	SIO_C_Hi();
	Delay_us(500);

	SIO_D_Hi();
	Delay_us(500);
}



/**
  * @brief  Create on SCCB/I2C interface "NoACK"
  * 	          _____
  * 		SIO_D 	   \___
  * 			  ___
  * 		SIO_C 	 \_____
  * @param  None
  * @retval None
  */
static void NoAck(void)
{
	SIO_D_Dir(OUT);		// Configure SIO_D of SCCB/I2C interface as output for write
	SIO_D_Hi();
	Delay_us(500);

	SIO_C_Hi();
	Delay_us(500);

	SIO_C_Lo();
	Delay_us(500);

	SIO_D_Lo();
	Delay_us(500);
}



/**
  * @brief  Delay for SCCB/I2C interface
  * @param  Time: time for delay
  * @retval None
  */
static void Delay_us(uint16_t Time)
{
   uint16_t i = 0;
   while(Time--)
   {
	   // Calculate for 168MHz sysclk
	   i = 5;	//72Mhz i = 10		35
	   while(i--) ;
   }
}



/**
  * @brief  Initializes the MCO1 (pin PA8) as XCLK for OV7670 camera module
  * @note	HSI (16MHz) is used as clock source for MCO1 directly (without prescaler)
  * @param  None
  * @retval ERROR if HSI startup is failed
  * 		SUCESS if HSI startup is correct
  */
static ErrorStatus OV7670_XCLK_Conf(void)
{
	__IO uint32_t startupcounter = 0;
	ErrorStatus status = ERROR;
	FlagStatus HSI_Status = RESET;


	GPIO_InitTypeDef  GPIO_InitStructure;

	// Enable high speed internal 16MHz oscillator *******************************
	RCC_HSICmd(ENABLE);

	// Wait till HSI is ready and if Time out is reached exit
	do
	{
		HSI_Status = RCC_GetFlagStatus(RCC_FLAG_HSIRDY);
	    startupcounter++;
	}while((startupcounter != HSI_STARTUP_TIMEOUT) && (HSI_Status == RESET));

	if (RCC_GetFlagStatus(RCC_FLAG_HSIRDY) != RESET)
	{
		status = SUCCESS;
	}
	else
	{
	    status = ERROR;
	}

	// Output HSI clock on MCO1 pin(PA8) ****************************************
	// Enable the GPIOA peripheral
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	// Connect MCO1 pin to AF0
	// Connect to AF0 is default after reset
	// GPIO_PinAFConfig(GPIOA, GPIO_PinSource8, GPIO_AF_MCO);

	// Configure MCO1 pin(PA8) in alternate function
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	// HSI clock selected to output on MCO1 pin(PA8)
	RCC_MCO1Config(RCC_MCO1Source_HSI, RCC_MCO1Div_1);

	return(status);
}
