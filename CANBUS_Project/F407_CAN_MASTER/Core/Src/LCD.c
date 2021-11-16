#include "LCD.h"
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#define I2C_SLAVE_ADDRESS  0x4E			/* User must write own slave device address */

extern I2C_HandleTypeDef hi2c1;			/* User must write own i2c typedef variable */

/**
  * @brief LCD Send Command Function
  * @param Command = LCD Instruction value
  * @retval None
  */

void LCD_SendCommand(uint8_t Command)
{
	uint8_t TempData[4];
	uint8_t UpperNibble = ( Command & 0xF0 );
	uint8_t LowerNibble = ( ( Command << 4 ) & 0xF0 );

	TempData[ 0 ] = UpperNibble | LCD_BACK_LIGHT_ON | LCD_E_ENABLE;
	TempData[ 1 ] = UpperNibble | LCD_BACK_LIGHT_ON;
	TempData[ 2 ] = LowerNibble | LCD_BACK_LIGHT_ON | LCD_E_ENABLE;
	TempData[ 3 ] = LowerNibble | LCD_BACK_LIGHT_ON;

	HAL_I2C_Master_Transmit(&hi2c1, I2C_SLAVE_ADDRESS, TempData, 4, 200);
}

/**
  * @brief LCD Send Data Function
  * @param Data = Sending value
  * @retval None
  */

void LCD_SendData(uint8_t Data)
{
	uint8_t TempData[4];
	uint8_t UpperNibble = ( Data & 0xF0 );
	uint8_t LowerNibble = ( ( Data << 4 ) & 0xF0 );

	TempData[ 0 ] = UpperNibble | LCD_BACK_LIGHT_ON | LCD_E_ENABLE | LCD_RS_ON;
	TempData[ 1 ] = UpperNibble | LCD_BACK_LIGHT_ON | LCD_RS_ON;
	TempData[ 2 ] = LowerNibble | LCD_BACK_LIGHT_ON | LCD_E_ENABLE  | LCD_RS_ON;
	TempData[ 3 ] = LowerNibble | LCD_BACK_LIGHT_ON | LCD_RS_ON;

	HAL_I2C_Master_Transmit(&hi2c1, I2C_SLAVE_ADDRESS, TempData, 4, 200);
}
/**
  * @brief LCD Clear Display Function
  * @param None
  * @retval None
  */
void LCD_Clear(void)
{
	LCD_SendCommand(LCD_CLEAR_DISPLAY);
	HAL_Delay(1);
}

/**
  * @brief LCD Initialization Function
  * @param None
  * @retval None
  */
void LCD_Init(void)
{

	/* 4-Bit Interface Initialization */

	HAL_Delay(50);				/* Wait for more than 15ms after VCC rises to 4.5 V */
	LCD_SendCommand(0x30);		/* According to datasheet Send Function set value   */
	HAL_Delay(5);				/* Wait for more than 4.1ms							*/
	LCD_SendCommand(0x30);		/* According to datasheet Send Function set value   */
	HAL_Delay(1);				/* Wait for more than 100us							*/
	LCD_SendCommand(0x30);
	HAL_Delay(10);
	LCD_SendCommand(0x20);
	HAL_Delay(10);

	/* Display Initialization */

	LCD_SendCommand(LCD_FUNCTION_SET | LCD_FUNCTION_N);		/* Select 4-bit operation, 2-line display and 5x8 dot character font */
	HAL_Delay(1);											/* According to datasheet, Wait for function set execution time		 */
	LCD_SendCommand(LCD_DISPLAY_CONTROL);					/* Select display off, cursor off and cursor blink off				 */
	HAL_Delay(1);											/* According to datasheet, Wait for display control execution time	 */
	LCD_SendCommand(LCD_CLEAR_DISPLAY);						/* Clear Display													 */
	HAL_Delay(3);
	LCD_SendCommand(LCD_ENTRY_MODE | LCD_ENTRY_ID);			/* Entry Mode I/D bit set, S bit reset so cursor moves to right		 */
	HAL_Delay(1);
	LCD_SendCommand(LCD_DISPLAY_CONTROL | LCD_DISPLAY_D);	/* LCD Display On 													 */
	HAL_Delay(3);

}

/**
  * @brief Set Cursor Position
  * @param row = row value must be 0 or 1. 0 = line 1, 1 = line 2
  * @param columns = columns value must be between 0 - 15.
  * @retval None
  */

void LCD_SetCursor(uint8_t row, uint8_t columns)
{
	uint8_t maskValue;
	uint8_t CursorPosition;
	maskValue = columns & 0x0F; 					/* This mask for if user write bigger than 15 value */

	switch(row)
	{
		case 0:
			CursorPosition = maskValue | 0x80; 		/* DDRAM address is 0x80 and Display address begin 0x00 so  we add to value to address and moves to cursor */
			break;
		case 1:
			CursorPosition = maskValue | 0xC0;		/* Display second line address is 0x40. İf we want to move on display, sum 0x80 and 0x40 therefore 0xC0     */
			break;
	}
	LCD_SendCommand(CursorPosition);
}

/**
  * @brief Set Cursor Position for 1-Line
  * @retval None
  */

void LCD_FirstLine(void)
{
	LCD_SetCursor(0, 0);
}

/**
  * @brief Set Cursor Position for 2-Line
  * @retval None
  */

void LCD_SecondLine(void)
{
	LCD_SetCursor(1, 0);
}

/**
  * @brief Select 2-Line or 1-Line Display Mode
  * @param LineModeState ONE_LINE or TWO_LINE
  * @retval None
  */

void LCD_LinesMode(LineModeState Mode)
{
	if( Mode == ONE_LINE )
	{
		LCD_SendCommand(LCD_FUNCTION_SET);
	}
	else
	{
		LCD_SendCommand(LCD_FUNCTION_SET | LCD_FUNCTION_N);
	}
}

/**
  * @brief Set Display setting Display,Cursor and Cursor Blink On or Off
  * @param FunctionalState Enable or Disable
  * @retval None
  */

void LCD_DisplayControl(FunctionalState Display, FunctionalState Cursor, FunctionalState Blink)
{
	if( Display == ENABLE )
	{
		if( Cursor == ENABLE)
		{
			if(Blink == ENABLE)
			{
				LCD_SendCommand(LCD_DISPLAY_CONTROL | LCD_DISPLAY_D | LCD_DISPLAY_C | LCD_DISPLAY_B);
			}
			else
			{
				LCD_SendCommand(LCD_DISPLAY_CONTROL | LCD_DISPLAY_D | LCD_DISPLAY_C);
			}
		}
		else
		{
			LCD_SendCommand(LCD_DISPLAY_CONTROL | LCD_DISPLAY_D);
		}
	}
	else
	{
		LCD_SendCommand(LCD_DISPLAY_CONTROL);
	}

}

/**
  * @brief  Shift Cursor and Display Right or Left
  * @param  ShiftState = Right or Left Shift
  * @param  Offset Value
  * @retval None
  */
void LCD_ShiftDisplay(ShiftState Move, uint8_t offset)
{
	if( Move == RIGHT_SHIFT)
	{
		for(size_t i = 0; i < offset ; i++)
		{
			LCD_SendCommand(LCD_CURSOR_SHIFT | LCD_CURSORDISPLAY_RIGHT);
		}
	}
	else
	{
		for(size_t i = 0; i < offset ; i++)
		{
			LCD_SendCommand(LCD_CURSOR_SHIFT | LCD_CURSORDISPLAY_LEFT);
		}
	}
}

/**
  * @brief  Print to LCD display (like a printf function)
  * @param  string value
  * @retval None
  */

void LCD_Printf(const char* parameterValue, ...)
{
	char Array[20];

	va_list argumans;
	va_start(argumans, parameterValue);
	vsprintf(Array, parameterValue, argumans);
	va_end(argumans);

	for(size_t i = 0; i < strlen(Array) && i < 16; i++)
	{
		LCD_SendData((uint8_t)Array[i]);
	}

}

/**
  * @brief Print to LCD display (like a printf function in C). This function is optional because same LCD_Printf function
  * @param string value
  * @retval None
  */
void LCD_Printf_String (char *str)
{
	while (*str)
	{
		LCD_SendData(*str++);
	}
}
