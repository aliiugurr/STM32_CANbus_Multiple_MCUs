#ifndef INC_DHT11_H_
#define INC_DHT11_H_

#include "stm32f4xx.h"
#include "stdint.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_tim.h"
#include <string.h>

typedef enum
{
	OUTPUT = 0U,
	INPUT  = !OUTPUT

}DHT_ModeState;

typedef struct
{
	uint8_t Temperature;
	uint8_t Humidity;

}DHT_InitTypeDef;

void DHT11_GPIO_Mode(DHT_ModeState Mode, GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
void DHT11_Init(DHT_InitTypeDef* DHT_InitStruct, TIM_HandleTypeDef *htim, GPIO_TypeDef* DHT_Port, uint16_t DHT_Pin);
void Delay_uS (TIM_HandleTypeDef *htim, uint16_t uS);
HAL_StatusTypeDef DHT11_Read(DHT_InitTypeDef *DHT_InitStruct, TIM_HandleTypeDef *htim, GPIO_TypeDef* DHT_Port, uint16_t DHT_Pin);


#endif /* INC_DHT11_H_ */
