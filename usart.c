/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usart.c
  * @brief   This file provides code for the configuration
  *          of the USART instances.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "usart.h"

/* USER CODE BEGIN 0 */
#include "retarget.h"
/* USER CODE END 0 */

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart3;
extern uint8_t USART1_BUF[20];
extern uint8_t Control[4];
extern uint16_t reflext_clk;
extern uint16_t reflext_clk_angle;
extern uint16_t set_angle;
extern uint8_t buf[6];
extern uint8_t motor1;
extern uint8_t motor2;
extern uint8_t Flag;

/* USART1 init function */

void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}
/* USART3 init function */

void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspInit 0 */

  /* USER CODE END USART1_MspInit 0 */
    /* USART1 clock enable */
    __HAL_RCC_USART1_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* USART1 interrupt Init */
    HAL_NVIC_SetPriority(USART1_IRQn, 2, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspInit 1 */

  /* USER CODE END USART1_MspInit 1 */
  }
  else if(uartHandle->Instance==USART3)
  {
  /* USER CODE BEGIN USART3_MspInit 0 */

  /* USER CODE END USART3_MspInit 0 */
    /* USART3 clock enable */
    __HAL_RCC_USART3_CLK_ENABLE();

    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**USART3 GPIO Configuration
    PB10     ------> USART3_TX
    PB11     ------> USART3_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_11;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* USART3 interrupt Init */
    HAL_NVIC_SetPriority(USART3_IRQn, 2, 0);
    HAL_NVIC_EnableIRQ(USART3_IRQn);
  /* USER CODE BEGIN USART3_MspInit 1 */

  /* USER CODE END USART3_MspInit 1 */
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspDeInit 0 */

  /* USER CODE END USART1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART1_CLK_DISABLE();

    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9|GPIO_PIN_10);

    /* USART1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspDeInit 1 */

  /* USER CODE END USART1_MspDeInit 1 */
  }
  else if(uartHandle->Instance==USART3)
  {
  /* USER CODE BEGIN USART3_MspDeInit 0 */

  /* USER CODE END USART3_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART3_CLK_DISABLE();

    /**USART3 GPIO Configuration
    PB10     ------> USART3_TX
    PB11     ------> USART3_RX
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_10|GPIO_PIN_11);

    /* USART3 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART3_IRQn);
  /* USER CODE BEGIN USART3_MspDeInit 1 */

  /* USER CODE END USART3_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	uint8_t com_data;
	uint8_t i;
	static uint8_t RxCounter1 = 0;
	static uint8_t RxState = 0;
	static uint16_t RxBuffer1[6] = {0};
	if (huart == &huart1)
	{
		printf("%c\r\n",USART1_BUF[0]);
		HAL_UART_Receive_IT(&huart1, USART1_BUF, 1);
	}
	if (huart == &huart3)
	{
		com_data = buf[0];
		if(RxState==0&&com_data==0x2C)  //0x2c帧头
		{

			RxState=1;
			RxBuffer1[RxCounter1++]=com_data;
		}
		else if(RxState==1&&com_data==0x12)  //0x12帧头
		{
			RxState=2;
			RxBuffer1[RxCounter1++]=com_data;
		}
		else if(RxState==2)
		{
			RxBuffer1[RxCounter1++]=com_data;
			if(RxCounter1==6 && com_data == 0x5B)       //RxBuffer1接受满了,接收数据结束
			{

				motor1=RxBuffer1[RxCounter1-4];
				motor2=RxBuffer1[RxCounter1-3];
				Flag=RxBuffer1[RxCounter1-2];
				RxCounter1 = 0;
				RxState = 0;
			}
			else if(RxCounter1 > 6)            //接收异常
			{
				RxState = 0;
				RxCounter1=0;
				for(i=0;i<6;i++)
				{
					RxBuffer1[i]=0x00;      //将存放数据数组清零
				}
			}
		}
		else   //接收异常
		{
			RxState = 0;
			RxCounter1=0;
			for(i=0;i<6;i++)
			{
				RxBuffer1[i]=0x00;      //将存放数据数组清零
			}
		}
		HAL_UART_Receive_IT(&huart3, buf, 1);
	}
	reflext_clk_angle = 0;
	switch (USART1_BUF[0])
	{
		case '1':
			Control[0] = 0x01;
			Control[1] = 0x01;
			USART1_BUF[0] = 0;
			printf("前进\r\n");
			break;
		case '2':
			Control[0] = 0x10;
			Control[1] = 0x10;
			USART1_BUF[0] = 0;
			printf("后退\r\n");
			break;
		case '3':
			Control[0] = 0x10;
			Control[1] = 0x01;
			USART1_BUF[0] = 0;
			printf("左转\r\n");
			reflext_clk_angle = reflext_clk;
			break;
		case '4':
			Control[0] = 0x01;
			Control[1] = 0x10;
			USART1_BUF[0] = 0;
			printf("右转\r\n");
			reflext_clk_angle = reflext_clk;
			break;
		case '5':
			if (Control[2] == 3) {
				Control[2] = 3;
			}else {
				Control[2] ++;
			}
			USART1_BUF[0] = 0;
			printf("当前挡位%d\r\n",Control[2]);
			break;
		case '6':
			if (Control[2] == 0) {
				Control[2] = 0;
			}else {
				Control[2] --;
			}
			USART1_BUF[0] = 0;
			printf("当前挡位%d\r\n",Control[2]);
			break;
		case '7':
			Control[0] = 0x11;
			Control[1] = 0x11;
			USART1_BUF[0] = 0;
			printf("刹车\r\n");
			break;
		case 'B':
			Control[3] = 0x00;
			USART1_BUF[0] = 0;
			break;
		case 'T':
			Control[3] = 0x01;
			USART1_BUF[0] = 0;
			break;
		case 'V':
			Control[3] = 0x10;
			USART1_BUF[0] = 0;
		case 'P':
			if (set_angle == 180) {
				set_angle = 180;
			}else {
				set_angle += 30;
			}
			printf("当前设置旋转角度%d\r\n",set_angle);
			USART1_BUF[0] = 0;
			break;
		case 'S':
			if (set_angle == 0) {
				set_angle = 0;
			}else {
				set_angle -= 30;
			}
			printf("当前设置旋转角度%d\r\n",set_angle);
			USART1_BUF[0] = 0;
			break;
	}
}
/* USER CODE END 1 */
