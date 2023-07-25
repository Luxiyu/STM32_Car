/*
 * Control.c
 *
 *  Created on: 2023年7月11日
 *      Author: luxy
 */
#include "Control.h"
#include "retarget.h"

extern uint8_t Data[3];
extern uint8_t Mode;
extern TIM_HandleTypeDef htim2;
extern uint16_t reflext_clk;
extern uint16_t reflext_clk_angle;
extern uint16_t set_angle;
uint8_t reflext_data[5] = {1,1,1,1,1}; //存放红外的检测数据，1为没检测到，0为检测到。
extern uint8_t motor1;
extern uint8_t motor2;
extern uint8_t Flag;

void CarDataAnalysis(uint8_t Data[4]){
	if(Mode!=Data[3]){
		if (Data[3]==0x00) {
			printf("Bluetooth Mode\r\n");
		}
		if (Data[3]==0x01) {
			printf("Patrol Mode\r\n");
		}
		if (Data[3]==0x10) {
			printf("Vision Track Mode\r\n");
		}
	}
	Mode = Data[3];
	switch (Data[3]) {
		case 0x00:
	//		printf("遥控模式\r\n");
			Bluetooth_mode(Data);
			break;
		case 0x01:
	//		printf("巡线模式\r\n");
			Track_mode(Data);
			break;
		case 0x10:
			Vision_mode(Data);

	}
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)//输入中断处理函数
{
	switch (GPIO_Pin) {
		case DO1_Pin:
			 if(HAL_GPIO_ReadPin(DO1_GPIO_Port, DO1_Pin))//上升沿
			{
				reflext_data[0] = 1;
			}
			if(!HAL_GPIO_ReadPin(DO1_GPIO_Port, DO1_Pin))//下降沿
			{
				reflext_data[0] = 0;
			}
			break;
		case DO2_Pin:
			if(HAL_GPIO_ReadPin(DO2_GPIO_Port, DO2_Pin))//上升沿
			{
				reflext_data[1] = 1;
			}
			if(!HAL_GPIO_ReadPin(DO2_GPIO_Port, DO2_Pin))//下降沿
			{
				reflext_data[1] = 0;
			}
			break;
		case DO3_Pin:
	        if(HAL_GPIO_ReadPin(DO3_GPIO_Port, DO3_Pin))//上升沿
	        {
	        	reflext_data[2] = 1;
	        }
	        if(!HAL_GPIO_ReadPin(DO3_GPIO_Port, DO3_Pin))//下降沿
	        {
	        	reflext_data[2] = 0;
	        }
			break;
		case DO4_Pin:
			if(HAL_GPIO_ReadPin(DO4_GPIO_Port, DO4_Pin))//上升沿
			{
				reflext_data[3] = 1;
			}
			if(!HAL_GPIO_ReadPin(DO4_GPIO_Port, DO4_Pin))//下降沿
			{
				reflext_data[3] = 0;
			}
			break;
		case DO5_Pin:
	        if(HAL_GPIO_ReadPin(DO5_GPIO_Port, DO5_Pin))//上升沿
	        {
	        	reflext_data[4] = 1;
	        }
	        if(!HAL_GPIO_ReadPin(DO5_GPIO_Port, DO5_Pin))//下降沿
	        {
	        	reflext_data[4] = 0;
	        }
			break;
		case DO6_Pin:
			if(HAL_GPIO_ReadPin(DO6_GPIO_Port, DO6_Pin))//上升沿
			{
				reflext_clk ++ ;
			}
			break;
	}
}

//void reflect_data_output(void)  //将探测情况输出
//{
//    uint8_t i = 0;
//
//    for(i = 0;i < 5;i++)
//
//    {
//        if(reflext_data[i] == 0) printf("-0-");
//        else if(reflext_data[i] == 1) printf("-1-");
//        else {};
//    }
//    printf("\r\n");
//}


void Bluetooth_mode(uint8_t Data[4]){
//	printf("遥控模式\r\n");
	uint8_t Speed;
	switch (Data[2]) {
		case 0:
			Speed = 0;
			break;
		case 1:
			Speed = 35;
			break;
		case 2:
			Speed = 60;
			break;
		case 3:
			Speed = 90;
			break;
	}
//	printf("Clk:%d\r\n",reflext_clk);
//	printf("行驶的距离:%f cm\r\n",reflext_clk*1.03);
	switch (Data[0]) { //左
		case 0x01:
			__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, Speed);
			HAL_GPIO_WritePin(AIN1_GPIO_Port, AIN1_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(AIN2_GPIO_Port,AIN2_Pin, GPIO_PIN_RESET);
			break;
		case 0x10:
			__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, Speed);
			HAL_GPIO_WritePin(AIN1_GPIO_Port, AIN1_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(AIN2_GPIO_Port, AIN2_Pin, GPIO_PIN_SET);
			break;
		case 0x11:
		    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, 0);
			HAL_GPIO_WritePin(AIN1_GPIO_Port, AIN1_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(AIN2_GPIO_Port, AIN2_Pin, GPIO_PIN_SET);
			break;
	}
	switch (Data[1]) { //右
		case 0x01:
			__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, Speed);
			HAL_GPIO_WritePin(BIN1_GPIO_Port, BIN1_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(BIN2_GPIO_Port, BIN2_Pin, GPIO_PIN_RESET);
			break;
		case 0x10:
			__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, Speed);
			HAL_GPIO_WritePin(BIN1_GPIO_Port, BIN1_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(BIN2_GPIO_Port, BIN2_Pin, GPIO_PIN_SET);
			break;
		case 0x11:
			__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, 0);
			HAL_GPIO_WritePin(BIN1_GPIO_Port, BIN1_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(BIN2_GPIO_Port, BIN2_Pin, GPIO_PIN_SET);
			break;
	}
}

void Track_mode(uint8_t Data[4]){
//	printf("巡线模式\r\n");
	uint8_t Speed;
	switch (Data[2]) {
		case 0:
			Speed = 20;
			break;
		case 1:
			Speed = 45;
			break;
		case 2:
			Speed = 60;
			break;
		case 3:
			Speed = 90;
			break;
	}
	uint8_t Track_mode_Control[4];
//	reflect_data_output(); //输出探测情况
	if (reflext_data[0]==0&&reflext_data[1]==1&&reflext_data[2]==1&&reflext_data[3]==1&&reflext_data[4]==1) //0,1,1,1,1  左转
	{
		Track_mode_Control[0] = 0x01;
		//大幅度左转
//		printf("go_left");
	}
	else if (reflext_data[0]==0&&reflext_data[1]==0&&reflext_data[2]==1&&reflext_data[3]==1&&reflext_data[4]==1) //0,0,1,1,1  左转
	{
		Track_mode_Control[0] = 0x01;
//		printf("go_left");
		//大幅度左转
	}
	else if (reflext_data[0]==0&&reflext_data[1]==0&&reflext_data[2]==0&&reflext_data[3]==1&&reflext_data[4]==1) //0,0,0,1,1  左转
	{
		Track_mode_Control[0] = 0x01;
//		printf("go_left");
		//大幅度左转
	}
	else if (reflext_data[0]==0&&reflext_data[1]==0&&reflext_data[2]==0&&reflext_data[3]==0&&reflext_data[4]==1) //0,0,0,0,1  左转
	{
		Track_mode_Control[0] = 0x01;
//		printf("go_left");
		//大幅度左转
	}
	else if (reflext_data[0]==1&&reflext_data[1]==0&&reflext_data[2]==0&&reflext_data[3]==1&&reflext_data[4]==1) //1,0,0,1,1  左转
	{
		Track_mode_Control[0] = 0x10;
//		printf("go_left");
		//小幅度左转
	}
	else if (reflext_data[0]==1&&reflext_data[1]==0&&reflext_data[2]==1&&reflext_data[3]==1&&reflext_data[4]==1) //1,0,1,1,1  左转
	{
		Track_mode_Control[0] = 0x10;
//		printf("go_left");
		//小幅度左转
	}
	else if (reflext_data[0]==1&&reflext_data[1]==1&&reflext_data[2]==1&&reflext_data[3]==1&&reflext_data[4]==0) //1,1,1,1,0  右转
	{
		Track_mode_Control[1] = 0x01;
//		printf("go_right");
		//大幅度右转
	}
	else if (reflext_data[0]==1&&reflext_data[1]==1&&reflext_data[2]==1&&reflext_data[3]==0&&reflext_data[4]==0) //1,1,1,0,0  右转
	{
		Track_mode_Control[1] = 0x01;
//		printf("go_right");
		//大幅度右转
	}
	else if (reflext_data[0]==1&&reflext_data[1]==1&&reflext_data[2]==0&&reflext_data[3]==0&&reflext_data[4]==0) //1,1,0,0,0  右转
	{
		Track_mode_Control[1] = 0x01;
//		printf("go_right");
		//大幅度右转
	}
	else if (reflext_data[0]==1&&reflext_data[1]==0&&reflext_data[2]==0&&reflext_data[3]==0&&reflext_data[4]==0) //1,0,0,0,0  右转
	{
		Track_mode_Control[1] = 0x01;
//		printf("go_right");
		//大幅度右转
	}
	else if (reflext_data[0]==1&&reflext_data[1]==1&&reflext_data[2]==0&&reflext_data[3]==0&&reflext_data[4]==1) //1,1,0,0,1  右转
	{
		Track_mode_Control[1] = 0x10;
//		printf("go_right");
		//小幅度右转
	}
	else if (reflext_data[0]==1&&reflext_data[1]==1&&reflext_data[2]==1&&reflext_data[3]==0&&reflext_data[4]==1) //1,1,1,0,1  右转
	{
		Track_mode_Control[1] = 0x10;
//		printf("go_right");
		//小幅度右转
	}
	else if (reflext_data[0]==1&&reflext_data[1]==1&&reflext_data[2]==1&&reflext_data[3]==1&&reflext_data[4]==1) //没检测到时，慢行
	{
		Track_mode_Control[2] = 0x01;
	}
	else  //其余情况下直行
	{
		Track_mode_Control[3] = 0x01;
//		printf("just_go_ahead");
	}
	switch (Track_mode_Control[0]) {
		case 0x01://大幅度左转
			__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, Speed+15); //右正
			HAL_GPIO_WritePin(BIN1_GPIO_Port, BIN1_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(BIN2_GPIO_Port, BIN2_Pin, GPIO_PIN_RESET);
			__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, Speed-20); //左停
			HAL_GPIO_WritePin(AIN1_GPIO_Port, AIN1_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(AIN2_GPIO_Port,AIN2_Pin, GPIO_PIN_SET);
			break;
		case 0x10://小幅度左转
			__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, Speed); //右
			HAL_GPIO_WritePin(BIN1_GPIO_Port, BIN1_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(BIN2_GPIO_Port, BIN2_Pin, GPIO_PIN_RESET);
			__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, Speed-15); //左
			HAL_GPIO_WritePin(AIN1_GPIO_Port, AIN1_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(AIN2_GPIO_Port,AIN2_Pin, GPIO_PIN_SET);
			break;
	}
	switch (Track_mode_Control[1]) {
		case 0x01://大幅度右转
			__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, Speed-20); //右停
			HAL_GPIO_WritePin(BIN1_GPIO_Port, BIN1_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(BIN2_GPIO_Port, BIN2_Pin, GPIO_PIN_SET);
			__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, Speed+15); //左正
			HAL_GPIO_WritePin(AIN1_GPIO_Port, AIN1_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(AIN2_GPIO_Port,AIN2_Pin, GPIO_PIN_RESET);
			break;
		case 0x10://小幅度右转
			__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, Speed-15); //右
			HAL_GPIO_WritePin(BIN1_GPIO_Port, BIN1_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(BIN2_GPIO_Port, BIN2_Pin, GPIO_PIN_SET);
			__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, Speed); //左
			HAL_GPIO_WritePin(AIN1_GPIO_Port, AIN1_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(AIN2_GPIO_Port,AIN2_Pin, GPIO_PIN_RESET);
			break;
	}
	switch (Track_mode_Control[2]) {
		case 0x01:
			__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, Speed-15);
			HAL_GPIO_WritePin(BIN1_GPIO_Port, BIN1_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(BIN2_GPIO_Port, BIN2_Pin, GPIO_PIN_RESET);
			__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, Speed-15);
			HAL_GPIO_WritePin(AIN1_GPIO_Port, AIN1_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(AIN2_GPIO_Port,AIN2_Pin, GPIO_PIN_RESET);
			break;
	}
	switch (Track_mode_Control[3]) {
		case 0x01:
			__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, Speed-5);
			HAL_GPIO_WritePin(AIN1_GPIO_Port, AIN1_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(AIN2_GPIO_Port,AIN2_Pin, GPIO_PIN_RESET);
			__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, Speed-5);
			HAL_GPIO_WritePin(BIN1_GPIO_Port, BIN1_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(BIN2_GPIO_Port, BIN2_Pin, GPIO_PIN_RESET);
			break;
	}
}

void Vision_mode(uint8_t Data[4]){
	uint8_t Speed_left;
	uint8_t Speed_right;
	Speed_left = motor1;
	Speed_right = motor2;
	switch (Data[0]) { //左
			case 0x01:
				__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, Speed_right);
				HAL_GPIO_WritePin(AIN1_GPIO_Port, AIN1_Pin, GPIO_PIN_SET);
				HAL_GPIO_WritePin(AIN2_GPIO_Port,AIN2_Pin, GPIO_PIN_RESET);
				break;
			case 0x10:
				__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, Speed_right);
				HAL_GPIO_WritePin(AIN1_GPIO_Port, AIN1_Pin, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(AIN2_GPIO_Port, AIN2_Pin, GPIO_PIN_SET);
				break;
			case 0x11:
			    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, 0);
				HAL_GPIO_WritePin(AIN1_GPIO_Port, AIN1_Pin, GPIO_PIN_SET);
				HAL_GPIO_WritePin(AIN2_GPIO_Port, AIN2_Pin, GPIO_PIN_SET);
				break;
		}
		switch (Data[1]) { //右
			case 0x01:
				__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, Speed_left);
				HAL_GPIO_WritePin(BIN1_GPIO_Port, BIN1_Pin, GPIO_PIN_SET);
				HAL_GPIO_WritePin(BIN2_GPIO_Port, BIN2_Pin, GPIO_PIN_RESET);
				break;
			case 0x10:
				__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, Speed_left);
				HAL_GPIO_WritePin(BIN1_GPIO_Port, BIN1_Pin, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(BIN2_GPIO_Port, BIN2_Pin, GPIO_PIN_SET);
				break;
			case 0x11:
				__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, 0);
				HAL_GPIO_WritePin(BIN1_GPIO_Port, BIN1_Pin, GPIO_PIN_SET);
				HAL_GPIO_WritePin(BIN2_GPIO_Port, BIN2_Pin, GPIO_PIN_SET);
				break;
		}
}
