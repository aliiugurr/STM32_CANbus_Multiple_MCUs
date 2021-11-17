#ifndef INC_LCD_H_
#define INC_LCD_H_

#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_i2c.h"

typedef enum
{
  ONE_LINE = 0U,
  TWO_LINE = !ONE_LINE
} LineModeState;

typedef enum
{
  LEFT_SHIFT  = 0U,
  RIGHT_SHIFT = !LEFT_SHIFT
} ShiftState;

/* Commands / Instruction */

#define LCD_CLEAR_DISPLAY				0x01
#define LCD_RETURN_HOME					0x02
#define LCD_ENTRY_MODE					0x04
#define LCD_DISPLAY_CONTROL				0x08
#define LCD_CURSOR_SHIFT				0x10
#define LCD_FUNCTION_SET				0x20
#define LCD_SETCGRAM_ADDRESS			0x40
#define LCD_SETDDRAM_ADDRESS			0x80

/* Entry Mode Set Bit Definition */

#define LCD_ENTRY_S						0x01
#define LCD_ENTRY_ID					0x02

/* Cursor or Display Shift Bit Definition */

#define LCD_CURSOR_LEFT					0x00	/* Cursor shift to the left	                                  	  */
#define LCD_CURSOR_RIGHT				0x04	/* Cursor shift to the right 								      */
#define LCD_CURSORDISPLAY_LEFT			0x08	/* Display shift to the left and cursor follow the display shift  */
#define LCD_CURSORDISPLAY_RIGHT			0x0C	/* Display shift to the right and cursor follow the display shift */


/* Function Set Bit Definition */

#define LCD_FUNCTION_DL					0x10 	/* LCD 8-bit Mode 												  */
#define LCD_FUNCTION_N					0x08 	/* LCD 2-Line Display 	                                          */
#define LCD_FUNCTION_F					0x04 	/* LCD 5x11 Data Format	                                          */

/* Display On/Off Control Bit Definition */

#define LCD_DISPLAY_B					0x01	/* LCD Cursor Blink ON	                                          */
#define LCD_DISPLAY_C					0x02	/* LCD Cursor ON		                                          */
#define LCD_DISPLAY_D					0x04	/* LCD Display ON		                                          */

/* I2C Control bits */

#define LCD_RS_ON        				(1 << 0)
#define LCD_RW_ON      					(1 << 1)
#define LCD_E_ENABLE   					(1 << 2)
#define LCD_BACK_LIGHT_ON  				(1 << 3)

/* Function Prototype */

void LCD_SendCommand(uint8_t Command);
void LCD_SendData(uint8_t Data);
void LCD_Clear(void);
void LCD_Init(void);
void LCD_SetCursor(uint8_t row, uint8_t columns);
void LCD_Printf(const char* parameterValue, ...);
void LCD_Printf_String (char *str);
void LCD_DisplayControl(FunctionalState Display, FunctionalState Cursor, FunctionalState Blink);
void LCD_LinesMode(LineModeState Mode);
void LCD_ShiftDisplay(ShiftState Move, uint8_t offset);
void LCD_SecondLine(void);
void LCD_FirstLine(void);

#endif /* INC_LCD_H_ */
