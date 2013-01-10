/**
  ******************************************************************************
  * @file    SSD1289.c
  * @author  Juraj upravil
  * @original author AVRman
  * @version V1.0
  * @date    Jul 4, 2011
  * @brief	 Driver pre 3.2" TFT LCD Modul s SSD1289 kontrolérom
  ******************************************************************************

  ******************************************************************************
  */




/* Includes -------------------------------------------------- ----------------*/
#include "SSD1289.h"

/* Private typedef -------------------------------------------------- ---------*/
/* Private define -------------------------------------------------- ----------*/
/* Private macro -------------------------------------------------- -----------*/
/* Private variables -------------------------------------------------- -------*/
static uint16_t TextColor = Black, BackColor = White;

// Znak o (kruh 32x32) pre kalibráciu tocuh displeja
/*static uint8_t const Circle1[8][16] = {
	{0x00,0x00,0x00,0x00,0x01,0x03,0x07,0x0F,0x1F,0x1F,0x3F,0x3F,0x3F,0x7F,0x7F,0x7F},
	{0x00,0x03,0x1F,0xFF,0xFF,0xFF,0xFF,0xFF,0xFC,0xF0,0xE0,0xC0,0xC0,0x80,0x00,0x00},
	{0x00,0xC0,0xF8,0xFF,0xFF,0xFF,0xFF,0xFF,0x3F,0x0F,0x07,0x03,0x03,0x01,0x00,0x00},
	{0x00,0x00,0x00,0x00,0x80,0xC0,0xE0,0xF0,0xF8,0xF8,0xFC,0xFC,0xFC,0xFE,0xFE,0xFE},
	{0x7F,0x7F,0x7F,0x3F,0x3F,0x3F,0x1F,0x1F,0x0F,0x07,0x03,0x01,0x00,0x00,0x00,0x00},
	{0x00,0x00,0x80,0xC0,0xC0,0xE0,0xF0,0xFC,0xFF,0xFF,0xFF,0xFF,0xFF,0x1F,0x03,0x00},
	{0x00,0x00,0x01,0x03,0x03,0x07,0x0F,0x3F,0xFF,0xFF,0xFF,0xFF,0xFF,0xF8,0xC0,0x00},
	{0xFE,0xFE,0xFE,0xFC,0xFC,0xFC,0xF8,0xF8,0xF0,0xE0,0xC0,0x80,0x00,0x00,0x00,0x00},
};
*/
static uint8_t const Circle2[2][16] = {
	{0x00,0x07,0x07,0x1F,0x1F,0x7C,0x78,0x78,0x78,0x78,0x7C,0x1F,0x1F,0x07,0x07,0x00},
	{0x00,0xE0,0xE0,0xF8,0xF8,0x3E,0x1E,0x1E,0x1E,0x1E,0x3E,0xF8,0xF8,0xE0,0xE0,0x00},
};

/* Private function prototypes -----------------------------------------------*/
static __inline void LCD_WriteReg(uint8_t LCD_Reg, uint16_t LCD_RegValue);
static void LCD_Delay(uint16_t nCount);
static __inline uint16_t LCD_ReadReg(uint8_t LCD_Reg);
static void LCD_InitBackLight(void);
static void LCD_FSMCConfig(void);
static void LCD_FSMCConfig(void);

/* Private functions -------------------------------------------------- -------*/

  
/**
  * @brief 	Config GPIO(FSMC) for LCD Module			 
  * @param	None
  * @retval	None
 */
static void LCD_PinConfig(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	// Configures the FSMC GPIOs to interface with the LCD module
	// Enable GPIOD, GPIOE, GPIOF, GPIOG and AFIO clocks 
   	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOG | 
						   RCC_AHB1Periph_GPIOE | RCC_AHB1Periph_GPIOF, 
						   ENABLE);

	// Enable FSMC, GPIOD, GPIOE and AFIO clocks
	RCC_AHB3PeriphClockCmd(RCC_AHB3Periph_FSMC, ENABLE);

	//-- GPIO Configuration ------------------------------------------------------------
   	// SRAM Data lines,  NOE and NWE configuration 
	// Connect FSMC pins to AF12
	// +-------------------+--------------------+------------------+-------------------+
    // +                       LCD pins assignment                                     +
    // +-------------------+--------------------+------------------+-------------------+
    // | PD0  <-> FSMC_D2  | PD8   <-> FSMC_D13 | PD15 <-> FSMC_D1 | PE11 <-> FSMC_D8  |
    // | PD1  <-> FSMC_D3  | PD9   <-> FSMC_D14 | PE7  <-> FSMC_D4 | PE12 <-> FSMC_D9  |
    // | PD4  <-> FSMC_NOE | PD10  <-> FSMC_D15 | PE8  <-> FSMC_D5 | PE13 <-> FSMC_D10 |
    // | PD5  <-> FSMC_NWE | PD11  <-> FSMC_A16 | PE9  <-> FSMC_D6 | PE14 <-> FSMC_D11 |
    // | PD7  <-> FSMC_NE1 | PD14  <-> FSMC_D0  | PE10 <-> FSMC_D7 | PE15 <-> FSMC_D12 |
  	// +-------------------+--------------------+------------------+-------------------+

	// PORTD
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource0, GPIO_AF_FSMC);			// D2
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource1, GPIO_AF_FSMC);			// D3
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource4, GPIO_AF_FSMC);			// NOE -> RD
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource5, GPIO_AF_FSMC);			// NWE -> WR
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource7, GPIO_AF_FSMC);			// NE1 -> CS
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource8, GPIO_AF_FSMC);			// D13
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource9, GPIO_AF_FSMC);			// D14
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource10, GPIO_AF_FSMC);		// D15
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource11, GPIO_AF_FSMC);		// A16 -> RS
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource14, GPIO_AF_FSMC);		// D0
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource15, GPIO_AF_FSMC);		// D1

	// PORTE
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource7, GPIO_AF_FSMC);			// D4
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource8, GPIO_AF_FSMC);			// D5
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource9, GPIO_AF_FSMC);			// D6
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource10, GPIO_AF_FSMC);		// D7
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource11, GPIO_AF_FSMC);		// D8
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource12, GPIO_AF_FSMC);		// D9
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource13, GPIO_AF_FSMC);		// D10
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource14, GPIO_AF_FSMC);		// D11
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource15, GPIO_AF_FSMC);		// D12

	// FSMC GPIO configuration
	// SRAM Data lines,  NOE and NWE configuration 
	// PORTD
   	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_5 |
                                  GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 |
                                  GPIO_Pin_11 | GPIO_Pin_14 | GPIO_Pin_15;
   	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
   	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
   	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
   	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;

	GPIO_Init(GPIOD, &GPIO_InitStructure);

	// PORTE
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 |
                                  GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 |
                                  GPIO_Pin_15;
   	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
   	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
   	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
   	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;

	GPIO_Init(GPIOE, &GPIO_InitStructure);
}



/**
  * @brief 	Config FSMC Parallel interface for LCD Module
  * @param	None
  * @retval	None
 */
static void LCD_FSMCConfig(void)
{
	FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
	FSMC_NORSRAMTimingInitTypeDef FSMC_NORSRAMTimingInitStructure;

	// HCLK div1 with 168Mhz SYSCLK (1/168 000 000 = 5,952 ns)
	FSMC_NORSRAMTimingInitStructure.FSMC_AddressSetupTime = 0;  
  	FSMC_NORSRAMTimingInitStructure.FSMC_AddressHoldTime = 0;	   
  	FSMC_NORSRAMTimingInitStructure.FSMC_DataSetupTime = 3;	   
  	FSMC_NORSRAMTimingInitStructure.FSMC_BusTurnAroundDuration = 0;
  	FSMC_NORSRAMTimingInitStructure.FSMC_CLKDivision = 1;
  	FSMC_NORSRAMTimingInitStructure.FSMC_DataLatency = 0;
  	FSMC_NORSRAMTimingInitStructure.FSMC_AccessMode = FSMC_AccessMode_A;

	FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM1;
	FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;
	FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_SRAM;
	FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;
	FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
	FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;
	FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;
	FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;
	FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait = FSMC_AsynchronousWait_Disable;
	FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;
	FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &FSMC_NORSRAMTimingInitStructure;

	FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);

	FSMC_NORSRAMTimingInitStructure.FSMC_AddressSetupTime = 0;   
 	FSMC_NORSRAMTimingInitStructure.FSMC_AddressHoldTime = 0;	   
  	FSMC_NORSRAMTimingInitStructure.FSMC_DataSetupTime = 3;	   
  	FSMC_NORSRAMTimingInitStructure.FSMC_BusTurnAroundDuration = 0;
  	FSMC_NORSRAMTimingInitStructure.FSMC_CLKDivision = 1;
  	FSMC_NORSRAMTimingInitStructure.FSMC_DataLatency = 0;
  	FSMC_NORSRAMTimingInitStructure.FSMC_AccessMode = FSMC_AccessMode_A;	
  	FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &FSMC_NORSRAMTimingInitStructure;

	FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);

    // Enable FSMC Bank1 SRAM Bank
    FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM1, ENABLE);
}



/**
  * @brief 	Initialize LCD Module with SSD1289 controller 
  * @param	None
  * @retval	None
 */
void LCD_Init_SSD1289(void)
{
	// Config GPIO(FSMC) for LCD Module
	LCD_PinConfig();
	// Config FSMC Parallel interface for LCD Module
	LCD_FSMCConfig();
	// Initialize PWM & GPIO for BackLight and brightness on 100%
	//LCD_InitBackLight();
	//LCD_BackLight(100);

	LCD_Delay(5);

	// Turn ON LCD Module
	LCD_SetFunctionalState(ENABLE);

	LCD_WriteReg(0x0003,0xA8A4);    LCD_Delay(5);
	LCD_WriteReg(0x000C,0x0000);    LCD_Delay(5);
	LCD_WriteReg(0x000D,0x080C);    LCD_Delay(5);
    LCD_WriteReg(0x000E,0x2B00);    LCD_Delay(5);
    LCD_WriteReg(0x001E,0x00B0);    LCD_Delay(5);

    LCD_WriteReg(0x0001,0x2b3F);    LCD_Delay(5);   // 0x2B3F RGB mode	BGR 233f
    LCD_WriteReg(0x0005,0x0000);    LCD_Delay(5);
    LCD_WriteReg(0x0006,0x0000);    LCD_Delay(5);
    LCD_WriteReg(0x0016,0xEF1C);    LCD_Delay(5);
    LCD_WriteReg(0x0017,0x0003);    LCD_Delay(5);

    LCD_WriteReg(0x000B,0x0000);    LCD_Delay(5);
    LCD_WriteReg(0x000F,0x0000);    LCD_Delay(5);   // Gate Scan Position 0-319
    LCD_WriteReg(0x0041,0x0000);    LCD_Delay(5);
    LCD_WriteReg(0x0042,0x0000);    LCD_Delay(5);
    LCD_WriteReg(0x0048,0x0000);    LCD_Delay(5);

    LCD_WriteReg(0x0049,0x013F);    LCD_Delay(5);
    LCD_WriteReg(0x004A,0x0000);    LCD_Delay(5);
    LCD_WriteReg(0x004B,0x0000);    LCD_Delay(5);
    LCD_WriteReg(0x0044,0xEF00);    LCD_Delay(5);
    LCD_WriteReg(0x0045,0x0000);    LCD_Delay(5);

    LCD_WriteReg(0x0046,0x013F);    LCD_Delay(5);
    LCD_WriteReg(0x0030,0x0707);    LCD_Delay(5);
    LCD_WriteReg(0x0031,0x0204);    LCD_Delay(5);
    LCD_WriteReg(0x0032,0x0204);    LCD_Delay(5);
    LCD_WriteReg(0x0033,0x0502);    LCD_Delay(5);

    LCD_WriteReg(0x0034,0x0507);    LCD_Delay(5);
    LCD_WriteReg(0x0035,0x0204);    LCD_Delay(5);
    LCD_WriteReg(0x0036,0x0204);    LCD_Delay(5);
    LCD_WriteReg(0x0037,0x0502);    LCD_Delay(5);
    LCD_WriteReg(0x003A,0x0302);    LCD_Delay(5);

    LCD_WriteReg(0x003B,0x0302);    LCD_Delay(5);
    LCD_WriteReg(0x0023,0x0000);    LCD_Delay(5);
    LCD_WriteReg(0x0024,0x0000);    LCD_Delay(5);
    LCD_WriteReg(0x0025,0x8000);    LCD_Delay(5);
    LCD_WriteReg(0x004f,0);
    LCD_WriteReg(0x004e,0);


    // Kalibrácia tocuhscreen kontroláru
//    LCD_Clear(White);
//    ADS7843_Calibrate();
}



/**
  * @brief 	Set LCD Module operation condition
  * @param	NewState: New state of operation for LCD Module
  * 				  where it cen be: ENABLE, DISABLE, SLEEP
  * @retval	None
 */
void LCD_SetFunctionalState(FunctionalState NewState)
{
	// Zapne LCD
	if(NewState == ENABLE);
	{
		// R07h = 0021h (GON = 1,DTE = 0,D[1:0] = 01)
		LCD_WriteReg(0x0007, 0x0021);
		// R00h = 0001h (OSCEN=1)
		LCD_WriteReg(0x0000, 0x0001);
		// R07h = 0023h (GON = 1,DTE = 0,D[1:0] = 11)
		LCD_WriteReg(0x0007, 0x0023);
		// R10h = 0000h (Exit sleep mode)
		LCD_WriteReg(0x0010, 0x0000);
		// Delay 30ms
		LCD_Delay(3);
		// R07h = 0033h (GON = 1,DTE = 1,D[1:0] = 11)
		LCD_WriteReg(0x0007, 0x0033);
		// R11H = 0x6070 Entry mode
		// vsmode DFM1 DFM0 TRANS OEDef WMode DMode1 DMode0 TY1 TY0 ID1 ID0 AM LG2 LG2 LG0
		//   0     1    1     0     0     0     0      0     0   1   1   1   1  0   0   0
		LCD_WriteReg(0x0011, 0x6028);  //6068
		// R02h = 0x0600 LCD driver AC setting
		LCD_WriteReg(0x0002, 0x0600);
	}

	// Turn OFF LCD Module
	if(NewState == DISABLE)
	{

	}

	// Sleep mode on LCD Module
	if(NewState == SLEEP)
	{

	}
}



/**
  * @brief 	Initialize LCD Module Backlight
  * @param	None
  * @retval	None
 */
static void LCD_InitBackLight(void)
{
/*
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO , ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

	// GPIOB:  PB5(TIM3 CH2) alternate function push-pull
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_5 ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE);


	// TIM3CLK = 36 MHz, Prescaler = 35, TIM3 counter clock = 1 MHz
	// TIM3 ARR Register = 999 => TIM3 Frequency = TIM3 counter clock/(ARR + 1)
	// TIM3 Frequency = 1 KHz.
	// TIM3 Channel2 duty cycle = (TIM3_CCR2/ TIM3_ARR)* 100
	// 1MHz TIM3 counter clock = 1MHz
	TimerPeriod = (uint16_t) (SystemCoreClock / 1000000) - 1;

	// 1Mhz/(999 + 1 )=1kHz
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseStructure.TIM_Prescaler = TimerPeriod;
	TIM_TimeBaseStructure.TIM_Period = 999;   					// TIM3 ARR Register
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);


	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = Channel2Pulse;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);

	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);

	TIM_ARRPreloadConfig(TIM3, ENABLE);
	TIM_Cmd(TIM3, ENABLE);
*/
}




/**
  * @brief 	Set LCD Module backlight from 0 to 100%
  * @param	Percent: 0-100(%) from brighntess of backlight 
  * @retval	None
 */
void LCD_BackLight(uint8_t Percent)
{

}



/**
  * @brief 	Write data to the specific register of LCD Module
  * @param	LCD_Reg: Address of register
  * 		LCD_RegValue: Value of register
  * @retval	None
 */
static __inline void LCD_WriteReg(uint8_t LCD_Reg, uint16_t LCD_RegValue)
{
	// Put data from LCD_Reg to LCD_REG
	LCD_REG = LCD_Reg;
	// Write data from LCD_RegValue 16-bit register to LCD(RAM-REG)
	LCD_RAM = LCD_RegValue;
}



/**
  * @brief 	Read data from specific register from LCD Module
  * @param	LCD_Reg: Address of register
  * @retval: Value of register
 */
static __inline uint16_t LCD_ReadReg(uint8_t LCD_Reg)
{
	// Vloži hodnotu LCD_Reg do LCD_REG
	LCD_REG = LCD_Reg;
	// Preèita hodnotu registra LCD
	return (LCD_RAM);
}




/**
  * @brief 	Prepare LCD Module to write continous data to the RAM
  * @param	None
  * @retval	None
 */
__inline void LCD_WriteRAM_Prepare(void)
{
	LCD_REG = 0x0022;	//R34 =0x0022h
}




/**
  * @brief 	Write data to the RAM of LCD Module
  * @param	RGB_Code: color of the pixel in RGB565 form
  * @retval	None
 */
__inline void LCD_WriteRAM(uint16_t RGB_Code)
{
	// Put data into 16-bit GRAM register
	LCD_RAM = RGB_Code;
}



/**
  * @brief 	Set cursor to the specific X,Y coordination
  * @param	PosX: row (0-239)
  * 		Posy: column (0-319)
  * @retval	None
 */
void LCD_SetCursor(uint16_t PosX,uint16_t PosY)
{
	LCD_WriteReg(0x004e,PosX);		// Row
	LCD_WriteReg(0x004f,PosY); 		// Column
}



/**
  * @brief 	Clear the LCD Module with specific color (Background color)
  * @param	Color: Color of the background
  * @retval	None
 */
void LCD_Clear(uint16_t Color)
{
	uint32_t i;

	LCD_SetCursor(0,0);
	LCD_WriteRAM_Prepare();

	for(i=0; i<76800; i++)
	{
		LCD_RAM = Color;
	}
}



/**
  * @brief 	Set the specific pixel on LCD Module with choossen color
  * @param	Color: Color of the pixel
  * @retval	None
 */
void LCD_SetPoint(uint16_t PosX, uint16_t PosY, uint16_t Color)
{
	if (( PosX > 239 ) || ( PosY > 319 )) return;
	LCD_SetCursor(PosX, PosY);
	LCD_WriteRAM_Prepare();
	LCD_WriteRAM(Color);
}




/**
  * @brief 	Put ASCII character on the specific coordination on LCD Module
  * @param	PosX: X - Row (X coordination)
  * 		PosY: Y - Column (Y coordination)
  * 		c: ASCII character from table
  * 		CharColor: Text color
  * 		BackColor: Background color
  * @retval	None
 */
void LCD_PutChar(uint16_t PosX, uint16_t PosY, unsigned char c, uint16_t CharColor,
				 uint16_t BackColor)
{
	uint8_t i = 0;
	uint8_t j = 0;
	uint8_t Buffer[16];
	uint8_t TmpChar = 0;

	GetASCIICode(Buffer,c);

	for (i=0;i<16;i++)
	{
		TmpChar = Buffer[i];
		for (j=0;j<8;j++)
		{
			if ( ((TmpChar >> (7-j)) & 0x01) == 0x01)
			{
				LCD_SetPoint(PosX + j, PosY + i, CharColor);
			}
			else
			{
				LCD_SetPoint(PosX + j, PosY + i, BackColor);
			}
		}
	}
}




/**
  * @brief 	Put text into specific coordination of LCD Module
  * @param	PosX: X - Text start X coordination (Row) 
  * 		PosY: Y - Text start Y coordination (Column)
  * 		*str: pointer on text(string)
  * 		TextColor: Text color
  * 		BackColor: Background color
  * @retval	None
 */
void LCD_GUI_Text(uint16_t PosX, uint16_t PosY, uint8_t *str,
			  	  uint16_t TextColor, uint16_t BackColor)
{
	uint8_t TempChar;

	do
	{
		TempChar = *str++;
		LCD_PutChar(PosX, PosY, TempChar, TextColor, BackColor);

		if (PosX < 232)
		{
			PosX += 8;
		}
		else if (PosY < 304)
		{
			PosX = 0;
			PosY += 16;
		}
		else
		{
			PosX = 0;
			PosY = 0;
		}
	}
	while (*str != 0);
}




/**
  * @brief 	Vykresli kruh na dané miesto na LCD
  * @param	None
  * @retval	PosX, PosY: X a suradnica zaèiatku kruhu
 */
void LCD_DrawCalibPoint(uint16_t PosX, uint16_t PosY)
{
	uint8_t i, j, l, TempChar;

	LCD_Clear(White);
	LCD_GUI_Text(40,120,"Kalibracia dotykovej", TextColor, BackColor);
	LCD_GUI_Text(84,140,"obrazovky", TextColor, BackColor);
	LCD_GUI_Text(20,180,"STLAC KRUH PRE KALIBRACIU", TextColor, BackColor);
	LCD_GUI_Text(20,200,"A POTVRD TLACIDLOM 'KEYA'", TextColor, BackColor);
/*
	// Vykreslenie kruhu (Circle1) pre kalibraciu touch kontroléru
	for(k=0;k<2;k++)
	{
		for(l=0;l<4;l++)
		{
			for (i=0;i<16;i++)
			{
				pom = ((k + 1)*(l + 1)) - 1;
				if(k == 1)
					pom = 4 + l;
				TempChar = Circle1[pom][i];
				for (j=0;j<8;j++)
				{
					if ( ((TempChar >> (7-j)) & 0x01) == 0x01)
					{
						LCD_SetPoint(PosX+j+(l*8), PosY+i+(k*16), Red);
					}
					else
					{
						LCD_SetPoint(PosX+j+(l*8), PosY+i+(k*16), White);
					}
				}
			}
		}
	}
*/
	// Vykreslenie kruhu (Circle2) pre kalibraciu touch kontroléru
	for(i=0;i<2;i++)
	{
		for(l=0;l<16;l++)
		{
			TempChar = Circle2[i][l];
			for (j=0;j<8;j++)
			{
				if ( ((TempChar >> (7-j)) & 0x01) == 0x01)
				{
					LCD_SetPoint(PosX+j+(i*8), PosY+l, Red);
				}
				else
				{
					LCD_SetPoint(PosX+j+(i*8), PosY+l, White);
				}
			}
		}
	}
}




/**
  * @brief 	Delay for LCD Module
  * @param	nCount: Number of counts to wait
  * @retval	None
 */
static void LCD_Delay(uint16_t nCount)
{
	uint16_t TimingDelay;
	while(nCount--)
	{
		for(TimingDelay=0;TimingDelay<60000;TimingDelay++);	 //24000
	}
}
