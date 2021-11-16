/*
 * HCSR04.c
 *
 *  Created on: Oct 29, 2021
 *      Author: aliug
 */

#include "HCSR04.h"
/*
extern TIM_HandleTypeDef htim2;

uint32_t IC_Val1 = 0;
uint32_t IC_Val2 = 0;
uint32_t Difference = 0;
uint8_t First_Capture = 0;
uint8_t Distance  = 0;

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
	{
		if(First_Capture == 0)
		{
			IC_Val1 = HAL_TIM_ReadCapturedValue(&htim2, TIM_CHANNEL_1);
			First_Capture = 1;

			__HAL_TIM_SET_CAPTUREPOLARITY(&htim2,TIM_CHANNEL_1,TIM_INPUTCHANNELPOLARITY_FALLING);
		}

		else if( First_Capture == 1)
		{
			IC_Val2 = HAL_TIM_ReadCapturedValue(&htim2, TIM_CHANNEL_1);
			__HAL_TIM_SET_COUNTER(&htim2,0);
			First_Capture = 0;

			if(IC_Val2 > IC_Val1)
			{
				Difference = IC_Val2 - IC_Val1;
			}

			else
			{
				Difference = (0xFFFF - IC_Val1) + IC_Val2;
			}

			Distance = Difference * (0.034 / 2);

			__HAL_TIM_SET_CAPTUREPOLARITY(&htim2,TIM_CHANNEL_1,TIM_INPUTCHANNELPOLARITY_RISING);
			__HAL_TIM_DISABLE_IT(&htim2,TIM_IT_CC1);
		}

	}

}

*/
