// ---------------------------------------------------------------------------------
// libAdc.c
// Created on: 15-Jan-2010
// Author: nbk
// Description :
// SensorInit() : To initialise for sensor use
// StartSensorISR() : To trigger sensor ISR routine. This should be called regularly every msec.
// DispAllSensorValues() : Display all sensors value for debugging
// DisableSensor() & EnableSensor() - Enable or disable sensor ISR through flag
// DisplaySensorOnDotMatrix() - To display a sensor value on dot matrix display
// ReadBatteryVolt() - Read and convert battery adc input
// DMA2_Channel4_5_IRQHandler() - 	Sensor ISR, which is actually a DMA interrupt. Need to modify
// 									according to your robot needs.
// ---------------------------------------------------------------------------------

#include "project.h"

volatile uint16_t adc[2][NUM_ADC3_INPUT];
volatile uint16_t sensor[NUM_SENSOR];
volatile uint16_t sensorOld[NUM_SENSOR];
volatile uint16_t sensorMin[NUM_SENSOR];
volatile uint16_t sensorMax[NUM_SENSOR];
volatile uint16_t adc1_dma_buf[16];
volatile uint16_t adc2_dma_buf[16];
volatile uint16_t adc3_dma_buf[16];


volatile int state, substate;
volatile int adcCnt;
bool bDispSensorValue;
bool bEndSensorISRFlag;


bool bSensorEnableFlag;
void CalibrateADC(ADC_TypeDef* ADCx);

// ---------------------------------------------------------------------------------
// @brief : To init ADC and port pins for analog sensors
// @param : none
// @retval: none
// ---------------------------------------------------------------------------------
void SensorInit(void) {

	ADC_InitTypeDef ADC_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	//ADC i/p port pin initialisation
	//Change this according to your design
	//MODIFY HERE


	GPIO_Init_Mode(GPIOC, GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5, GPIO_Mode_AIN);
	GPIO_Init_Mode(GPIOA, GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7, GPIO_Mode_AIN);
	GPIO_Init_Mode(GPIOB, GPIO_Pin_0|GPIO_Pin_1, GPIO_Mode_AIN);


	//------------------------------------------------------------------
	// ADC1 is tied to DMA1 channel1
	//------------------------------------------------------------------
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);   /* Enable ADC1 clock */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

	DMA_DeInit(DMA1_Channel1);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) & (ADC1->DR);	//JDR1
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t) adc1_dma_buf;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = NUM_ADC1_INPUT;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);
	DMA_Cmd(DMA1_Channel1, ENABLE);
	ADC_DMACmd(ADC1, ENABLE);

	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode = ENABLE; // Convert a few channels
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; 	//ADC_ExternalTrigInjecConv_None
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel = NUM_ADC1_INPUT;
	ADC_Init(ADC1, &ADC_InitStructure);

	// Add all the non-pulsed adc inputs here
	//MODIFY HERE

	ADC_RegularChannelConfig(ADC1, ADC_CH_SEN_0, 1, ADC_SampleTime_28Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_CH_SEN_1, 2, ADC_SampleTime_28Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_CH_SEN_2, 3, ADC_SampleTime_28Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_CH_SEN_3, 4, ADC_SampleTime_28Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_CH_SEN_4, 5, ADC_SampleTime_28Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_CH_SEN_5, 6, ADC_SampleTime_28Cycles5);




	// Enable dma interrupt on TC5 (transfer complete)
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = SENSOR_ISR_PRIORITY;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	DMA_ITConfig(DMA1_Channel1, DMA_IT_TC, ENABLE);


	ADC_Cmd(ADC1, ENABLE);
	CalibrateADC(ADC1);
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		// ADC actively converting all the time
					// result in adc1_dma_buf

}

// ---------------------------------------------------------------------------------
// @brief : To start sensor isr. ADC3 is triggered by TIM_SensorISR
//		  : ADC1 is software started
// @param : none
// @retval: none
// ---------------------------------------------------------------------------------
void StartSensorISR() {
	state = 0;
	substate = 0;
	bEndSensorISRFlag = FALSE;

	TIM_Cmd(TIM_SensorISR, ENABLE);
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);	//Start ADC1 conversion
}

void DMA1_Channel1_IRQHandler(void) {
	DMA_ClearITPendingBit(DMA1_IT_TC1);
		int a=0;


	for(a=0;a<NUM_ADC1_INPUT;a++){
    	 sensor[a] = adc1_dma_buf[a];
     }


}

// ---------------------------------------------------------------------------------
// @brief : To calibrate ADC during initialisation
// @param ADCx: which adc
// @retval: none
// ---------------------------------------------------------------------------------
void CalibrateADC(ADC_TypeDef* ADCx) {

	/* Enable ADCx reset calibaration register */
	ADC_ResetCalibration(ADCx);

	/* Check the end of ADCx reset calibration register */
	while (ADC_GetResetCalibrationStatus(ADCx))
		;

	/* Start ADCx calibaration */
	ADC_StartCalibration(ADCx);

	/* Check the end of ADCx calibration */
	while (ADC_GetCalibrationStatus(ADCx))
		;
}

// @brief : To collect sensors' minimum and maximum values
// @param : none
// @retval: none
void CollectSensorStatistic() {
	uint16_t i;

	for (i = 0; i < NUM_SENSOR; i++) {
		if (sensor[i] > sensorMax[i])
			sensorMax[i] = sensor[i];
		if (sensor[i] < sensorMin[i])
			sensorMin[i] = sensor[i];
	}

}

// ---------------------------------------------------------------------------------
// @brief : To display all sensor values on UART
// @param : none
// @retval: none
// ---------------------------------------------------------------------------------
#define ROW1	4
#define COL1	8
void DispAllSensorValues() {
	int i;

	bDispSensorValue = TRUE;
	clrscr();
	DispDotMatrixRaw(waitMsg, waitMsgSize);
	DelaymSec(500);

	//	SetMotorCtrl(ENABLE_MOTOR);
	//	SetMotorCtrl(FORWARDDIR);
	//	targetXSpeed = 1000;

	//	SetNextCommand( CMD_STRAIGHT_RUN, 0);
	//	curTargetPosX = 0;
	//	finalTargetPosX = CD(1800);


	for (i = 0; i < NUM_SENSOR; i++) {
		sensorMax[i] = 0;
		sensorMin[i] = 4096;
	}



	while (1) {

		gotoxy(COL1, ROW1);
		printf("   S0   S1   S2   S3   S4   S5");		// dc value

		gotoxy(COL1, ROW1+2);
		printf("%4u %4u %4u %4u %4u %4u ", sensor[0], sensor[1], sensor[2],
				sensor[3], sensor[4], sensor[5]);


		gotoxy(COL1, ROW1+4);
		printf("Battery %4uV ", ReadBatteryVolt());


		if (bSWFlag) {
			bSWFlag = 0;
			DelaymSec(200);
			break;
		}
	}
	//	SetMotorCtrl(DISABLE_MOTOR);
	bDispSensorValue = FALSE;
}

// @brief : To disable IR sensor isr()
void DisableSensor() {
	bSensorEnableFlag = 0;
}

// @brief : To enable IR sensor isr()
void EnableSensor() {
	bSensorEnableFlag = 1;
}

void DisplaySensorOnDotMatrix(int sensorNum) {

	uint16_t value;
	uint16_t *ptr = NULL;

	switch (sensorNum) {
	case 1:
		ptr = (uint16_t *) &sensor[0];
		break;
	case 2:
		ptr = (uint16_t *) &sensor[1];
		break;
	case 3:
		ptr = (uint16_t *) &sensor[2];
		break;
	case 4:
		ptr = (uint16_t *) &sensor[3];
		break;
	case 5:
		ptr = (uint16_t *) &sensor[4];
		break;
	case 6:
		ptr = (uint16_t *) &sensor[5];
		break;
	case 7:
		ptr = (uint16_t *) &sensor[6];
		break;
	case 8:
		ptr = (uint16_t *) &sensor[7];
		break;


	}

	while (1) {
		char s[10];
		value = *ptr;

		sprintf(s, "%04d", value);
		DispDotMatrix(s);
		DelaymSec(50);

		if (bSWFlag) {
			//Beep(SOUND_ACK);
			bSWFlag = 0;
			break;
		}
	}
}

void DisplayBatteryVoltOnDotMatrix() {
	while (1) {
		char s[10];

		sprintf(s, "%04d", ReadBatteryVolt());
		DispDotMatrix(s);
		DelaymSec(100);

		if (bSWFlag) {
			bSWFlag = 0;
			break;
		}
	}
}

// Convert battery adc value into 0.01 volts. So 8.4 volts is returned as 840.
uint16_t ReadBatteryVolt() {
	// R series = 10k & 20k
	// ratio = 0.333
	// adc is 4096 for 3.3V
	// 8.4V -> adc = 8.4V*0.3333/3.3V*4096 = 3476
	// The 2nd parameter needs to be calibrated
	return (senBattery * 840) / 3476;
}

