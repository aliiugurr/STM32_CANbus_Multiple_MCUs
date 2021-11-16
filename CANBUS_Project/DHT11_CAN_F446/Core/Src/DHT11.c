#include "DHT11.h"

/**
  * @brief GPIO Mode Initialization Function
  * @param DHT_ModeState Mode Selected, OUTPUT or INPUT
  * @param DHT_Port = GPIOx where x can be (A..K) to select the GPIO peripheral for STM32F429X device or
  *                      x can be (A..I) to select the GPIO peripheral for STM32F40XX and STM32F427X devices.
  * @param DHT_Pin  = GPIO_PIN_x  x can be (0..15) to select the GPIO pin
  * @retval None
  */

void DHT11_GPIO_Mode(DHT_ModeState Mode, GPIO_TypeDef *DHT_Port, uint16_t DHT_Pin)
{
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };

	if( Mode == OUTPUT)
	{
		  GPIO_InitStruct.Pin = DHT_Pin;
		  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		  GPIO_InitStruct.Pull = GPIO_NOPULL;
		  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

		HAL_GPIO_Init(DHT_Port, &GPIO_InitStruct);
	}
	else if( Mode == INPUT)
	{
		GPIO_InitStruct.Pin = DHT_Pin;
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

		HAL_GPIO_Init(DHT_Port, &GPIO_InitStruct);
	}
}

/**
  * @brief DHT11 Initialization Function
  * @param DHT_InitStruct DHT Base Handle
  * @param htim TIM Base handle
  * @param DHT_Port = GPIOx where x can be (A..K) to select the GPIO peripheral for STM32F429X device or
  *                      x can be (A..I) to select the GPIO peripheral for STM32F40XX and STM32F427X devices.
  * @param DHT_Pin  = GPIO_PIN_x  x can be (0..15) to select the GPIO pin
  * @retval None
  */

void DHT11_Init(DHT_InitTypeDef* DHT_InitStruct, TIM_HandleTypeDef *htim, GPIO_TypeDef *DHT_Port, uint16_t DHT_Pin)
{

	DHT11_GPIO_Mode(OUTPUT, DHT_Port, DHT_Pin);
	HAL_GPIO_WritePin(DHT_Port, DHT_Pin, GPIO_PIN_RESET);
	Delay_uS(htim,18000);
	HAL_GPIO_WritePin(DHT_Port, DHT_Pin, GPIO_PIN_SET);
	Delay_uS(htim,40);
	DHT11_GPIO_Mode(INPUT, DHT_Port, DHT_Pin);
}

/**
  * @brief DHT11 Read Value Function
  * @param DHT_InitStruct DHT Base Handle
  * @param htim TIM Base handle
  * @param DHT_Port = GPIOx where x can be (A..K) to select the GPIO peripheral for STM32F429X device or
  *                      x can be (A..I) to select the GPIO peripheral for STM32F40XX and STM32F427X devices.
  * @param DHT_Pin  = GPIO_PIN_x  x can be (0..15) to select the GPIO pin
  * @retval None
  */

HAL_StatusTypeDef DHT11_Read(DHT_InitTypeDef *DHT_InitStruct, TIM_HandleTypeDef *htim, GPIO_TypeDef* DHT_Port, uint16_t DHT_Pin)
{
	uint16_t Time = 0;
	uint8_t bits[5] = {0};
	uint8_t sum = 0;
	uint8_t cnt = 7;
	uint8_t idx = 0;


	/* EMPTY BUFFER */
	memset(bits, 0, 5);

	/* Start timer for Delay_uS function */
	HAL_TIM_Base_Start(htim);

	/* REQUEST SAMPLE */
	DHT11_Init(DHT_InitStruct, htim, DHT_Port, DHT_Pin);

	/* ACKNOWLEDGE or TIMEOUT */
	uint16_t loopCnt = 10000;
	while(HAL_GPIO_ReadPin(DHT_Port, DHT_Pin) == GPIO_PIN_RESET)
		if(loopCnt-- == 0) return HAL_TIMEOUT;

	loopCnt = 10000;
	while(HAL_GPIO_ReadPin(DHT_Port, DHT_Pin) == GPIO_PIN_SET)
		if(loopCnt-- == 0) return HAL_TIMEOUT;

	/* READ OUTPUT - 40 BITS => 5 BYTES or TIMEOUT */
	for(size_t i = 0; i < 40; i++)
	{
		loopCnt = 10000;
		while(HAL_GPIO_ReadPin(DHT_Port, DHT_Pin) == GPIO_PIN_RESET)
			if(loopCnt-- == 0) return HAL_TIMEOUT;

		__HAL_TIM_SET_COUNTER(htim,0);			/* Set 0 counter value */
		Time = __HAL_TIM_GET_COUNTER(htim);		/* Get Counter value   */

		loopCnt = 10000;
		while(HAL_GPIO_ReadPin(DHT_Port, DHT_Pin) == GPIO_PIN_SET)
			if(loopCnt-- == 0) return HAL_TIMEOUT;

		if(__HAL_TIM_GET_COUNTER(htim) - Time > 40) bits[idx] |= (1 << cnt);
		if(cnt == 0)
		{
			cnt = 7;
			idx++;
		}
		else
			cnt--;
	}

	DHT_InitStruct->Humidity    = bits[0];
	DHT_InitStruct->Temperature = bits[2];

	sum = bits[0]+ bits[2];
	if(bits[4] != sum) return HAL_ERROR;
	return HAL_OK;
}

/**
  * @brief Microsecond and Milisecond Delay Function
  * @param htim TIM Base handle
  * @param uS  = Second Value for example; for 18uS = Delay_uS(&htimx, 18) for 18ms = Delay_uS(&htimx, 18000)
  * @retval None
  */

void Delay_uS (TIM_HandleTypeDef *htim, uint16_t uS)
{
	HAL_TIM_Base_Start(htim);
	__HAL_TIM_SET_COUNTER(htim,0); 				/* Set the TIM Counter value is 0 							   */
	while( __HAL_TIM_GET_COUNTER(htim) < uS);		/* wait for the counter to reach the us input in the parameter */
}


