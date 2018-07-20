// ---------------------------------------------------------------------------------
// libSensor.h
// Created on: 22-Feb-2011
// Author: nbk
// ---------------------------------------------------------------------------------

#ifndef LIBSENSOR_H_
#define LIBSENSOR_H_

// ---------------------------------------------------------------------------------
//  Macros
// ---------------------------------------------------------------------------------
// These sensors need to be actively pulsed. They are converted thru ADC3
#define ADC_CH_SEN_0		ADC_Channel_12
#define ADC_CH_SEN_1		ADC_Channel_13
#define ADC_CH_SEN_2		ADC_Channel_0
#define ADC_CH_SEN_3		ADC_Channel_1
#define NUM_ADC3_INPUT		0

// These sensors provide analog output directly. They are converted thru ADC1
#define ADC_CH_SEN_4	ADC_Channel_2
#define ADC_CH_SEN_5	ADC_Channel_3
#define NUM_ADC1_INPUT		6

#define NUM_SENSOR		(NUM_ADC3_INPUT+NUM_ADC1_INPUT)

// IO pins used for IR sensor transmit
#define SEN_TX0			GPIOC,GPIO_Pin_13
#define SEN_TX12		GPIOB,GPIO_Pin_8
#define SEN_TX12_H		GPIOB,GPIO_Pin_9
#define SEN_TX3			GPIOC,GPIO_Pin_14

// Macros to give meaningful names to the sensors
#define senBattery			sensor[5]

// ---------------------------------------------------------------------------------
//  Global variables
// ---------------------------------------------------------------------------------
extern volatile uint16_t adc1_dma_buf[16];	// For adc->dma buffer
extern volatile uint16_t adc2_dma_buf[16];	// max-16 inputs
extern volatile uint16_t adc3_dma_buf[16];	// Each adc has it's own buffer

extern volatile uint16_t adc3[2][NUM_ADC3_INPUT];		// For storing the adc values
extern volatile uint16_t sensor[NUM_SENSOR];		// actual sensor values
extern volatile uint16_t sensorOld[NUM_SENSOR];

extern char dispBuffer[256];
extern bool	bDispSensorValue;
extern bool bSensorEnableFlag;
extern bool bFlashDetectFlag;
extern bool bEndSensorISRFlag;

// ---------------------------------------------------------------------------------
//  Function prototypes
// ---------------------------------------------------------------------------------
void SensorInit(void);
void StartSensorISR(void);
void DispAllSensorValues(void);
void CollectSensorStatistic();
void EnableSensor();
void DisableSensor();
void DisplaySensorOnDotMatrix(int sensorNum);
void DisplayBatteryVoltOnDotMatrix();
uint16_t ReadBatteryVolt();

#endif /* LIBSENSOR_H_ */
