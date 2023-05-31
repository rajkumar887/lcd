/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
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
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
//WIZNET Includes
#include "../../ioLibrary_Driver/Ethernet/socket.h"
#include "../../ioLibrary_Driver/Ethernet/wizchip_conf.h"
#include "../../ioLibrary_Driver/Application/loopback/loopback.h"
#include "w5500_init.h"
//OLED Includes
//#include "../../ssd1306/ssd1306.h"
//#include "../../ssd1306/ssd1306_tests.h"
#include <stdbool.h>
#include <stdlib.h>
#include<string.h>
//Synthesizer Includes
#include "ADF4351.h"
//NHD OLED Include file
#include "NHD_2_23_OLED.h"
//W25N01 SPI FLASH
#include "W25N01_FLASH.h"
#include "EEPROM.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
void Print_IP(void);
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;
ADC_HandleTypeDef hadc2;

DAC_HandleTypeDef hdac1;
DAC_HandleTypeDef hdac2;

I2C_HandleTypeDef hi2c1;
I2C_HandleTypeDef hi2c4;

SPI_HandleTypeDef hspi2;
SPI_HandleTypeDef hspi4;
SPI_HandleTypeDef hspi5;

TIM_HandleTypeDef htim2;

UART_HandleTypeDef huart3;

/* USER CODE BEGIN PV */
ADC_ChannelConfTypeDef sConfig = {0};
DAC_ChannelConfTypeDef sConfig1 = {0};

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void PeriphCommonClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_SPI2_Init(void);
static void MX_TIM2_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_SPI4_Init(void);
static void MX_ADC1_Init(void);
static void MX_ADC2_Init(void);
static void MX_DAC1_Init(void);
static void MX_SPI5_Init(void);
static void MX_I2C4_Init(void);
static void MX_DAC2_Init(void);
/* USER CODE BEGIN PFP */
#define T_R_CTRL_1 HAL_GPIO_WritePin(T_R_CTRL_CC_GPIO_Port,T_R_CTRL_CC_Pin,SET);
#define T_R_CTRL_0 HAL_GPIO_WritePin(T_R_CTRL_CC_GPIO_Port,T_R_CTRL_CC_Pin,RESET);

#define TX_CTRL_1 HAL_GPIO_WritePin(TX_CTRL_CC_GPIO_Port,TX_CTRL_CC_Pin,SET);
#define TX_CTRL_0 HAL_GPIO_WritePin(TX_CTRL_CC_GPIO_Port,TX_CTRL_CC_Pin,RESET);

#define TX_CTRL1_1 HAL_GPIO_WritePin(TX_CTRL1_GPIO_Port, TX_CTRL1_Pin, SET);
#define TX_CTRL1_0 HAL_GPIO_WritePin(TX_CTRL1_GPIO_Port, TX_CTRL1_Pin, RESET);

#define RX_CTRL_1 HAL_GPIO_WritePin(RX_CTRL_CC_GPIO_Port,RX_CTRL_CC_Pin,SET);
#define RX_CTRL_0 HAL_GPIO_WritePin(RX_CTRL_CC_GPIO_Port,RX_CTRL_CC_Pin,RESET);

#define VG_CTRL_1 HAL_GPIO_WritePin(VG_CTRL_CC_GPIO_Port,VG_CTRL_CC_Pin,SET);
#define VG_CTRL_0 HAL_GPIO_WritePin(VG_CTRL_CC_GPIO_Port,VG_CTRL_CC_Pin,RESET);

#define LNA_CTRL_1 HAL_GPIO_WritePin(LNA_CTRL_CC_GPIO_Port,LNA_CTRL_CC_Pin,SET);
#define LNA_CTRL_0 HAL_GPIO_WritePin(LNA_CTRL_CC_GPIO_Port,LNA_CTRL_CC_Pin,RESET);

#define RX_AGC_CTRL_1 HAL_GPIO_WritePin(RX_AGC_CTRL_CC_GPIO_Port,RX_AGC_CTRL_CC_Pin,SET);
#define RX_AGC_CTRL_0 HAL_GPIO_WritePin(RX_AGC_CTRL_CC_GPIO_Port,RX_AGC_CTRL_CC_Pin,RESET);

#define RX_ATTN_CTRL_1 HAL_GPIO_WritePin(RX_ATTN_CTRL_CC_GPIO_Port,RX_ATTN_CTRL_CC_Pin,SET);
#define RX_ATTN_CTRL_0 HAL_GPIO_WritePin(RX_ATTN_CTRL_CC_GPIO_Port,RX_ATTN_CTRL_CC_Pin,RESET);

#define RX_MUTE_CTRL_1 HAL_GPIO_WritePin(RX_MUTE_CTRL_CC_GPIO_Port,RX_MUTE_CTRL_CC_Pin,SET);
#define RX_MUTE_CTRL_0 HAL_GPIO_WritePin(RX_MUTE_CTRL_CC_GPIO_Port,RX_MUTE_CTRL_CC_Pin,RESET);

#define IF_AGC_CTRL_1 HAL_GPIO_WritePin(IF_AGC_CTRL_CC_GPIO_Port,IF_AGC_CTRL_CC_Pin,SET);
#define IF_AGC_CTRL_0 HAL_GPIO_WritePin(IF_AGC_CTRL_CC_GPIO_Port,IF_AGC_CTRL_CC_Pin,RESET);

#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif

//OLED Function Prototype Declaration
void OLED_Reset(void);
void OLED_Launch_Screen(const unsigned char *icon);
void OLED_Launch_Screen1(const unsigned char *icon);
//Ethernet Function Prototype Declaration
void Ethernet_TCP_UDP_RUN(void);
void WIZNET_Inint_Process(void);
//OLED Application definations
void Menu_OLED_Application(void);
//Keypad FPF
char Read_keypad(void);
void Keypad_Application(void);
void Password_Login_Page(void);
void Battery_Display(const unsigned char *icon);
void Signal_Display(const unsigned char *icon);
void Down_Up_Arrow(const unsigned char *icon);
void Mute_Speaker(const unsigned char *icon);
void Freq_Channel_Reading(void);
void Manual_FREQ_Reading(void);
void Manual_Freq_Applying(void);
double AVF_Reading(void);
double AVR_Reading(void);
double DVF_Reading(void);
double DVR_Reading(void);
double ARSSI_Reading(void);
double DRSSI_Reading(void);
double AVTEMP_Reading(void);
double DVTEMP_Reading(void);
uint32_t AMRSSI_Reading(void);
double Audio_out_reading(void);
void TX_RX_Controls(void);
void RX_DAC_Controls(void);
void AGC_Enable(uint8_t EN);
void Frequency_Reading(void);
void Channel_Reading(void);
void Frequency_Deleting(void);
void Channel_Deleting(void);
void Save_Changes_Apply(void);
void Reset_Passwords(void);
void Password_Reading(void);
void Reset_Password_Reading(void);
void DAC1_Generating(double Value);
void DAC2_Generating(double Value);
void DAC3_Generating(double Value);
void SQ_Check_State(void);
double voltage_rms(double voltage_samples[], int num_samples);

double DAC1_Voltage, DAC2_Voltage,DAC3_Voltage;
double AVF_Volt,AVR_Volt,VSWR_Volt,DVF_Volt,DVR_Volt,ARSSI_Volt,DRSSI_Volt,AVTEMP_Volt,
DVTEMP_Volt,AMRSSI_Diff,Audio_out_volt;
unsigned int Audio_Speaker_State;
uint32_t AMRSSI_Volt;
unsigned int VSWR_State,AVTEMP_State,RX_ATTN,Mute_CTRL,VSWR_State_Control,Prev_VSWR_State,Prev_AVTEMP_State;
unsigned int Squelth_Check_state,Squelth_Read_State;
unsigned int PTT_Switch_state=0,PTT_RX_State=0,PTT_OLED_Erase_state;
uint8_t Gain_Lock;
unsigned int Frq_Length_DC,Frq_Length_IC;
int Index1,Index2,Index3,Index4,Index5,Index6,Index7,Index8,Index_CHANNEL2,Index_FREQUENCY2,Index_SQ_TH,tempvalue;
char aut[20], rxlevel[20]="-105",channel[20]="0",Current_CH[5]="0",passwordmask[5]="0",Password[5]="0",frequency[20]="0",
		Current_Freq[20]="0",SQ_TH[20],TX_PWR_WT[3]="0",Enter_New_Pass[10]="0",Enter_New_Pass_Mask[10]="0",Re_Enter_Pass[10]="0",Re_Enter_Pass_Mask[10]="0",
		Saved_Password[10],Read_Saved_Password[10],Compare_PassWord[10],Save_Frequency[20],SQ_T_H_Val[5]="1",
		Admin_Password[20]="2233",Admin_Password_Entry[20]="0",Admin_Password_Entry_Mask[20]="0",dummy_freq[20];
char EEPROM_Freq[20],EEPROM_CH[20];
unsigned AGC_TH = 27,VSWR_TH = 0.7;
unsigned int SQ_T_H_Val_int;
unsigned int Undersore =0;
double Frequency_Int,Prev_Frequency_Int,Channel_Int,Prev_Channel_Int,Rssi_Int,Prev_Rssi_Int,OLED_Frequency,Frequency_Step_Inc,Frequency_Step_Dnc;
unsigned int Save_CH_TX,Save_CH_RX,Read_CH_TX,Read_CH_RX,TX_Mode_Flag,RX_Mode_Flag;
double Save_frequency_double;
double ETH_Freq,ETH_TX_RX_Flag;
unsigned int ETH_CH_NO,ETH_Tx_PWR,ETH_Frequency_Int;
char ETH_GET,ETH_CC_DD_Mode;
char TCP_Send_Buf[10],ETH_Current_Freq[20]="0",ETH_Current_CH[5]="0";
unsigned int PTT_OLED_Erase,Password_State=1,Enter_Pass_Length,Re_Enter_Pass_Length,
		Password_Length,Enter_Password_State,Admin_Password_State;
int Admin_Reset_State=0,Admin_Password_Entry_State,Admin_Pass_Length;
int Inc=0,Back_Page=0,Reset_State=0,YES_State,NO_State,Login_Entry_Flag,Admin_Entry_flag,CH_Erase_RX;
char Key,Keydata=0;
int Indexcur=0,Size=0;
uint16_t raw;
char msg[20];
int screenchange=0,Page_Erase0,Page_Erase1,Page_Erase2,Page_Erase3,Page_Erase4,Page_Erase5,PassWord_flag;
int Enter=0,Menu=0,back,passcomp=0,Page_start,Num_Value=0,Inc_State,Manual_FRQ_Entry;
unsigned int Get_Tx_CH_Flag,Get_Rx_CH_Flag;
int Tx_Parameters,Tx_channel,Rx_Parameters,Rx_channel,Mask_Flag;
unsigned int Tx_Parameters_Flag,Tx_channel_Flag,Rx_Parameters_Flag,Rx_channel_Flag,Mask_Flag_flag;
int Page_start,Tx_Flag,Rx_Flag;
unsigned char RX_ATTN_TH;
unsigned int DAC1_state;
double Default_DAC1_volt;
//char Keydata;
unsigned int Password_Reset_Flag=0;
unsigned int Pass0,Pass1,Pass2,Pass3,Saved_flag0,Saved_flag1,Saved_flag2,Saved_flag3;
unsigned char Menu_Disp0[21]="  SET TX PARAMETERS";
unsigned char Menu_Disp1[21]="  SEL TX CHANNEL NO";
unsigned char Menu_Disp2[21]="  SET RX PARAMETERS";
unsigned char Menu_Disp3[21]="  SEL RX CHANNEL NO";
unsigned char Menu_Disp0_1[23]="->SET TX PARAMETERS";
unsigned char Menu_Disp1_1[22]="->SEL TX CHANNEL NO";
unsigned char Menu_Disp2_1[23]="->SET RX PARAMETERS";
unsigned char Menu_Disp3_1[23]="->SEL RX CHANNEL NO";
unsigned char Clear_Arrow0_1[4]="  ";
unsigned char Clear_Arrow0_2[4]="  ";
unsigned char Clear_Arrow0_3[4]="  ";
unsigned char Clear_Arrow0_4[4]="  ";
unsigned char Clear_Arrow1_1[4]="->";
unsigned char Clear_Arrow1_2[4]="  ";
unsigned char Clear_Arrow1_3[4]="  ";
unsigned char Clear_Arrow1_4[4]="  ";
unsigned char Clear_Arrow2_1[4]="  ";
unsigned char Clear_Arrow2_2[4]="->";
unsigned char Clear_Arrow2_3[4]="  ";
unsigned char Clear_Arrow2_4[4]="  ";
unsigned char Clear_Arrow3_1[4]="  ";
unsigned char Clear_Arrow3_2[4]="  ";
unsigned char Clear_Arrow3_3[4]="->";
unsigned char Clear_Arrow3_4[4]="  ";
unsigned char Clear_Arrow4_1[4]="  ";
unsigned char Clear_Arrow4_2[4]="  ";
unsigned char Clear_Arrow4_3[4]="  ";
unsigned char Clear_Arrow4_4[4]="->";
unsigned char TX_PARAMETERS[15]="TX PARAMETERS";
unsigned char FREQ_str[11]="FREQ   : ";
unsigned char TX_PWR_str[11]="TX PWR : ";
unsigned char CH_NO_Str[11]="CH NO  : ";
unsigned char Back_Str[6]="BACK";
unsigned char RX_PARAMETERS[15]="RX PARAMETERS";
unsigned char SQ_SET_ON[13]= "Sq :  ON";
unsigned char SQ_SET_OFF[13]="Sq : OFF";
unsigned char SQ_T_H[10]="SQ_T/H :";
unsigned char Enter_password[14]="  ENTER PW: ";
unsigned char Reset_password[18]="  RESET PASSWORD";
unsigned char Enter_password_1[13]="->ENTER PW: ";
unsigned char Reset_password_1[18]="->RESET PASSWORD";
unsigned char Saved_in_memory[17]="SAVED IN MEMORY";
unsigned char Featching_in_memory[23]="FETCHING FROM MEMORY";
unsigned char Not_Available_in_memory[15]="NOT AVAILABLE";
unsigned char Not_in_Range[15]="NOT IN RANGE";
unsigned char Rx_CH[9]="CH NO: ";
unsigned char FREQ[8]="FREQ: ";
unsigned char Rx[4]="Rx";
unsigned char MHz[5]="MHz";
unsigned char Rssi[8]="Rssi: ";
unsigned char Sq[9]="Sq: OFF";
unsigned char dBm[5]="dBm";
unsigned char Enter_New_Pass_Disp[18] = "Enter New Pass: ";
unsigned char Re_Enter_Pass_Disp[17] = "Re Enter Pass: ";
unsigned char Enter_CH_No[15] = "Enter CH No: ";

/*Channel defines for savings in SPI Flash*/
const unsigned char RECT[] = { /* 0X10,0X01,0X00,0X14,0X00,0X11, */
		0xff, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81,		//0xe0, 0x00, 0xf8, 0x50, 0x4c, 0xfc, 0x02, 0xff
		0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81
};

const unsigned char SGT4[] = { /* 0X10,0X01,0X00,0X14,0X00,0X11, */
		0x78, 0x78, 0x78, 0x00, 0x7c, 0x7c, 0x7c, 0x00, 0x7e, 0x7e, 0x7e, 0x00, 0x7f, 0x7f, 0x7f, 0x00		//0xe0, 0x00, 0xf8, 0x50, 0x4c, 0xfc, 0x02, 0xff
};

const unsigned char SGT3[] = { /* 0X10,0X01,0X00,0X14,0X00,0X11, */
		0xf8, 0xf8, 0xf8, 0x00, 0xfc, 0xfc, 0xfc, 0x00, 0xfe, 0xfe, 0xfe, 0x00, 0xff, 0x81, 0xff, 0x00		//0xe0, 0x00, 0xf8, 0x50, 0x4c, 0xfc, 0x02, 0xff
};

const unsigned char SGT2[] = { /* 0X10,0X01,0X00,0X14,0X00,0X11, */
		0xf8, 0xf8, 0xf8, 0x00, 0xfc, 0xfc, 0xfc, 0x00, 0xfe, 0x82, 0xfe, 0x00, 0xff, 0x81, 0xff, 0x00	//0xe0, 0x00, 0xf8, 0x50, 0x4c, 0xfc, 0x02, 0xff
};

const unsigned char SGT1[] = { /* 0X10,0X01,0X00,0X14,0X00,0X11, */
		0xf8, 0xf8, 0xf8, 0x00, 0xfc, 0x84, 0xfc, 0x00, 0xfe, 0x82, 0xfe, 0x00, 0xff, 0x81, 0xff, 0x00		//0xe0, 0x00, 0xf8, 0x50, 0x4c, 0xfc, 0x02, 0xff
};

const unsigned char SGT0[] = { /* 0X10,0X01,0X00,0X14,0X00,0X11, */
		0xf8, 0x88, 0xf8, 0x00, 0xfc, 0x84, 0xfc, 0x00, 0xfe, 0x82, 0xfe, 0x00, 0xff, 0x81, 0xff, 0x00		//0xe0, 0x00, 0xf8, 0x50, 0x4c, 0xfc, 0x02, 0xff
};

const unsigned char BT4[] = { /* 0X10,0X01,0X00,0X14,0X00,0X11, */
		// 0x7f, 0x41, 0x3d, 0x3d, 0x41, 0x3d, 0x3d, 0x41, 0x3d, 0x3d, 0x41, 0x3d, 0x3d, 0x41, 0x7f, 0x1e		//0xe0, 0x00, 0xf8, 0x50, 0x4c, 0xfc, 0x02, 0xff
		0xff, 0x81, 0xbd, 0xbd, 0x81, 0xbd, 0xbd, 0x81, 0xbd, 0xbd, 0x81, 0xbd, 0xbd, 0x81, 0xff, 0x3c
};

const unsigned char BT3[] = { /* 0X10,0X01,0X00,0X14,0X00,0X11, */
		0xff, 0x81, 0xbd, 0xbd, 0x81, 0xbd, 0xbd, 0x81, 0xbd, 0xbd, 0x81, 0x81, 0x81, 0x81, 0xff, 0x3c		//0xe0, 0x00, 0xf8, 0x50, 0x4c, 0xfc, 0x02, 0xff
};

const unsigned char BT2[] = { /* 0X10,0X01,0X00,0X14,0X00,0X11, */
		0xff, 0x81, 0xbd, 0xbd, 0x81, 0xbd, 0xbd, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0xff, 0x3c		//0xe0, 0x00, 0xf8, 0x50, 0x4c, 0xfc, 0x02, 0xff
};

const unsigned char BT1[] = { /* 0X10,0X01,0X00,0X14,0X00,0X11, */
		0xff, 0x81, 0xbd, 0xbd, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0xff, 0x3c		//0xe0, 0x00, 0xf8, 0x50, 0x4c, 0xfc, 0x02, 0xff
};

const unsigned char Arrow[] = {
		0x18, 0x18, 0x18, 0x18, 0x7e, 0x42, 0x24, 0x18
};
const unsigned char Arrow_Down[] =
{
		0x10, 0x20, 0x40, 0xff, 0x40, 0x20, 0x10, 0x00
};
const unsigned char Arrow_Up[] =
{
		0x08, 0x04, 0x02, 0xff, 0x02, 0x04, 0x08, 0x00
};
const unsigned char Clear_Arrow[] =
{
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
const unsigned char Speaker_UnMute[] =
{
		0x18, 0x3c, 0x42, 0x81, 0xff, 0x00, 0x24, 0x99, 0x42, 0x24, 0x99, 0x42, 0x24, 0x18, 0x00, 0x00
};
const unsigned char Speaker_Mute[] =
{
		0x18, 0x3c, 0x42, 0x81, 0xff, 0x00, 0x00, 0x42, 0x24, 0x18, 0x18, 0x24, 0x42, 0x00, 0x00, 0x00
};
// 'ics_logo', 128x32px
// Array of all bitmaps for convenience. (Total bytes used to store images in PROGMEM = 528)
const unsigned char epd_bitmap_ics_logo [512] = {

		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

		0x00, 0xe0, 0xf8, 0xf8, 0xf8, 0xfc, 0x1c, 0x04, 0x04, 0x04, 0x04, 0x84, 0x84, 0x80, 0x88, 0x88,

		0x98, 0xa0, 0x80, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

		0x00, 0x00, 0x00, 0x00, 0x02, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0x02, 0x02, 0x00, 0xe0,

		0x30, 0x9f, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xf6, 0x08, 0x08, 0x6a, 0xc3, 0x2f, 0xc3, 0xb5, 0x39,

		0x37, 0x09, 0x13, 0x25, 0x45, 0x8b, 0x64, 0x46, 0xca, 0xba, 0xa4, 0x8a, 0x3c, 0x34, 0xc8, 0x30,

		0xf0, 0xc0, 0x80, 0x00, 0x20, 0x78, 0xfc, 0xfc, 0xfe, 0xf6, 0xc6, 0xc6, 0xc6, 0xc6, 0x84, 0x1c,

		0x3e, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

		0x00, 0x00, 0x80, 0x80, 0x80, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x80, 0x80, 0x80, 0x07,

		0x08, 0x1a, 0x34, 0x25, 0x0b, 0x77, 0x9f, 0xff, 0x7f, 0xff, 0xf8, 0xf8, 0xf3, 0xc0, 0x9b, 0xb2,

		0x86, 0xbc, 0x83, 0xc4, 0xb9, 0x61, 0x68, 0x50, 0xd2, 0xbe, 0x8c, 0x61, 0x43, 0x12, 0x35, 0x1b,

		0x0c, 0x07, 0x03, 0x00, 0x00, 0xf8, 0x61, 0x61, 0x43, 0x43, 0xc3, 0xc7, 0x87, 0xc7, 0xff, 0x7f,

		0x3f, 0x3e, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x07, 0x0f, 0x1f, 0x1f,

		0x3f, 0x38, 0x38, 0x70, 0x70, 0x60, 0x60, 0x40, 0x40, 0x60, 0x60, 0x3f, 0x1f, 0x00, 0x00, 0x00,

		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/*Ethernet Defines Start */
///////////////////////////////////////
// Debugging Message Printout enable //
///////////////////////////////////////
#define _MAIN_DEBUG_

///////////////////////////
// Demo Firmware Version //
///////////////////////////
#define VER_H		1
#define VER_L		00

//////////////////////////////////////////////////
// Socket & Port number definition for Examples //
//////////////////////////////////////////////////
#define SOCK_TCPS       0
#define SOCK_UDPS       1
#define PORT_TCPS		5000
#define PORT_UDPS       3000

////////////////////////////////////////////////
// Shared Buffer Definition for LOOPBACK TEST //
////////////////////////////////////////////////

uint8_t gDATABUF[DATA_BUF_SIZE];

///////////////////////////
// Network Configuration //
///////////////////////////
//wiz_NetInfo gWIZNETINFO = { .mac  = {0x00, 0x08, 0xdc, 0x75, 0xc5, 0xac},//Unit Sl. No: 1 completed-10032023
//wiz_NetInfo gWIZNETINFO = { .mac = {0x00, 0x08, 0xdc, 0x75, 0xc5, 0xaf},//Unit Sl. No: 2
//wiz_NetInfo gWIZNETINFO = { .mac = {0x00, 0x08, 0xdc, 0x75, 0xc5, 0xae},//Unit Sl. No: 3 completed-10032023
wiz_NetInfo gWIZNETINFO = { .mac = {0x00, 0x08, 0xdc, 0x75, 0xc5, 0xab},//Unit Sl. No: 4 completed-10032023
		.ip = {192, 168, 1, 91},
		.sn = {255, 255, 255, 0},
		.gw = {192, 168, 1, 1},
		.dns = {8, 8, 8, 8},
		.dhcp = NETINFO_STATIC };

//wiz_NetInfo gWIZNETINFO = { .mac = {0x00, 0x08, 0xdc, 0x75, 0xc5, 0xab},
//		.ip = {192, 168, 1, 91},
//		.sn = {255, 255, 255, 0},
//		.gw = {192, 168, 1, 1},
//		.dns = {8, 8, 8, 8},
//		.dhcp = NETINFO_STATIC };
//
//wiz_NetInfo gWIZNETINFO = { .mac = {0x00, 0x08, 0xdc, 0x75, 0xc5, 0xab},
//		.ip = {192, 168, 1, 91},
//		.sn = {255, 255, 255, 0},
//		.gw = {192, 168, 1, 1},
//		.dns = {8, 8, 8, 8},
//		.dhcp = NETINFO_STATIC };
//
//wiz_NetInfo gWIZNETINFO = { .mac = {0x00, 0x08, 0xdc, 0x75, 0xc5, 0xab},
//		.ip = {192, 168, 1, 91},
//		.sn = {255, 255, 255, 0},
//		.gw = {192, 168, 1, 1},
//		.dns = {8, 8, 8, 8},
//		.dhcp = NETINFO_STATIC };

// For TCP client loopback examples; destination network info
uint8_t destip[4] = {192, 168, 0, 248};
uint16_t destport = 5000;
unsigned int TCP_Recv_Flag=0;
char TCP_Rec_Buff[13];

void WIZNET_Inint_Process(void)
{
	HAL_GPIO_WritePin(WIZ_RST_GPIO_Port, WIZ_RST_Pin, SET);
	HAL_Delay(3000);
	W5500_Init();
	HAL_Delay(3000);
	Net_Conf(gWIZNETINFO);

#ifdef _MAIN_DEBUG_
	uint8_t tmpstr[6] = {0,};

	ctlwizchip(CW_GET_ID,(void*)tmpstr);

	printf("\r\n=======================================\r\n");
	printf(" WIZnet %s EVB Board v%d.%.2d\r\n", tmpstr, VER_H, VER_L);
	printf("=======================================\r\n");
	//	printf(">> W5500 based Loopback Example\r\n");
	//	printf("=======================================\r\n");
	Display_Net_Conf(); // Print out the network information to serial terminal
#endif

}
/*Ethernet Defines END */
//void OLED_Reset_Init(void)
//{
//	HAL_GPIO_WritePin(OLED_Reset_GPIO_Port, OLED_Reset_Pin, RESET);
//	HAL_Delay(200);
//	HAL_GPIO_WritePin(OLED_Reset_GPIO_Port, OLED_Reset_Pin, SET);
//	HAL_Delay(100);
//	HAL_GPIO_WritePin(OLED_Reset_GPIO_Port, OLED_SA0_Pin, RESET);
//	HAL_Delay(200);
//	HAL_GPIO_WritePin(OLED_Reset_GPIO_Port, OLED_SA0_Pin, SET);
//	HAL_Delay(1000);
//	ssd1306_Init();
//}
void OLED_Launch_Screen(const unsigned char *icon)
{
	unsigned int i,j;
	for(i=0;i<4;i++){
		Set_Start_Page_12832(i);
		Set_Start_Column_12832(0x04);
		for(j=0;j<128;j++){     //128 columns wide
			oled_Data(*icon);    //send picture data
			icon++;
		}
	}
}
void OLED_Launch_Screen1(const unsigned char *icon)
{
	unsigned int i,j;
	for(i=0;i<4;i++){
		Set_Start_Page_12832(i);
		Set_Start_Column_12832(0x04);
		for(j=0;j<128;j++){     //128 columns wide
			oled_Data(*icon);    //send picture data
			icon++;
		}
	}
}
HAL_StatusTypeDef SPI_SendWord(uint32_t data, SPI_HandleTypeDef* hspi)
{
	HAL_StatusTypeDef ret;
	uint8_t buf[4];
	buf[0] = (data >> 24) & 0xff;
	buf[1] = (data >> 16) & 0xff;
	buf[2] = (data >> 8) & 0xff;
	buf[3] = data & 0xff;

	HAL_GPIO_WritePin(SPI1_NSS_4351_GPIO_Port,SPI1_NSS_4351_Pin,GPIO_PIN_RESET);
	ret = HAL_SPI_Transmit(hspi, buf, 4, 10);
	HAL_GPIO_WritePin(SPI1_NSS_4351_GPIO_Port,SPI1_NSS_4351_Pin,GPIO_PIN_SET);

	return ret;
}
/***************************************************************************//**
 * @brief Writes 4 bytes of data to ADF4351.
 * @param data - Data value to write.
 * @retval SPI status
 *******************************************************************************/
HAL_StatusTypeDef adf4351_write(uint32_t data)
{
	return SPI_SendWord(data, &hspi4);
}

void PLL_Sync(void)
{
	int i;

	for (i=0; i<6; i++)
	{
		adf4351_write(ADF4351_GetRegisterBuf(5-i));	//	Write registers to PLL chip
	}
}
double CurrentFreq;
double calcfreq,TX_CurrentFreq,RX_CurrentFreq;
unsigned int PTT_state=0,prev_PTT_state;

void ADF3451_REG_Init(void)
{
	ADF4351_Init();
	ADF4351_Reg1.b.PhaseAdjust = ADF4351_DISABLE;
	ADF4351_Reg1.b.PhaseVal = 1;
	ADF4351_Reg1.b.Prescaler = ADF4351_PRESCALER_4_5;

	ADF4351_Reg2.b.CounterReset = ADF4351_DISABLE;
	ADF4351_Reg2.b.RDiv2 = ADF4351_REFDIV_1;
	ADF4351_Reg2.b.RMul2 = ADF4351_REFMUL_1;
	ADF4351_Reg2.b.CPCurrent = ADF4351_CPCURRENT_1_25;
	ADF4351_Reg2.b.CPTristate = ADF4351_DISABLE;
	ADF4351_Reg2.b.DoubleBuffer  = ADF4351_DISABLE;
	ADF4351_Reg2.b.LockFunction = ADF4351_LDF_FRAC;
	ADF4351_Reg2.b.LockPrecision = ADF4351_LDP_10NS;
	ADF4351_Reg2.b.LowNoiseSpur = ADF4351_LOW_NOISE_MODE;
	ADF4351_Reg2.b.MuxOut = ADF4351_MUX_DIGITALLOCK;
	ADF4351_Reg2.b.PhasePolarity = ADF4351_POLARITY_POSITIVE;
	ADF4351_Reg2.b.PowerDown = ADF4351_DISABLE;
	ADF4351_Reg2.b.RCountVal = 3;

	ADF4351_Reg3.b.AntibacklashW = ADF4351_ABP_6NS;
	ADF4351_Reg3.b.BandSelMode = ADF4351_BANDCLOCK_LOW;
	ADF4351_Reg3.b.ChargeCh = ADF4351_DISABLE;
	ADF4351_Reg3.b.ClkDivMod = ADF4351_CLKDIVMODE_OFF;
	ADF4351_Reg3.b.ClkDivVal = 150;
	ADF4351_Reg3.b.CsrEn = ADF4351_DISABLE;

	if(PTT_state==1) //TX
	{
		ADF4351_Reg4.b.AuxEnable = ADF4351_ENABLE;
		ADF4351_Reg4.b.Mtld = ADF4351_DISABLE;
		ADF4351_Reg4.b.OutEnable = ADF4351_ENABLE;
		ADF4351_Reg4.b.OutPower = ADF4351_POWER_PLUS5DB;
		ADF4351_Reg4.b.VcoPowerDown =  ADF4351_DISABLE;
		ADF4351_Reg4.b.Feedback = ADF4351_FEEDBACK_FUNDAMENTAL;

		ADF4351_Reg4.b.AuxPower	= ADF4351_POWER_PLUS5DB; //		:2;  		/*!< bit:  6.. 7 	AUX Output Power */
		ADF4351_Reg4.b.AuxSel = ADF4351_FEEDBACK_DIVIDED; //				:1;  		/*!< bit:  9 	AUX Output Select */
		//ADF4351_Reg4.b.RfDivSel = 5;
	}
	else if(PTT_state == 0) //RX
	{
		ADF4351_Reg4.b.AuxEnable = ADF4351_DISABLE;
		ADF4351_Reg4.b.Mtld = ADF4351_DISABLE;
		ADF4351_Reg4.b.OutEnable = ADF4351_ENABLE;
		ADF4351_Reg4.b.OutPower = ADF4351_POWER_PLUS5DB;
		ADF4351_Reg4.b.VcoPowerDown =  ADF4351_DISABLE;
		ADF4351_Reg4.b.Feedback = ADF4351_FEEDBACK_FUNDAMENTAL;

		ADF4351_Reg4.b.AuxPower	= ADF4351_POWER_PLUS5DB; //		:2;  		/*!< bit:  6.. 7 	AUX Output Power */
		ADF4351_Reg4.b.AuxSel = ADF4351_FEEDBACK_DIVIDED; //				:1;  		/*!< bit:  9 	AUX Output Select */
		//ADF4351_Reg4.b.RfDivSel = 5;
	}
	ADF4351_Reg5.b.LdPinMode = ADF4351_LD_PIN_DIGITAL_LOCK;
	//	ST_REG_Load();
	//	ST_REG_Update();
	/*CurrentFreq += 1e6;
	if (CurrentFreq > 3410e6) CurrentFreq = 3402e6;*/
}
void ADF4351_Write(uint32_t Reg_data)
{
	uint8_t spi_tx_data[4];

	spi_tx_data[0] = Reg_data>>24;
	spi_tx_data[1] = Reg_data>>16;
	spi_tx_data[2] = Reg_data>>8;
	spi_tx_data[3] = Reg_data&0xFF;
	//////////////spi tramsmit///////////////////////
	HAL_GPIO_WritePin(SPI1_NSS_4351_GPIO_Port,SPI1_NSS_4351_Pin,GPIO_PIN_RESET);

	HAL_SPI_Transmit(&hspi4,spi_tx_data,4,100);

	HAL_GPIO_WritePin(SPI1_NSS_4351_GPIO_Port,SPI1_NSS_4351_Pin,GPIO_PIN_SET);;
}
void Keypad_Frequency_RX(double freq)
{
	CurrentFreq=freq;
	RX_CurrentFreq = CurrentFreq+38.85;
	RX_CurrentFreq = RX_CurrentFreq*1000000;
	ADF3451_REG_Init();
	UpdateFrequencyRegisters(RX_CurrentFreq, 26000000.0, 8330, 1, 1, &calcfreq);
	PLL_Sync();
//	HAL_Delay(2000);
}
void PTT_BT_Pressed_TX(double freq)
{
	PTT_state=1;
	if(PTT_state!=prev_PTT_state)
	{
		prev_PTT_state=PTT_state;
		CurrentFreq=freq;
		ADF3451_REG_Init();
		TX_CurrentFreq = CurrentFreq*1000000;
		//TX_CurrentFreq = TX_CurrentFreq -35;
		UpdateFrequencyRegisters(TX_CurrentFreq, 26000000.0, 8330, 1, 1, &calcfreq);
		PLL_Sync();
	}

}
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim)
{
	//printf("Running Timer\r\n");
//	Key = Read_keypad();
//	if(!(Key==0))
//	{
//		Keydata=Key;
//		printf("Key Pressed: %c\r\n",Key);
//	}
	if(PTT_Switch_state==0 && Frequency_Int>=118 && Frequency_Int<=136.97500)
	{
		DAC3_Voltage = 0;
		DAC3_Generating(DAC3_Voltage);
		AGC_Enable(1);
	}
}
void Delay(void)
{
	HAL_Delay(2000);
}
// EXTI Line9 External Interrupt ISR Handler CallBackFun
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(HAL_GPIO_ReadPin(PTT_BUTTON_GPIO_Port, PTT_BUTTON_Pin))
	{
		printf("PTT LOW\r\n");
		PTT_RX_State =1;
		T_R_CTRL_0;
		TX_CTRL_0;
		TX_CTRL1_0
		RX_CTRL_1;
		VSWR_State_Control = 0;
		PTT_state=0;
		VSWR_State = 0;
		PTT_Switch_state =0;
		PTT_OLED_Erase =0;
		Menu=0;
		Inc = 0;
//		Manual_FRQ_Entry = 0;
		if(Manual_FRQ_Entry==1)
			Enter =1;
		else if(Manual_FRQ_Entry==2)
			Enter =1;
		else
			Enter = 0;
		PTT_OLED_Erase_state = 1;
		Page_Erase0=0,Page_Erase1=0,Page_Erase2=0,Page_Erase3=0,Page_Erase4=0;
		Prev_Channel_Int =0;//Prev_Channel_Int +1;
		Prev_Rssi_Int = 0;//Prev_Rssi_Int+1;
		if(Prev_Channel_Int == Channel_Int)
			Prev_Channel_Int = 251;
		else
			Prev_Channel_Int =0;//Prev_Channel_Int +1;
	}
	else
	{
		printf("PTT HIGH\r\n");
		Menu=0;
		Enter = 0;
		Inc = 0;
		Page_Erase0=0,Page_Erase1=0,Page_Erase2=0,Page_Erase3=0,Page_Erase4=0;
		if(Prev_Channel_Int == Channel_Int)
			Prev_Channel_Int = 250;
		else
			Prev_Channel_Int =0;//Prev_Channel_Int +1;
		Prev_Rssi_Int = 0;//Prev_Rssi_Int+1;
		Prev_Frequency_Int=0;//Prev_Frequency_Int+1;
		PTT_Switch_state =1;
		PTT_RX_State =0;
		T_R_CTRL_1;
		TX_CTRL_1;
		TX_CTRL1_1
		RX_CTRL_0;
		if(Prev_VSWR_State == VSWR_State)
		{
			Prev_VSWR_State = Prev_VSWR_State+2;
			if(Prev_VSWR_State>10)
			{
				Prev_VSWR_State = 2;
			}
		}
		else
			Prev_VSWR_State = 5;
	}
}
void TX_RX_Controls(void)
{
	if(PTT_Switch_state==1)
	{
		HAL_Delay(100);
		AVF_Volt = AVF_Reading();
		AVF_Volt = AVF_Reading();
		printf("AVF_Volt:%f\r\n",AVF_Volt);
		AVR_Volt = AVR_Reading();
		AVR_Volt = AVR_Reading();
		printf("AVR_Volt:%f\r\n",AVR_Volt);
		VSWR_Volt = AVR_Volt/AVF_Volt;
		printf("VSWR_Ratio: %f\r\n",VSWR_Volt);
		if(AVF_Volt >= 0.080)
		{
			HAL_Delay(10);
			if(VSWR_Volt <= 0.52)
			{
				VSWR_State =1;
				//printf("VSWR_Volt <= 0.45\r\n");
				printf("VSWR_State: %d\r\n",VSWR_State);
				AVTEMP_Volt= AVTEMP_Reading();
				AVTEMP_Volt= AVTEMP_Reading();
				//if(AVTEMP_Volt/2<1.65) //AVTeamp state
				{
//					AVTEMP_State=1; //Enable get reading from RF
					T_R_CTRL_1; // T/R Control pin High
					TX_CTRL_1; // TX control pin High
					TX_CTRL1_1;
					RX_CTRL_0; //RX Control pin Low
					//printf("Enabling TX\r\n");
					//printf("AVTEMP_State: %d\r\n",AVTEMP_State);

				}
//				else
//				{
//					AVTEMP_State=0;
//					VSWR_State =0;
//					//printf("Disabling TX\r\n");
//				}
			}
			else if(VSWR_Volt > 0.52)
			{
				VSWR_State =0;
				T_R_CTRL_0; // T/R Control pin High
				TX_CTRL_0; // TX control pin High
				TX_CTRL1_0;
				RX_CTRL_1; //RX Control pin Low
				//printf("VSWR_Volt > 0.45\r\n");
				printf("VSWR_State: %d\r\n",VSWR_State);
			}
		}
		else if(AVF_Volt < 0.080)
		{
			HAL_Delay(5);
			//printf("AVF_Volt < 0.3\r\n");
			VSWR_State =0;//chk
			T_R_CTRL_0; // T/R Control pin High
			TX_CTRL_0; // TX control pin High
			TX_CTRL1_0;
			RX_CTRL_1; //RX Control pin Low//==
		}
		if(VSWR_State == 1)
		{
			DAC3_Voltage = 1.93;
			DAC3_Generating(DAC3_Voltage);
		}
		else
		{
			DAC3_Voltage = 0;
			DAC3_Generating(DAC3_Voltage);
		}
	}
	else
	{
		//printf("Receiving Mode\r\n");
		VSWR_State =0;
		T_R_CTRL_0; // T/R Control pin High
		TX_CTRL_0; // TX control pin High
		TX_CTRL1_0;
		RX_CTRL_1; //RX Control pin Low
	}
	if(PTT_Switch_state == 1 && VSWR_State == 0)
	{
		HAL_Delay(5);
		AVF_Volt = AVF_Reading();
		HAL_Delay(5);
		AVF_Volt = AVF_Reading();
		HAL_Delay(5);
		AVR_Volt = AVR_Reading();
		HAL_Delay(5);
		AVR_Volt = AVR_Reading();
		HAL_Delay(5);
		VSWR_Volt = AVR_Volt/AVF_Volt;
		if(AVF_Volt >= 0.080)
		{
			if(VSWR_Volt <= 0.52)
			{
				VSWR_State =1;
				T_R_CTRL_1; // T/R Control pin High
				TX_CTRL_1; // TX control pin High
				TX_CTRL1_1
				RX_CTRL_0; //RX Control pin Low
			}
			else
			{
				VSWR_State =0;
				T_R_CTRL_0; // T/R Control pin High
				TX_CTRL_0; // TX control pin High
				TX_CTRL1_0;
				RX_CTRL_1; //RX Control pin Low

			}
		}
		if(VSWR_State == 1)
		{
			DAC3_Voltage = 1.93;
			DAC3_Generating(DAC3_Voltage);
		}
		else
		{
			DAC3_Voltage = 0;
			DAC3_Generating(DAC3_Voltage);
		}
		if(VSWR_State == 0)
		{
			if(VSWR_State_Control == 0)
			{
				printf("TX Power disabling due to VSWR State = 0\r\n");
				VSWR_State_Control =1;
				ADF4351_Reg4.b.AuxEnable = ADF4351_DISABLE;
				ADF4351_Reg4.b.Mtld = ADF4351_DISABLE;
				ADF4351_Reg4.b.OutEnable = ADF4351_DISABLE;
				ADF4351_Reg4.b.OutPower = ADF4351_POWER_PLUS5DB;
				ADF4351_Reg4.b.VcoPowerDown =  ADF4351_DISABLE;
				ADF4351_Reg4.b.Feedback = ADF4351_FEEDBACK_FUNDAMENTAL;

				ADF4351_Reg4.b.AuxPower	= ADF4351_POWER_PLUS5DB; //		:2;  		/*!< bit:  6.. 7 	AUX Output Power */
				ADF4351_Reg4.b.AuxSel = ADF4351_FEEDBACK_DIVIDED; //				:1;  		/*!< bit:  9 	AUX Output Select */
				//ADF4351_Reg4.b.RfDivSel = 5;
				//ADF3451_REG_Init(); //Disable TX current frequency Yellow
				adf4351_write(ADF4351_GetRegisterBuf(4));	//	Write registers to PLL chip
			}
		}
	}
}

PUTCHAR_PROTOTYPE
{
	/* Place your implementation of fputc here */
	/* e.g. write a character to the USART1 and Loop until the end of transmission */
	HAL_UART_Transmit(&huart3, (uint8_t *)&ch, 1, 0xFFFF);
	return ch;
}

#define M2401_DEVICE_ADDRESS_WR 0xB0
#define M2401_DEVICE_ADDRESS_RD 0xB1
char m24_id;
#define DEV_ADDR 0xa0
uint8_t dataw1[50] = "hello world from EEPROM";
uint8_t dataw2[50] = "This is the second string from EEPROM";
float dataw3 = 1234.5678;
uint8_t datar1[50];
uint8_t datar2[50];
float datar3;
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
 	unsigned char AIR_Band[22]="AIRBAND TRANSCEIVER";

	unsigned char AIR_Band1[16]="\"ICS-ABR-V01TR\"";
	//unsigned char AIR_Band1[16]="\"ICS-ABR-02VT\"";
	//unsigned char AIR_Band1[16]="\"ICS-ABR-03VT\"";
	//unsigned char AIR_Band1[16]="\"ICS-ABR-04VT\"";
//	int i=0;
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

/* Configure the peripherals common clocks */
  PeriphCommonClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_I2C1_Init();
	MX_SPI2_Init();
	MX_TIM2_Init();
	MX_USART3_UART_Init();
	MX_SPI4_Init();
	MX_ADC1_Init();
	MX_ADC2_Init();
	MX_DAC1_Init();
	MX_SPI5_Init();
	MX_I2C4_Init();
	MX_DAC2_Init();
  /* USER CODE BEGIN 2 */
	T_R_CTRL_0; // T/R Control pin High
	TX_CTRL_0; // TX control pin High
	TX_CTRL1_0;
	RX_CTRL_0; //RX Control pin Low
	VG_CTRL_0;
	LNA_CTRL_0;
	RX_AGC_CTRL_0;
	RX_ATTN_CTRL_0;
	RX_MUTE_CTRL_0;
	IF_AGC_CTRL_0;
	printf("ABR_Project\r\n");
	HAL_TIM_Base_Start_IT(&htim2);
//	W25Nxx_Init();
//	W25Nxx_EraseBlock(0x00);
	OLED_Init_12832();
//	for (int i=0; i<13; i++)
//	{
//	  EEPROM_PageErase(i);
//	}
	EEPROM_Read(0, 5, (unsigned char*)Read_Saved_Password, 4);
	printf("Read_Saved_Password:%s\r\n",Read_Saved_Password);
	if(Read_Saved_Password[0] == 0xff)
	{
		memset(Read_Saved_Password,0,sizeof(Read_Saved_Password));
		if(Read_Saved_Password[0]==0)
		{
			Read_Saved_Password[strlen(Read_Saved_Password)] = '0';
		}
	}
	else
	{
		memcpy(Saved_Password,Read_Saved_Password,4);
	}
	if(Read_Saved_Password[0]==0)
	{
		Read_Saved_Password[strlen(Read_Saved_Password)] = '0';
	}
	if(Saved_Password[0]==0)
	{
		Saved_Password[strlen(Saved_Password)] = '0';
	}
//	printf("Saved_Password:%s\r\n",Saved_Password);
	Current_Freq_Read((unsigned char *)Current_Freq, (unsigned char *)Current_CH);
	if(Current_Freq[0] == 0xff)
	{
		memset(Current_Freq,0,sizeof(Current_Freq));
		memset(Current_CH,0,sizeof(Current_CH));
		if(Current_Freq[0]==0)
		{
			Current_Freq[strlen(Current_Freq)] = '0';
		}
		if(Current_CH[0]==0)
		{
			Current_CH[strlen(Current_CH)] = '0';
		}
	}
	if(Current_Freq[0]==0)
	{
		Current_Freq[strlen(Current_Freq)] = '0';
	}
	if(Current_CH[0]==0)
	{
		Current_CH[strlen(Current_CH)] = '0';
	}
	printf("Read Freq:%s \t,CH: %s\r\n",Current_Freq,Current_CH);
	if(Prev_Channel_Int == Channel_Int)
		Prev_Channel_Int = 250;
	else
		Prev_Channel_Int =0;//Prev_Channel_Int +1;
	if(Prev_Frequency_Int == Prev_Frequency_Int)
		Prev_Frequency_Int = 249;
		else
			Prev_Frequency_Int =0;
	Set_Column_Address_12832(0x00,0x83);
	Fill_RAM_12832(0x00);
	OLED_Launch_Screen(epd_bitmap_ics_logo);
	HAL_Delay(2000);
	Fill_RAM_12832(0x00);
	Show_String_12832(1,AIR_Band,1,04);
	Show_String_12832(1,AIR_Band1,3,20);
	HAL_Delay(1000);
  	WIZNET_Inint_Process(); //Init process and print MAC and IP address
	Fill_RAM_12832(0x00);
	PTT_RX_State = 1;
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	while (1)
	{
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		AVF_Volt = AVF_Reading();
		AVR_Volt = AVR_Reading();
		VSWR_Volt = AVR_Volt/AVF_Volt;
		//	printf("AVF_Volt:%f\r\n",AVF_Volt);
		//	printf("AVR_Volt:%f\r\n",AVR_Volt);
		//DVF_Volt= DVF_Reading();
		//DVR_Volt= DVR_Reading();
		ARSSI_Volt= ARSSI_Reading();
		//DRSSI_Volt= DRSSI_Reading();
		//AVTEMP_Volt= AVTEMP_Reading();
		//DVTEMP_Volt= DVTEMP_Reading();
		AMRSSI_Volt = AMRSSI_Reading();
		Audio_out_volt=Audio_out_reading();
		//	printf("DVF_Volt: %f\r\n",DVF_Volt);
		//	printf("DVR_Volt: %f\r\n",DVR_Volt);
		//	printf("ARSSI_Volt: %f\r\n",ARSSI_Volt);
		//	printf("DRSSI_Volt: %f\r\n",DRSSI_Volt);
		//	printf("AVTEMP_Volt: %f\r\n",AVTEMP_Volt);
		//	printf("DVTEMP_Volt: %f\r\n",DVTEMP_Volt);
		//printf("\r\nAMRSSI_Volt: %f\t",AMRSSI_Volt);
		//printf("\r\nAudio_out_volt: %f\t",Audio_out_volt);
//		if(PTT_Switch_state==0 && Frequency_Int>=118 && Frequency_Int<=136.97500)
//		{
//			DAC3_Voltage = 0;
//			DAC3_Generating(DAC3_Voltage);
//			AGC_Enable(1);
//		}
		Key = Read_keypad();
		if(!(Key==0))
		{
			Keydata=Key;
			printf("Key: %c\r\n",Key);
			HAL_Delay(200);
		}
//		if(AMRSSI_Volt == 27 && DAC2_Voltage>=1.6 && DAC1_Voltage>=1.4 && DAC1_Voltage<1.85 )
//		{
//			if(DAC1_state == 0)
//			{
//				Default_DAC1_volt = 0.166;
//				DAC1_Voltage = DAC1_Voltage+Default_DAC1_volt;
//				DAC1_Generating(DAC1_Voltage);
//				HAL_Delay(100);
//			}
//		}
		Keypad_Application();
		Menu_OLED_Application();

		SQ_Check_State();
		//if(PTT_Switch_state!=1)
		{
			Ethernet_TCP_UDP_RUN(); //Enable after testing
		}
		if(PTT_Switch_state == 0)
		{
			SQ_T_H_Val_int = atoi(SQ_T_H_Val);
			if(SQ_T_H_Val_int > 4)
				SQ_T_H_Val_int = 4;
			if(SQ_T_H_Val_int == 0)
			{
				RX_MUTE_CTRL_1;//Enable speaker output
				Audio_Speaker_State = 1;
			}
			else if(SQ_T_H_Val_int == 1)
			{
				if(Audio_out_volt<0.290) //1-335,2-290,3 and 4-365,
				{
					RX_MUTE_CTRL_0;//no speaker output
					Audio_Speaker_State = 0;
				}
				if(Audio_out_volt>0.310) //1-375,2-310,3-385,
				{
					RX_MUTE_CTRL_1;//Enable speaker output
					Audio_Speaker_State = 1;
				}
			}
			else if(SQ_T_H_Val_int == 2)
			{
				if(Audio_out_volt<0.510) //1-555,2-510,3 and 4-585
				{
					RX_MUTE_CTRL_0;//no speaker output
					Audio_Speaker_State = 0;
				}
				if(Audio_out_volt>0.550) //1-595,2-550,3-625
				{
					RX_MUTE_CTRL_1;//Enable speaker output
					Audio_Speaker_State = 1;
				}
			}
			else if(SQ_T_H_Val_int == 3)
			{
				if(Audio_out_volt<0.530) //1-575,2-530,3 and 4-605
				{
					RX_MUTE_CTRL_0;//no speaker output
					Audio_Speaker_State = 0;
				}
				if(Audio_out_volt>0.570)//1-615,2-570,3 and 4-645
				{
					RX_MUTE_CTRL_1;//Enable speaker output
					Audio_Speaker_State = 1;
				}
			}
			else if(SQ_T_H_Val_int == 4)
			{
				if(Audio_out_volt<0.590) //1-635,2-590,3-665
				{
					RX_MUTE_CTRL_0;//no speaker output
					Audio_Speaker_State = 0;
				}
				if(Audio_out_volt>0.630)//1-675,2-630,3-705
				{
					RX_MUTE_CTRL_1;//Enable speaker output
					Audio_Speaker_State = 1;
				}
			}
		}
		//HAL_Delay(100);
	}
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /*AXI clock gating */
  RCC->CKGAENR = 0xFFFFFFFF;

  /** Supply configuration update enable
  */
  HAL_PWREx_ConfigSupply(PWR_DIRECT_SMPS_SUPPLY);

  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

  /** Macro to configure the PLL clock source
  */
  __HAL_RCC_PLL_PLLSOURCE_CONFIG(RCC_PLLSOURCE_HSI);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_DIV1;
  RCC_OscInitStruct.HSICalibrationValue = 64;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 25;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_3;
  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_D3PCLK1|RCC_CLOCKTYPE_D1PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief Peripherals Common Clock Configuration
  * @retval None
  */
void PeriphCommonClock_Config(void)
{
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Initializes the peripherals clock
  */
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInitStruct.PLL2.PLL2M = 4;
  PeriphClkInitStruct.PLL2.PLL2N = 8;
  PeriphClkInitStruct.PLL2.PLL2P = 3;
  PeriphClkInitStruct.PLL2.PLL2Q = 2;
  PeriphClkInitStruct.PLL2.PLL2R = 2;
  PeriphClkInitStruct.PLL2.PLL2RGE = RCC_PLL2VCIRANGE_3;
  PeriphClkInitStruct.PLL2.PLL2VCOSEL = RCC_PLL2VCOWIDE;
  PeriphClkInitStruct.PLL2.PLL2FRACN = 0;
  PeriphClkInitStruct.AdcClockSelection = RCC_ADCCLKSOURCE_PLL2;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_MultiModeTypeDef multimode = {0};
  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Common config
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV4;
  hadc1.Init.Resolution = ADC_RESOLUTION_8B;
  hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc1.Init.LowPowerAutoWait = DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.ConversionDataManagement = ADC_CONVERSIONDATA_DR;
  hadc1.Init.Overrun = ADC_OVR_DATA_PRESERVED;
  hadc1.Init.LeftBitShift = ADC_LEFTBITSHIFT_NONE;
  hadc1.Init.OversamplingMode = DISABLE;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure the ADC multi-mode
  */
  multimode.Mode = ADC_MODE_INDEPENDENT;
  if (HAL_ADCEx_MultiModeConfigChannel(&hadc1, &multimode) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_8;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_64CYCLES_5;
  sConfig.SingleDiff = ADC_SINGLE_ENDED;
  sConfig.OffsetNumber = ADC_OFFSET_NONE;
  sConfig.Offset = 0;
  sConfig.OffsetSignedSaturation = DISABLE;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief ADC2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC2_Init(void)
{

  /* USER CODE BEGIN ADC2_Init 0 */

  /* USER CODE END ADC2_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC2_Init 1 */

  /* USER CODE END ADC2_Init 1 */

  /** Common config
  */
  hadc2.Instance = ADC2;
  hadc2.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV4;
  hadc2.Init.Resolution = ADC_RESOLUTION_8B;
  hadc2.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc2.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc2.Init.LowPowerAutoWait = DISABLE;
  hadc2.Init.ContinuousConvMode = DISABLE;
  hadc2.Init.NbrOfConversion = 1;
  hadc2.Init.DiscontinuousConvMode = DISABLE;
  hadc2.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc2.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc2.Init.ConversionDataManagement = ADC_CONVERSIONDATA_DR;
  hadc2.Init.Overrun = ADC_OVR_DATA_PRESERVED;
  hadc2.Init.LeftBitShift = ADC_LEFTBITSHIFT_NONE;
  hadc2.Init.OversamplingMode = DISABLE;
  if (HAL_ADC_Init(&hadc2) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_1;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_64CYCLES_5;
  sConfig.SingleDiff = ADC_SINGLE_ENDED;
  sConfig.OffsetNumber = ADC_OFFSET_NONE;
  sConfig.Offset = 0;
  sConfig.OffsetSignedSaturation = DISABLE;
  if (HAL_ADC_ConfigChannel(&hadc2, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC2_Init 2 */

  /* USER CODE END ADC2_Init 2 */

}

/**
  * @brief DAC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_DAC1_Init(void)
{

  /* USER CODE BEGIN DAC1_Init 0 */

  /* USER CODE END DAC1_Init 0 */

  DAC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN DAC1_Init 1 */

  /* USER CODE END DAC1_Init 1 */

  /** DAC Initialization
  */
  hdac1.Instance = DAC1;
  if (HAL_DAC_Init(&hdac1) != HAL_OK)
  {
    Error_Handler();
  }

  /** DAC channel OUT1 config
  */
  sConfig.DAC_SampleAndHold = DAC_SAMPLEANDHOLD_DISABLE;
  sConfig.DAC_Trigger = DAC_TRIGGER_NONE;
  sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;
  sConfig.DAC_ConnectOnChipPeripheral = DAC_CHIPCONNECT_DISABLE;
  sConfig.DAC_UserTrimming = DAC_TRIMMING_FACTORY;
  if (HAL_DAC_ConfigChannel(&hdac1, &sConfig, DAC_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }

  /** DAC channel OUT2 config
  */
  if (HAL_DAC_ConfigChannel(&hdac1, &sConfig, DAC_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN DAC1_Init 2 */

  /* USER CODE END DAC1_Init 2 */

}

/**
  * @brief DAC2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_DAC2_Init(void)
{

  /* USER CODE BEGIN DAC2_Init 0 */

  /* USER CODE END DAC2_Init 0 */

  DAC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN DAC2_Init 1 */

  /* USER CODE END DAC2_Init 1 */

  /** DAC Initialization
  */
  hdac2.Instance = DAC2;
  if (HAL_DAC_Init(&hdac2) != HAL_OK)
  {
    Error_Handler();
  }

  /** DAC channel OUT1 config
  */
  sConfig.DAC_SampleAndHold = DAC_SAMPLEANDHOLD_DISABLE;
  sConfig.DAC_Trigger = DAC_TRIGGER_NONE;
  sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;
  sConfig.DAC_ConnectOnChipPeripheral = DAC_CHIPCONNECT_DISABLE;
  sConfig.DAC_UserTrimming = DAC_TRIMMING_FACTORY;
  if (HAL_DAC_ConfigChannel(&hdac2, &sConfig, DAC_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN DAC2_Init 2 */

  /* USER CODE END DAC2_Init 2 */

}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.Timing = 0x00401242;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
  {
    Error_Handler();
  }

  /** I2C Enable Fast Mode Plus
  */
  HAL_I2CEx_EnableFastModePlus(I2C_FASTMODEPLUS_I2C1);
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief I2C4 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C4_Init(void)
{

  /* USER CODE BEGIN I2C4_Init 0 */

  /* USER CODE END I2C4_Init 0 */

  /* USER CODE BEGIN I2C4_Init 1 */

  /* USER CODE END I2C4_Init 1 */
  hi2c4.Instance = I2C4;
  hi2c4.Init.Timing = 0x009034B6;
  hi2c4.Init.OwnAddress1 = 0;
  hi2c4.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c4.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c4.Init.OwnAddress2 = 0;
  hi2c4.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c4.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c4.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c4) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c4, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c4, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C4_Init 2 */

  /* USER CODE END I2C4_Init 2 */

}

/**
  * @brief SPI2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI2_Init(void)
{

  /* USER CODE BEGIN SPI2_Init 0 */

  /* USER CODE END SPI2_Init 0 */

  /* USER CODE BEGIN SPI2_Init 1 */

  /* USER CODE END SPI2_Init 1 */
  /* SPI2 parameter configuration*/
  hspi2.Instance = SPI2;
  hspi2.Init.Mode = SPI_MODE_MASTER;
  hspi2.Init.Direction = SPI_DIRECTION_2LINES;
  hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi2.Init.NSS = SPI_NSS_SOFT;
  hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;
  hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi2.Init.CRCPolynomial = 0x0;
  hspi2.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
  hspi2.Init.NSSPolarity = SPI_NSS_POLARITY_LOW;
  hspi2.Init.FifoThreshold = SPI_FIFO_THRESHOLD_01DATA;
  hspi2.Init.TxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
  hspi2.Init.RxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
  hspi2.Init.MasterSSIdleness = SPI_MASTER_SS_IDLENESS_00CYCLE;
  hspi2.Init.MasterInterDataIdleness = SPI_MASTER_INTERDATA_IDLENESS_00CYCLE;
  hspi2.Init.MasterReceiverAutoSusp = SPI_MASTER_RX_AUTOSUSP_DISABLE;
  hspi2.Init.MasterKeepIOState = SPI_MASTER_KEEP_IO_STATE_DISABLE;
  hspi2.Init.IOSwap = SPI_IO_SWAP_DISABLE;
  if (HAL_SPI_Init(&hspi2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI2_Init 2 */

  /* USER CODE END SPI2_Init 2 */

}

/**
  * @brief SPI4 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI4_Init(void)
{

  /* USER CODE BEGIN SPI4_Init 0 */

  /* USER CODE END SPI4_Init 0 */

  /* USER CODE BEGIN SPI4_Init 1 */

  /* USER CODE END SPI4_Init 1 */
  /* SPI4 parameter configuration*/
  hspi4.Instance = SPI4;
  hspi4.Init.Mode = SPI_MODE_MASTER;
  hspi4.Init.Direction = SPI_DIRECTION_1LINE;
  hspi4.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi4.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi4.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi4.Init.NSS = SPI_NSS_SOFT;
  hspi4.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;
  hspi4.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi4.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi4.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi4.Init.CRCPolynomial = 0x0;
  hspi4.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
  hspi4.Init.NSSPolarity = SPI_NSS_POLARITY_LOW;
  hspi4.Init.FifoThreshold = SPI_FIFO_THRESHOLD_01DATA;
  hspi4.Init.TxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
  hspi4.Init.RxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
  hspi4.Init.MasterSSIdleness = SPI_MASTER_SS_IDLENESS_00CYCLE;
  hspi4.Init.MasterInterDataIdleness = SPI_MASTER_INTERDATA_IDLENESS_00CYCLE;
  hspi4.Init.MasterReceiverAutoSusp = SPI_MASTER_RX_AUTOSUSP_DISABLE;
  hspi4.Init.MasterKeepIOState = SPI_MASTER_KEEP_IO_STATE_DISABLE;
  hspi4.Init.IOSwap = SPI_IO_SWAP_DISABLE;
  if (HAL_SPI_Init(&hspi4) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI4_Init 2 */

  /* USER CODE END SPI4_Init 2 */

}

/**
  * @brief SPI5 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI5_Init(void)
{

  /* USER CODE BEGIN SPI5_Init 0 */

  /* USER CODE END SPI5_Init 0 */

  /* USER CODE BEGIN SPI5_Init 1 */

  /* USER CODE END SPI5_Init 1 */
  /* SPI5 parameter configuration*/
  hspi5.Instance = SPI5;
  hspi5.Init.Mode = SPI_MODE_MASTER;
  hspi5.Init.Direction = SPI_DIRECTION_2LINES;
  hspi5.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi5.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi5.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi5.Init.NSS = SPI_NSS_SOFT;
  hspi5.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
  hspi5.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi5.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi5.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi5.Init.CRCPolynomial = 0x0;
  hspi5.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
  hspi5.Init.NSSPolarity = SPI_NSS_POLARITY_LOW;
  hspi5.Init.FifoThreshold = SPI_FIFO_THRESHOLD_01DATA;
  hspi5.Init.TxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
  hspi5.Init.RxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
  hspi5.Init.MasterSSIdleness = SPI_MASTER_SS_IDLENESS_00CYCLE;
  hspi5.Init.MasterInterDataIdleness = SPI_MASTER_INTERDATA_IDLENESS_00CYCLE;
  hspi5.Init.MasterReceiverAutoSusp = SPI_MASTER_RX_AUTOSUSP_DISABLE;
  hspi5.Init.MasterKeepIOState = SPI_MASTER_KEEP_IO_STATE_DISABLE;
  hspi5.Init.IOSwap = SPI_IO_SWAP_DISABLE;
  if (HAL_SPI_Init(&hspi5) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI5_Init 2 */

  /* USER CODE END SPI5_Init 2 */

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 0;//3000;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 25000;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
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
  huart3.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart3.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart3.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&huart3, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&huart3, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_DisableFifoMode(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, RX_CTRL_CC_Pin|SPI1_NSS_4351_Pin|ADF4351_LD_Pin|VG_CTRL_CC_Pin
                          |LNA_CTRL_CC_Pin|RX_AGC_CTRL_CC_Pin|RX_ATTN_CTRL_CC_Pin|T_R_CTRL_CC_Pin
                          |TX_CTRL_CC_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, IF_AGC_CTRL_CC_Pin|RX_MUTE_CTRL_CC_Pin|WIZ_CS_Pin|WIZ_RST_Pin
                          |SPI5_CS_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, Keypad_C3_Pin|Keypad_C2_Pin|Keypad_C1_Pin|Keypad_C0_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, OLED_SA0_Pin|OLED_Reset_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(TX_CTRL1_GPIO_Port, TX_CTRL1_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : RX_CTRL_CC_Pin SPI1_NSS_4351_Pin ADF4351_LD_Pin VG_CTRL_CC_Pin
                           LNA_CTRL_CC_Pin RX_AGC_CTRL_CC_Pin RX_ATTN_CTRL_CC_Pin T_R_CTRL_CC_Pin
                           TX_CTRL_CC_Pin */
  GPIO_InitStruct.Pin = RX_CTRL_CC_Pin|SPI1_NSS_4351_Pin|ADF4351_LD_Pin|VG_CTRL_CC_Pin
                          |LNA_CTRL_CC_Pin|RX_AGC_CTRL_CC_Pin|RX_ATTN_CTRL_CC_Pin|T_R_CTRL_CC_Pin
                          |TX_CTRL_CC_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : IF_AGC_CTRL_CC_Pin RX_MUTE_CTRL_CC_Pin WIZ_CS_Pin WIZ_RST_Pin
                           SPI5_CS_Pin */
  GPIO_InitStruct.Pin = IF_AGC_CTRL_CC_Pin|RX_MUTE_CTRL_CC_Pin|WIZ_CS_Pin|WIZ_RST_Pin
                          |SPI5_CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : Keypad_R3_Pin Keypad_R2_Pin Keypad_R1_Pin Keypad_R0_Pin */
  GPIO_InitStruct.Pin = Keypad_R3_Pin|Keypad_R2_Pin|Keypad_R1_Pin|Keypad_R0_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pin : PTT_BUTTON_Pin */
  GPIO_InitStruct.Pin = PTT_BUTTON_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(PTT_BUTTON_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : Keypad_C3_Pin Keypad_C2_Pin Keypad_C1_Pin Keypad_C0_Pin */
  GPIO_InitStruct.Pin = Keypad_C3_Pin|Keypad_C2_Pin|Keypad_C1_Pin|Keypad_C0_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : OLED_SA0_Pin OLED_Reset_Pin */
  GPIO_InitStruct.Pin = OLED_SA0_Pin|OLED_Reset_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pin : TX_CTRL1_Pin */
  GPIO_InitStruct.Pin = TX_CTRL1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(TX_CTRL1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : EX_RF_Pin */
  GPIO_InitStruct.Pin = EX_RF_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(EX_RF_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : RF_VSWR_STATUS_Pin */
  GPIO_InitStruct.Pin = RF_VSWR_STATUS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(RF_VSWR_STATUS_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

}

/* USER CODE BEGIN 4 */
void AGC_Enable(uint8_t EN)
{
	/*DAC1_Voltage = 0.200;
	DAC2_Voltage = 1.6;
	if(DAC1_Voltage<1.85)
	{
		DAC1_Generating(DAC1_Voltage);
	}
	if(DAC2_Voltage<2.75)
	{
		DAC2_Generating(DAC2_Voltage);
	}*/

	if(EN==1)
	{
		if(AMRSSI_Volt>28)// && ARSSI_Volt<0.300)
		{
			Gain_Lock=0;
		}
		if(DAC2_Voltage<2.70 && RX_ATTN_TH==1 && ARSSI_Volt<1.18)
		{
			RX_ATTN_CTRL_0;
			RX_ATTN =0;
			RX_ATTN_TH = 0;
		}
		if(AMRSSI_Volt>27) //Devices 1,3,4 = 27 Device2=28
		{
			//printf("0-AMRSSI_Volt>27\r\n");
			if(DAC1_Voltage<1.85 && DAC2_Voltage<=1.6 && RX_ATTN_TH==0 && Gain_Lock==0)
			{
				DAC1_Voltage += 0.001;
				DAC2_Voltage = 1.6;
				DAC1_state = 0;
			}
			else if(DAC2_Voltage<2.75 && DAC1_Voltage>=1.85 && RX_ATTN_TH==0 && Gain_Lock==0)
			{
				DAC1_Voltage = 1.85;
				DAC2_Voltage += 0.001;
				if(DAC2_Voltage>2.68 && ARSSI_Volt>1.18)
				{
					RX_ATTN_CTRL_1;
					RX_ATTN =1;
					RX_ATTN_TH = 1;
					DAC2_Voltage = 2.4;
				}
			}
			/*else if(DAC2_Voltage<2.75 && DAC1_Voltage>=1.85 && RX_ATTN_TH==1 && Gain_Lock==0)
			{
				//if(ARSSI_Volt>1.12)
				{
					DAC1_Voltage = 1.85;
					DAC2_Voltage += 0.010;
				}
			}*/

			if(DAC1_Voltage<1.85)
			{
				DAC1_Generating(DAC1_Voltage);
			}
			if(DAC2_Voltage<2.75)
			{
				DAC2_Generating(DAC2_Voltage);
			}
		}
		else if(AMRSSI_Volt<27)
		{
			/*if(DAC1_Voltage<=0.740 && DAC1_Voltage>=0.500)
			{
				Gain_Lock = 0;
				DAC1_Voltage = 0.740;
				DAC2_Voltage = 1.6;
			}*/
			//printf("0-AMRSSI_Volt<27\r\n");
			if(DAC1_Voltage<=0.030)
			{
				Gain_Lock = 1;
				DAC1_Voltage = 0.030;
				DAC2_Voltage = 1.6;
			}
			if(DAC1_Voltage>0.030 && DAC2_Voltage<=1.6 && RX_ATTN_TH==0 && Gain_Lock==0)
			{
				DAC1_Voltage -= 0.001;
				DAC2_Voltage = 1.6;
				//DAC1_state = 0;

			}
			else if(DAC2_Voltage>1.6 && DAC1_Voltage>=1.85 && RX_ATTN_TH==0 && Gain_Lock==0)
			{
				DAC1_Voltage = 1.85;
				DAC2_Voltage -= 0.001;
			}
			/*else if(DAC2_Voltage>1.6 && DAC1_Voltage>=1.85 && RX_ATTN_TH==1 && Gain_Lock==0)
			{
				//if(ARSSI_Volt>1.12)
				{
					DAC2_Voltage-=0.010;
				}
			}*/
			if(DAC1_Voltage<1.85)
			{
				DAC1_Generating(DAC1_Voltage);
			}
			if(DAC2_Voltage<2.75)
			{
				DAC2_Generating(DAC2_Voltage);
			}
		}
		if(AMRSSI_Volt>27)
		{
			//printf("1-AMRSSI_Volt>27\r\n");
			if(DAC2_Voltage<2.75 && DAC1_Voltage>=1.85 && RX_ATTN_TH==1 && Gain_Lock==0)
			{
				if(ARSSI_Volt>1.12)
				{
					DAC1_Voltage = 1.85;
					DAC2_Voltage += 0.001;
				}
				if(DAC1_Voltage<1.85)
				{
					DAC1_Generating(DAC1_Voltage);
				}
				if(DAC2_Voltage<2.75)
				{
					DAC2_Generating(DAC2_Voltage);
				}
			}
		}
		else if(AMRSSI_Volt<27)
		{
			//printf("1-AMRSSI_Volt<27\r\n");
			if(DAC2_Voltage>1.6 && DAC1_Voltage>=1.85 && RX_ATTN_TH==1 && Gain_Lock==0)
			{
				if(ARSSI_Volt>1.12)
				{
					DAC2_Voltage-=0.001;
				}

				if(DAC1_Voltage<1.85)
				{
					//DAC1_Generating(DAC1_Voltage);
				}
				if(DAC2_Voltage<2.75)
				{
					DAC2_Generating(DAC2_Voltage);
				}
			}
		}
	}

//	if(DAC1_Voltage>1.85)
//	{
//		DAC1_Voltage=1.85;
//	}
//	if(DAC2_Voltage>2.75)
//	{
//		DAC2_Voltage=2.75;
//	}
	/*if(AMRSSI_Volt>0.260)
	{
		RX_MUTE_CTRL_1;
		Mute_CTRL = 1;
	}
	else
	{
		RX_MUTE_CTRL_0;
		Mute_CTRL = 0;
	}*/
}
void DAC1_Generating(double Value)
{
	uint32_t DAC_OUT1=0;
	if(Value<=1.85)
	{
		DAC_OUT1 = Value*(4096)/3.3;
		HAL_DAC_Start(&hdac1, DAC_CHANNEL_1);
		HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_1, DAC_ALIGN_12B_R, DAC_OUT1);
	}

}
void DAC2_Generating(double Value)
{
	uint32_t DAC_OUT2=0;
	if(Value<=2.75)
	{
		DAC_OUT2 = Value*(4096)/3.3;
		HAL_DAC_Start(&hdac1, DAC_CHANNEL_2);
		HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_2, DAC_ALIGN_12B_R, DAC_OUT2);
	}
}
void DAC3_Generating(double Value)
{
	uint32_t DAC_OUT3=0;
	if(Value<1.95)
	{
		DAC_OUT3 = Value*(4096)/3.3;
		HAL_DAC_Start(&hdac2, DAC_CHANNEL_1);
		HAL_DAC_SetValue(&hdac2, DAC_CHANNEL_1, DAC_ALIGN_12B_R, DAC_OUT3);
	}
}
double AVF_Reading(void)
{
	int i;
	double voltage_samples[100];
	uint32_t ADC_Samples=0;
	double ADC_Reading=0;
	sConfig.Channel = ADC_CHANNEL_8;
	sConfig.Rank = ADC_REGULAR_RANK_1;
	sConfig.SamplingTime = ADC_SAMPLETIME_8CYCLES_5;
	sConfig.SingleDiff = ADC_SINGLE_ENDED;
	sConfig.OffsetNumber = ADC_OFFSET_NONE;
	sConfig.Offset = 0;
	sConfig.OffsetSignedSaturation = DISABLE;
	if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
	{
		Error_Handler();
	}
	memset(voltage_samples,0,sizeof(voltage_samples));
	for(i=0;i<100;i++)
	{
		HAL_ADC_Start(&hadc1);
		HAL_ADC_PollForConversion(&hadc1, 1);
		ADC_Samples=HAL_ADC_GetValue(&hadc1);
		voltage_samples[i] = ADC_Samples;
//		if(PTT_Switch_state == 1)
//			printf("AVF_Samples[%d]:%ld\r\n",i,ADC_Samples);
	}
	ADC_Samples = voltage_rms(voltage_samples,100);
	ADC_Reading = ADC_Samples*(3.3/256);
	//printf("AVF Samples: %ld\r\n",ADC_Samples);
	//printf("volt1=%f\r\n",ADC_Reading);
	return ADC_Reading;
}
double Audio_out_reading(void)
{
	int i;
	double voltage_samples[100];
	uint32_t ADC_Samples=0;
	double ADC_Reading=0;
	sConfig.Channel = ADC_CHANNEL_7;
	sConfig.Rank = ADC_REGULAR_RANK_1;
	sConfig.SamplingTime = ADC_SAMPLETIME_8CYCLES_5;
	sConfig.SingleDiff = ADC_SINGLE_ENDED;
	sConfig.OffsetNumber = ADC_OFFSET_NONE;
	sConfig.Offset = 0;
	sConfig.OffsetSignedSaturation = DISABLE;
	if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
	{
		Error_Handler();
	}
	memset(voltage_samples,0,sizeof(voltage_samples));
	for(i=0;i<100;i++)
	{
		HAL_ADC_Start(&hadc1);
		HAL_ADC_PollForConversion(&hadc1, 1);
		ADC_Samples=HAL_ADC_GetValue(&hadc1);
		voltage_samples[i] = ADC_Samples;
	}

	ADC_Samples = voltage_rms(voltage_samples,100);
	ADC_Reading = ADC_Samples*(3.3/256);
	memset(voltage_samples,0,sizeof(voltage_samples));
	//printf("AVR Samples: %ld\r\n",ADC_Samples);
	//printf("volt2=%f\r\n",ADC_Reading);
	return ADC_Reading;
}
double AVR_Reading(void)
{
	int i;
	double voltage_samples[100];
	uint32_t ADC_Samples=0;
	double ADC_Reading=0;
	sConfig.Channel = ADC_CHANNEL_17;
	sConfig.Rank = ADC_REGULAR_RANK_1;
	sConfig.SamplingTime = ADC_SAMPLETIME_8CYCLES_5;
	sConfig.SingleDiff = ADC_SINGLE_ENDED;
	sConfig.OffsetNumber = ADC_OFFSET_NONE;
	sConfig.Offset = 0;
	sConfig.OffsetSignedSaturation = DISABLE;
	if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
	{
		Error_Handler();
	}
	memset(voltage_samples,0,sizeof(voltage_samples));
	for(i=0;i<100;i++)
	{
		HAL_ADC_Start(&hadc1);
		HAL_ADC_PollForConversion(&hadc1, 1);
		ADC_Samples=HAL_ADC_GetValue(&hadc1);
		voltage_samples[i] = ADC_Samples;
	}

	ADC_Samples = voltage_rms(voltage_samples,100);
	ADC_Reading = ADC_Samples*(3.3/256);
	memset(voltage_samples,0,sizeof(voltage_samples));
	//printf("AVR Samples: %ld\r\n",ADC_Samples);
	//printf("volt2=%f\r\n",ADC_Reading);
	return ADC_Reading;
}
double DVF_Reading(void)
{
	uint32_t ADC_Samples=0;
	double ADC_Reading=0;
	sConfig.Channel = ADC_CHANNEL_9;
	sConfig.Rank = ADC_REGULAR_RANK_1;
	sConfig.SamplingTime = ADC_SAMPLETIME_8CYCLES_5;
	sConfig.SingleDiff = ADC_SINGLE_ENDED;
	sConfig.OffsetNumber = ADC_OFFSET_NONE;
	sConfig.Offset = 0;
	sConfig.OffsetSignedSaturation = DISABLE;
	if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
	{
		Error_Handler();
	}
	HAL_ADC_Start(&hadc1);
	HAL_ADC_PollForConversion(&hadc1, 1);
	ADC_Samples=HAL_ADC_GetValue(&hadc1);
	ADC_Reading = ADC_Samples*(3.3/256);
	//printf("DVF Samples: %ld\r\n",ADC_Samples);
	//printf("volt2=%f\r\n",ADC_Reading);
	return ADC_Reading;
}
double DVR_Reading(void)
{
	uint32_t ADC_Samples=0;
	double ADC_Reading=0;
	sConfig.Channel = ADC_CHANNEL_16;
	sConfig.Rank = ADC_REGULAR_RANK_1;
	sConfig.SamplingTime = ADC_SAMPLETIME_8CYCLES_5;
	sConfig.SingleDiff = ADC_SINGLE_ENDED;
	sConfig.OffsetNumber = ADC_OFFSET_NONE;
	sConfig.Offset = 0;
	sConfig.OffsetSignedSaturation = DISABLE;
	if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
	{
		Error_Handler();
	}
	HAL_ADC_Start(&hadc1);
	HAL_ADC_PollForConversion(&hadc1, 1);
	ADC_Samples=HAL_ADC_GetValue(&hadc1);
	ADC_Reading = ADC_Samples*(3.3/256);
	//printf("DVR Samples: %ld\r\n",ADC_Samples);
	//printf("volt2=%f\r\n",ADC_Reading);
	return ADC_Reading;
}
double DRSSI_Reading(void)
{
	uint32_t ADC_Samples=0;
	double ADC_Reading=0;
	sConfig.Channel = ADC_CHANNEL_6;
	sConfig.Rank = ADC_REGULAR_RANK_1;
	sConfig.SamplingTime = ADC_SAMPLETIME_8CYCLES_5;
	sConfig.SingleDiff = ADC_SINGLE_ENDED;
	sConfig.OffsetNumber = ADC_OFFSET_NONE;
	sConfig.Offset = 0;
	sConfig.OffsetSignedSaturation = DISABLE;
	if (HAL_ADC_ConfigChannel(&hadc2, &sConfig) != HAL_OK)
	{
		Error_Handler();
	}
	HAL_ADC_Start(&hadc2);
	HAL_ADC_PollForConversion(&hadc2, 1);
	ADC_Samples=HAL_ADC_GetValue(&hadc2);
	ADC_Reading = ADC_Samples*(3.3/4096);
	//printf("DRSSI Samples: %ld\r\n",ADC_Samples);
	//printf("volt2=%f\r\n",ADC_Reading);
	return ADC_Reading;
}
double voltage_rms(double voltage_samples[], int num_samples) {
    double sum = 0;
    for (int i = 0; i < num_samples; i++) {
        sum += voltage_samples[i] * voltage_samples[i];
    }
    double mean = sum / num_samples;
    return sqrt(mean);
}
double AVTEMP_Reading(void)
{
	uint32_t ADC_Samples=0;
	double ADC_Reading=0;
	sConfig.Channel = ADC_CHANNEL_1;
	sConfig.Rank = ADC_REGULAR_RANK_1;
	sConfig.SamplingTime = ADC_SAMPLETIME_8CYCLES_5;
	sConfig.SingleDiff = ADC_SINGLE_ENDED;
	sConfig.OffsetNumber = ADC_OFFSET_NONE;
	sConfig.Offset = 0;
	sConfig.OffsetSignedSaturation = DISABLE;
	if (HAL_ADC_ConfigChannel(&hadc2, &sConfig) != HAL_OK)
	{
		Error_Handler();
	}
	HAL_ADC_Start(&hadc2);
	HAL_ADC_PollForConversion(&hadc2, 1);
	ADC_Samples=HAL_ADC_GetValue(&hadc2);
	ADC_Reading = ADC_Samples*(3.3/4096);
	//printf("AVTEMP Samples: %ld\r\n",ADC_Samples);
	//printf("volt2=%f\r\n",ADC_Reading);
	return ADC_Reading;
}
double DVTEMP_Reading(void)
{
	uint32_t ADC_Samples=0;
	double ADC_Reading=0;
	sConfig.Channel = ADC_CHANNEL_4;
	sConfig.Rank = ADC_REGULAR_RANK_1;
	sConfig.SamplingTime = ADC_SAMPLETIME_8CYCLES_5;
	sConfig.SingleDiff = ADC_SINGLE_ENDED;
	sConfig.OffsetNumber = ADC_OFFSET_NONE;
	sConfig.Offset = 0;
	sConfig.OffsetSignedSaturation = DISABLE;
	if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
	{
		Error_Handler();
	}
	HAL_ADC_Start(&hadc1);
	HAL_ADC_PollForConversion(&hadc1, 1);
	ADC_Samples=HAL_ADC_GetValue(&hadc1);
	ADC_Reading = ADC_Samples*(3.3/256);
	//	printf("DVTEMP Samples: %ld\r\n",ADC_Samples);
	//printf("volt2=%f\r\n",ADC_Reading);
	return ADC_Reading;
}
double ARSSI_Reading(void)
{
	int i;
	double voltage_samples[200];
	uint32_t ADC_Samples=0;
	double ADC_Reading=0;
	sConfig.Channel = ADC_CHANNEL_2;
	sConfig.Rank = ADC_REGULAR_RANK_1;
	sConfig.SamplingTime = ADC_SAMPLETIME_8CYCLES_5;
	sConfig.SingleDiff = ADC_SINGLE_ENDED;
	sConfig.OffsetNumber = ADC_OFFSET_NONE;
	sConfig.Offset = 0;
	sConfig.OffsetSignedSaturation = DISABLE;
	if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
	{
		Error_Handler();
	}
	for(i=0;i<200;i++)
	{
		HAL_ADC_Start(&hadc1);
		HAL_ADC_PollForConversion(&hadc1, 1);
		ADC_Samples=HAL_ADC_GetValue(&hadc1);
		voltage_samples[i] = ADC_Samples;
	}
	ADC_Samples = voltage_rms(voltage_samples,200);
	ADC_Reading = ADC_Samples*(3.3/256);
	ADC_Reading = ADC_Reading-0.050;
	//printf("ARSSI Samples: %ld\r\n",ADC_Samples);
	//printf("volt2=%f\r\n",ADC_Reading);
	return ADC_Reading;
}

uint32_t AMRSSI_Reading(void)
{
	int i;
	double voltage_samples[200];
	uint32_t ADC_Samples=0;
	double ADC_Reading=0;
	sConfig.Channel = ADC_CHANNEL_10;
	sConfig.Rank = ADC_REGULAR_RANK_1;
	sConfig.SamplingTime = ADC_SAMPLETIME_8CYCLES_5;
	sConfig.SingleDiff = ADC_SINGLE_ENDED;
	sConfig.OffsetNumber = ADC_OFFSET_NONE;
	sConfig.Offset = 0;
	sConfig.OffsetSignedSaturation = DISABLE;
	if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
	{
		Error_Handler();
	}
	for(i=0;i<200;i++)
	{
		HAL_ADC_Start(&hadc1);
		HAL_ADC_PollForConversion(&hadc1, 10);
		ADC_Samples=HAL_ADC_GetValue(&hadc1);
		voltage_samples[i] = ADC_Samples;
	}

	ADC_Samples = voltage_rms(voltage_samples,200);
	//ADC_Reading = ADC_Samples*(3.3/256);
	ADC_Reading = ADC_Reading;
//	memset(voltage_samples,0,sizeof(voltage_samples));
	return ADC_Samples;
}

void Ethernet_TCP_UDP_RUN(void)
{
	int i=0;
	loopback_tcps(SOCK_TCPS, gDATABUF, PORT_TCPS);
	loopback_udps(SOCK_UDPS, gDATABUF, PORT_UDPS);
	if(TCP_Recv_Flag==1)
	{
		if(TCP_Rec_Buff[0] == 0xc9 && TCP_Rec_Buff[9] == 0xcf)
		{
			ETH_TX_RX_Flag = TCP_Rec_Buff[1];
			ETH_CC_DD_Mode = TCP_Rec_Buff[2];
			ETH_CH_NO = TCP_Rec_Buff[3];
			ETH_Freq = (double)(TCP_Rec_Buff[4]<<24|TCP_Rec_Buff[5]<<16|TCP_Rec_Buff[6]<<8|TCP_Rec_Buff[7]);//
			ETH_Freq = ETH_Freq/100000;
			if(ETH_TX_RX_Flag == 0x00)
			{
				//RX Channel
				sprintf(channel,"%d",ETH_CH_NO);
				sprintf(frequency,"%f",ETH_Freq);
				if(ETH_CC_DD_Mode == 0xcc)
				{
					if(ETH_CH_NO >=1 && ETH_CH_NO <=100 && ETH_Freq>=118 && ETH_Freq<=136.97500)
					{
						EEPROM_RX_WRITE(ETH_CH_NO, (unsigned char *)frequency, (unsigned char *)channel);
					}
					send(0,(unsigned char *)"OK",2);
				}
				else if(ETH_CC_DD_Mode == 0xdd)
				{
					memcpy(Current_Freq,frequency,sizeof(frequency));
					memcpy(Current_CH,channel,sizeof(channel));
					Frequency_Int = atof(Current_Freq);
					Keypad_Frequency_RX(Frequency_Int);
					RX_CTRL_1;
					RX_MUTE_CTRL_1;
					Current_Freq_Write((unsigned char *)Current_Freq, (unsigned char *)Current_CH);
					send(0,(unsigned char *)"OK",2);
					PTT_RX_State =1;
					RX_Mode_Flag = 1;
					TX_Mode_Flag = 0;
					prev_PTT_state =0;
					Prev_Frequency_Int = 310;
					Prev_Channel_Int = 311;
					Prev_Rssi_Int = 312;
					if(Prev_Channel_Int == Channel_Int)
						Prev_Channel_Int =313;
					if(Prev_Frequency_Int == Frequency_Int)
						Prev_Frequency_Int = 314;
				}
			}
			else if(ETH_TX_RX_Flag == 0x01)
			{
				//TX Channel
				sprintf(channel,"%d",ETH_CH_NO);
				sprintf(frequency,"%f",ETH_Freq);
				if(ETH_CC_DD_Mode == 0xcc)
				{
					if(ETH_CH_NO >=1 && ETH_CH_NO <=100 && ETH_Freq>=118 && ETH_Freq<=136.97500)
					{
						EEPROM_TX_WRITE(ETH_CH_NO, (unsigned char *)frequency, (unsigned char *)channel);
					}
					send(0,(unsigned char *)"OK",2);
				}
				else if(ETH_CC_DD_Mode == 0xdd)
				{
					memcpy(Current_Freq,frequency,sizeof(frequency));
					memcpy(Current_CH,channel,sizeof(channel));
					Frequency_Int = atof(Current_Freq);
					//PTT_BT_Pressed_TX(Frequency_Int);
					Keypad_Frequency_RX(Frequency_Int);
					RX_CTRL_1;
					RX_MUTE_CTRL_1;
					Current_Freq_Write((unsigned char *)Current_Freq, (unsigned char *)Current_CH);
					send(0,(unsigned char *)"OK",2);
					PTT_RX_State = 1;
					RX_Mode_Flag = 0;
					TX_Mode_Flag = 1;
					prev_PTT_state = 0;
					Prev_Frequency_Int = 305;
					Prev_Channel_Int = 306;
					Prev_Rssi_Int = 307;
					if(Prev_Channel_Int == Channel_Int)
						Prev_Channel_Int =308;
					if(Prev_Frequency_Int == Frequency_Int)
						Prev_Frequency_Int = 309;
				}
			}
		}
		if(TCP_Rec_Buff[0] == 0x24 && TCP_Rec_Buff[3] == 0x23)
		{
			ETH_TX_RX_Flag = TCP_Rec_Buff[1];
			ETH_CC_DD_Mode = TCP_Rec_Buff[2];
			if(ETH_CC_DD_Mode == 0xDD)
			{
				//Current channel send for RX
				TCP_Send_Buf[0] = 0xC9;
				if(RX_Mode_Flag == 1)
				{
					TCP_Send_Buf[1] = 0x00;
				}
				else if(TX_Mode_Flag == 1)
				{
					TCP_Send_Buf[1] = 0x01;
				}
				TCP_Send_Buf[2] = 0xDD;
				Read_CH_RX = atoi(Current_CH);
				TCP_Send_Buf[3] = Read_CH_RX;
				Frequency_Int = atof(Current_Freq);
				ETH_Frequency_Int = Frequency_Int*100000;
				TCP_Send_Buf[4] =ETH_Frequency_Int >> 24;
				TCP_Send_Buf[5] =ETH_Frequency_Int >> 16;
				TCP_Send_Buf[6] =ETH_Frequency_Int >> 8;
				TCP_Send_Buf[7] =ETH_Frequency_Int;
				TCP_Send_Buf[8] = 0xCF;
				send(0, (unsigned char*)TCP_Send_Buf, 9);
				memset(TCP_Send_Buf,0,sizeof(TCP_Send_Buf));
			}
			else if(ETH_CC_DD_Mode == 0xCC)
			{
				//Send all channels data for RX
				if(ETH_TX_RX_Flag == 0x00)
				{
					for(i=1;i<=100;i++)
					{
						EEPROM_RX_READ(i,(unsigned char*)ETH_Current_Freq, (unsigned char*)ETH_Current_CH);
						TCP_Send_Buf[0] = 0xC9;
						TCP_Send_Buf[1] = 0x00;
						TCP_Send_Buf[2] = 0xCC;
						if(Current_CH[0]==0xff)
						{
							memset(Current_CH,0,sizeof(Current_CH));
							memset(ETH_Current_Freq,0,sizeof(ETH_Current_Freq));
						}
						Read_CH_RX = i;//atoi(ETH_Current_CH);
						TCP_Send_Buf[3] = Read_CH_RX;
						Frequency_Int = atof(ETH_Current_Freq);
						ETH_Frequency_Int = Frequency_Int*100000;
						TCP_Send_Buf[4] =ETH_Frequency_Int >> 24;
						TCP_Send_Buf[5] =ETH_Frequency_Int >> 16;
						TCP_Send_Buf[6] =ETH_Frequency_Int >> 8;
						TCP_Send_Buf[7] =ETH_Frequency_Int;
						TCP_Send_Buf[8] = 0xCF;
						send(0, (unsigned char*)TCP_Send_Buf, 9);
						memset(TCP_Send_Buf,0,sizeof(TCP_Send_Buf));
					}
				}
				else if(ETH_TX_RX_Flag == 0x01)
				{
					for(i=1;i<=100;i++)
					{
						EEPROM_TX_READ(i,(unsigned char*)ETH_Current_Freq, (unsigned char*)ETH_Current_CH);
						TCP_Send_Buf[0] = 0xC9;
						TCP_Send_Buf[1] = 0x01;
						TCP_Send_Buf[2] = 0xCC;
						if(Current_CH[0]==0xff)
						{
							memset(Current_CH,0,sizeof(Current_CH));
							memset(ETH_Current_Freq,0,sizeof(ETH_Current_Freq));
						}
						Read_CH_TX = i;//atoi(ETH_Current_CH);
						TCP_Send_Buf[3] = Read_CH_TX;
						Frequency_Int = atof(ETH_Current_Freq);
						ETH_Frequency_Int = Frequency_Int*100000;
						TCP_Send_Buf[4] =ETH_Frequency_Int >> 24;
						TCP_Send_Buf[5] =ETH_Frequency_Int >> 16;
						TCP_Send_Buf[6] =ETH_Frequency_Int >> 8;
						TCP_Send_Buf[7] =ETH_Frequency_Int;
						TCP_Send_Buf[8] = 0xCF;
						send(0, (unsigned char*)TCP_Send_Buf, 9);
						memset(TCP_Send_Buf,0,sizeof(TCP_Send_Buf));
					}
				}
			}
		}
		TCP_Recv_Flag=0;
//		printf("Tcp_Recv_Buff: %s\r\n",TCP_Rec_Buff);
		memset(TCP_Rec_Buff,0,13);
	}
}
void Battery_Display(const unsigned char *icon)
{
	unsigned int i,j;
	for(i=0;i<1;i++)
	{
		Set_Start_Page_12832(i);
		Set_Start_Column_12832(0x69);
		for(j=105;j<121;j++){     //128 columns wide
			oled_Data(*icon);    //send picture data
			icon++;
		}
	}
}
void Signal_Display(const unsigned char *icon)
{
	unsigned int i,j;
	for(i=0;i<1;i++)
	{
		Set_Start_Page_12832(i);
		Set_Start_Column_12832(0x71); //0x53
		for(j=113;j<128;j++){     //128 columns wide
			oled_Data(*icon);    //send picture data
			icon++;
		}
	}
}
void Down_Up_Arrow(const unsigned char *icon)
{
	unsigned int i,j;
	for(i=3;i<4;i++)
	{
		Set_Start_Page_12832(i);
		Set_Start_Column_12832(0x78);
		for(j=120;j<128;j++){     //128 columns wide
			oled_Data(*icon);    //send picture data
			icon++;
		}
	}
}
void Mute_Speaker(const unsigned char *icon)
{
	unsigned int i,j;
	for(i=0;i<1;i++)
	{
		Set_Start_Page_12832(i);
		Set_Start_Column_12832(0x5d);
		for(j=93;j<109;j++){     //128 columns wide
			oled_Data(*icon);    //send picture data
			icon++;
		}
	}
}
void SQ_Check_State(void)
{
	if(Squelth_Check_state == 1)
	{
		AMRSSI_Volt = AMRSSI_Reading();
		if(AMRSSI_Volt<0.300)
		{
			//SQUELCH ON condition
			RX_MUTE_CTRL_1;
		}
		else
		{
			//SQUELCH OFF condition
			RX_MUTE_CTRL_0;
		}
	}
}
void Menu_OLED_Application(void)
{
	if(Menu == 0 && PTT_Switch_state == 0 && Manual_FRQ_Entry == 0)
	{
		PTT_OLED_Erase =0;
		prev_PTT_state =0;
		VSWR_State_Control = 0;
		PTT_state =0;
		T_R_CTRL_0;
		TX_CTRL_0;
		TX_CTRL1_0
//		RX_CTRL_1;
		Frequency_Int = atof(Current_Freq);
		if(Frequency_Int!=Prev_Frequency_Int)
		{
			if(PTT_RX_State==1)
			{
				PTT_RX_State=0;
				if(Frequency_Int>=118 && Frequency_Int<=136.97500)
				{
					Keypad_Frequency_RX(Frequency_Int);
				}
				//printf("Calling RX Freq\r\n");
				RX_ATTN_CTRL_0;
				RX_ATTN_TH=0;
			}
			Fill_RAM_12832(0x00);
			RX_CTRL_1;
			RX_MUTE_CTRL_1;
			Prev_Frequency_Int=Frequency_Int;
			Show_String_12832(1,(unsigned char *)"Rx",0,65);
			Signal_Display(SGT4); //Need to check BRSSI signal and upadte
			Show_String_12832(1,(unsigned char *)"FREQ : ",1,4);
			if(Current_Freq[strlen(Current_Freq)]=='_')
			{
				Current_Freq[strlen(Current_Freq)]='0';
			}
			if(Current_Freq[0]=='_')
			{
				Current_Freq[0]='0';
			}
			else if(Current_Freq[0]==0)
			{
				Current_Freq[0]='0';
			}
			Show_String_12832(1,(unsigned char *)Current_Freq,1,40);
			Show_String_12832(1,(unsigned char *)"MHz",1,110);
			SQ_T_H_Val_int = atoi(SQ_T_H_Val);
			if(SQ_T_H_Val_int == 0)
				Show_String_12832(1,(unsigned char *)"Sq   : OFF ",3,4);
			else if(SQ_T_H_Val_int == 1)
				Show_String_12832(1,(unsigned char *)"Sq   : 1   ",3,4);
			else if(SQ_T_H_Val_int == 2)
				Show_String_12832(1,(unsigned char *)"Sq   : 2   ",3,4);
			else if(SQ_T_H_Val_int == 3)
				Show_String_12832(1,(unsigned char *)"Sq   : 3   ",3,4);
			else if(SQ_T_H_Val_int == 4)
				Show_String_12832(1,(unsigned char *)"Sq   : 4   ",3,4);
		}
		Channel_Int = atof(Current_CH);
		if(Channel_Int!=Prev_Channel_Int)
		{
			Prev_Channel_Int=Channel_Int;
			Show_String_12832(1,(unsigned char *)"CH NO: ",0,4);
			if(Current_CH[strlen(Current_CH)]=='_')
			{
				Current_CH[strlen(Current_CH)]='0';
			}
			if(Current_CH[strlen(Current_CH)-1]=='_')
			{
				Current_CH[strlen(Current_CH)-1]=0;
			}
			if(Current_CH[0]=='_')
			{
				Current_CH[0]='0';
			}
			else if(Current_CH[0]==0)
			{
				Current_CH[0]='0';
			}
			Show_String_12832(1,(unsigned char *)Current_CH,0,40);
		}
		Rssi_Int = atof(rxlevel);
		if(Rssi_Int!=Prev_Rssi_Int)
		{
			Prev_Rssi_Int=Rssi_Int;
			Show_String_12832(1,(unsigned char *)"Rssi : ",2,4);
			Show_String_12832(1,(unsigned char *)rxlevel,2,40);
			Show_String_12832(1,(unsigned char *)"dBm",2,66);
		}
		Mute_Speaker(Speaker_UnMute);
		TX_Mode_Flag = 0;
		RX_Mode_Flag = 1;
	}
	else if(Menu == 0 && PTT_Switch_state == 0 && Manual_FRQ_Entry == 1 && Enter ==0)
	{
		if(PTT_OLED_Erase_state==1)
		{
			PTT_OLED_Erase_state = 0;
			Fill_RAM_12832(0x00);
		}
		prev_PTT_state = 0;
		Show_String_12832(1,(unsigned char*)"ENTER FREQUENCY",0,20);
		Show_String_12832(1,(unsigned char*)"Freq : ",2,10);
		Show_String_12832(1,(unsigned char*)"          ",2,52);
		HAL_Delay(10);
		Show_String_12832(1,(unsigned char*)frequency,2,52);
		Show_String_12832(1,(unsigned char*)"MHz",2,110);
		Show_String_12832(1,SQ_T_H,3,10);
		Show_String_12832(1,(unsigned char *)SQ_T_H_Val,3,58);
		if(Frequency_Step_Inc == 1)
		{
			Frequency_Step_Inc = 0;
			Save_frequency_double = atof(frequency);
			if(Save_frequency_double>=118 && Save_frequency_double<=136.96667)
			{
				if(frequency[strlen(frequency)-1] == '_')
				{
					frequency[strlen(frequency)-1]=0;
				}
				if(frequency[0]=='_')
				{
					frequency[0]='0';
				}
				if(frequency[0]==0)
				{
					frequency[0]='0';
				}
				if(frequency[9]==' ')
				{
					frequency[9]=0;
				}
				if(frequency[3] != '.')
				{
					dummy_freq[0] = frequency[0];
					dummy_freq[1] = frequency[1];
					dummy_freq[2] = frequency[2];
					dummy_freq[3] = '.';
					dummy_freq[4] = frequency[3];
					dummy_freq[5] = frequency[4];
					dummy_freq[6] = frequency[5];
					dummy_freq[7] = frequency[6];
					dummy_freq[8] = frequency[7];
					dummy_freq[9] = frequency[8];
					dummy_freq[10] = frequency[9];
					dummy_freq[11] = frequency[10];
					dummy_freq[12] = frequency[11];
					memset(frequency,0,sizeof(frequency));
					memcpy(frequency,dummy_freq,sizeof(dummy_freq));
				}
				Save_frequency_double = atof(frequency);
				Save_frequency_double = Save_frequency_double+0.00833;
				OLED_Frequency = Save_frequency_double;
				OLED_Frequency = OLED_Frequency*100000;
				OLED_Frequency = OLED_Frequency/100000;
				sprintf(frequency,"%0.5f",OLED_Frequency);
			}
		}
		else if(Frequency_Step_Dnc)
		{
			Frequency_Step_Dnc = 0;
			Save_frequency_double = atof(frequency);
			if(Save_frequency_double>=118.00833 && Save_frequency_double<=136.97500)
			{
				if(frequency[strlen(frequency)-1] == '_')
				{
					frequency[strlen(frequency)-1]=0;
				}
				if(frequency[0]=='_')
				{
					frequency[0]='0';
				}
				if(frequency[0]==0)
				{
					frequency[0]='0';
				}
				if(frequency[9]==' ')
				{
					frequency[9]=0;
				}
				if(frequency[3] != '.')
				{
					dummy_freq[0] = frequency[0];
					dummy_freq[1] = frequency[1];
					dummy_freq[2] = frequency[2];
					dummy_freq[3] = '.';
					dummy_freq[4] = frequency[3];
					dummy_freq[5] = frequency[4];
					dummy_freq[6] = frequency[5];
					dummy_freq[7] = frequency[6];
					dummy_freq[8] = frequency[7];
					dummy_freq[9] = frequency[8];
					dummy_freq[10] = frequency[9];
					dummy_freq[11] = frequency[10];
					dummy_freq[12] = frequency[11];
					memset(frequency,0,sizeof(frequency));
					memcpy(frequency,dummy_freq,sizeof(dummy_freq));
				}
				Save_frequency_double = atof(frequency);
				Save_frequency_double = Save_frequency_double-0.00833;
				OLED_Frequency = Save_frequency_double;
				OLED_Frequency = OLED_Frequency*100000;
				OLED_Frequency = OLED_Frequency/100000;
				sprintf(frequency,"%0.5f",OLED_Frequency);
			}
		}
	}
	else if(Menu == 0 && PTT_Switch_state == 0 && Manual_FRQ_Entry == 2 && Enter ==0)
	{
		prev_PTT_state = 0;
		Show_String_12832(1,SQ_T_H,3,10);
		Show_String_12832(1,(unsigned char *)"   ",3,58);
		HAL_Delay(15);
		Show_String_12832(1,(unsigned char *)SQ_T_H_Val,3,58);
	}
	else if(Menu == 0 && PTT_Switch_state == 0 && Manual_FRQ_Entry == 1 && Enter ==1)
	{
		if(PTT_OLED_Erase_state==1)
		{
			PTT_OLED_Erase_state = 0;
			Fill_RAM_12832(0x00);
		}
		memset(channel,0,sizeof(channel));
		channel[0]='0';
		Enter = 0;
		Manual_FRQ_Entry =1;
		prev_PTT_state = 0;
		if(Prev_Channel_Int == Channel_Int)
			Prev_Channel_Int =206;
		if(Prev_Frequency_Int == Frequency_Int)
			Prev_Frequency_Int = 207;
		if(frequency[strlen(frequency)-1] == '_')
		{
			frequency[strlen(frequency)-1]=0;
		}
		if(frequency[0]=='_')
		{
			frequency[0]='0';
		}
		if(frequency[0]==0)
		{
			frequency[0]='0';
		}
		if(frequency[9]==' ')
		{
			frequency[9]=0;
		}
		Manual_Freq_Applying();
	}
	else if(Menu == 0 && PTT_Switch_state == 0 && Manual_FRQ_Entry == 2 && Enter ==1)
	{
		if(PTT_OLED_Erase_state==1)
		{
			PTT_OLED_Erase_state = 0;
			Fill_RAM_12832(0x00);
		}
		memset(channel,0,sizeof(channel));
		channel[0]='0';
		Enter = 0;
		Manual_FRQ_Entry =1;
		prev_PTT_state = 0;
		if(Prev_Channel_Int == Channel_Int)
			Prev_Channel_Int =206;
		if(Prev_Frequency_Int == Frequency_Int)
			Prev_Frequency_Int = 207;
		if(frequency[strlen(frequency)-1] == '_')
		{
			frequency[strlen(frequency)-1]=0;
		}
		if(frequency[0]=='_')
		{
			frequency[0]='0';
		}
		if(frequency[0]==0)
		{
			frequency[0]='0';
		}
		if(frequency[9]==' ')
		{
			frequency[9]=0;
		}
		Manual_Freq_Applying();
	}
	else if(Menu==0 && PTT_Switch_state == 1)
	{
		if(VSWR_State==1 && AVTEMP_State==1)
		{
			if(PTT_OLED_Erase==0)
			{
				PTT_OLED_Erase=1;
				Fill_RAM_12832(0x00);
				DAC3_Voltage = 1.93;
				DAC3_Generating(DAC3_Voltage);
				Frequency_Int = atof(Current_Freq);
				if(Frequency_Int!=0)
				{
					printf("Calling TX Freq\r\n");
					PTT_BT_Pressed_TX(Frequency_Int);
				}
				Show_String_12832(1,(unsigned char *)"CH NO: ",0,4);
				if(Current_CH[strlen(Current_CH)]=='_')
				{
					Current_CH[strlen(Current_CH)]='0';
				}
				Show_String_12832(1,(unsigned char *)Current_CH,0,40);
				Show_String_12832(1,(unsigned char *)"Tx",0,65);
				Show_String_12832(1,(unsigned char *)"P: 9W",3,98);
				Show_String_12832(1,(unsigned char *)"FREQ : ",1,4);
				Show_String_12832(1,(unsigned char *)Current_Freq,1,40);
				Show_String_12832(1,(unsigned char *)"MHz",1,110);
			}
			if(VSWR_State==1)
				Show_String_12832(1,(unsigned char *)"VSWR :  OK ",2,4);
			else
				Show_String_12832(1,(unsigned char *)"VSWR : NOK",2,4);//Changed NOT OK to OK
			//if(AVTEMP_State==1)
			if(VSWR_State==1)
				Show_String_12832(1,(unsigned char *)"THRM :  OK ",3,4);
			else
				Show_String_12832(1,(unsigned char *)"THRM : NOK",3,4);//Changed NOT OK to OK
			Mute_Speaker(Speaker_Mute);
			TX_Mode_Flag = 1;
			RX_Mode_Flag = 0;
			TX_RX_Controls(); //TX and RX Controls for Enabling TX frequency
		}
		else
		{
			if(PTT_OLED_Erase==0)
			{
				PTT_OLED_Erase=1;
				Fill_RAM_12832(0x00);
				DAC3_Voltage = 1.93;
				DAC3_Generating(DAC3_Voltage);
				Frequency_Int = atof(Current_Freq);
				if(Frequency_Int!=0)
				{
					printf("Calling TX Freq\r\n");
					PTT_BT_Pressed_TX(Frequency_Int);
				}
				Show_String_12832(1,(unsigned char *)"CH NO: ",0,4);
				Show_String_12832(1,(unsigned char *)Current_CH,0,40);
				Show_String_12832(1,(unsigned char *)"Tx",0,65);
				Show_String_12832(1,(unsigned char *)"FREQ : ",1,4);
				Show_String_12832(1,(unsigned char *)Current_Freq,1,40);
				Show_String_12832(1,(unsigned char *)"MHz",1,110);
				Show_String_12832(1,(unsigned char *)"P: 9W",3,98);
				Mute_Speaker(Speaker_Mute);
				TX_Mode_Flag = 1;
				RX_Mode_Flag = 0;
				//HAL_Delay(100);
			}
			TX_RX_Controls(); //TX and RX Controls for Enabling TX frequency
			if(VSWR_State==1)
				Show_String_12832(1,(unsigned char *)"VSWR :  OK",2,4);
			else
				Show_String_12832(1,(unsigned char *)"VSWR : NOK",2,4);//Changed NOT OK to OK
			//if(AVTEMP_State==1)
			if(VSWR_State==1)
				Show_String_12832(1,(unsigned char *)"THRM :  OK",3,4);
			else
				Show_String_12832(1,(unsigned char *)"THRM : NOK",3,4);//Changed NOT OK to OK
		}
	}
	else if(Menu==1 && Inc==0 && Enter==0)
	{
		Inc_State=1;
		if(Page_Erase0==0)
		{
			Page_Erase0=1,Page_Erase1=0,Page_Erase2=0,Page_Erase3=0,Page_Erase4=0;
			Inc=1;
		}
	}
	else if(Menu==1 && Inc==1 && Enter==0)
	{
		Inc_State=1;
		if(Page_Erase1==0)
		{
			Page_Erase1=1,Page_Erase0=0,Page_Erase2=0,Page_Erase3=0,Page_Erase4=0;
			//Fill_RAM_12832(0x00);
			Show_String_12832(1,(unsigned char*)"MENU",0,52);
			Show_String_12832(1,Menu_Disp0_1,1,4);
			Show_String_12832(1,Menu_Disp1,3,4);
			HAL_Delay(20);
		}
		Show_String_12832(1,Clear_Arrow0_1,0,4);
		Show_String_12832(1,Clear_Arrow0_2,1,4);
		Show_String_12832(1,Clear_Arrow0_3,2,4);
		Show_String_12832(1,Clear_Arrow0_4,3,4);
		Down_Up_Arrow(Arrow_Down);
		HAL_Delay(10);
		Show_String_12832(1,Clear_Arrow1_1,1,4);
	}
	else if(Menu==1 && Inc==2 && Enter==0)
	{
		Inc_State=1;
		if(Page_Erase2==0)
		{
			Page_Erase2=1,Page_Erase0=0,Page_Erase1=0,Page_Erase3=0,Page_Erase4=0;
			Show_String_12832(1,Menu_Disp0,1,4);
			Show_String_12832(1,Menu_Disp1_1,3,4);
			HAL_Delay(20);
		}
		Show_String_12832(1,Clear_Arrow0_1,0,4);
		Show_String_12832(1,Clear_Arrow0_2,1,4);
		Show_String_12832(1,Clear_Arrow0_3,2,4);
		Show_String_12832(1,Clear_Arrow0_4,3,4);
		Down_Up_Arrow(Arrow_Down);
		HAL_Delay(10);
		Show_String_12832(1,Clear_Arrow1_1,3,4);
	}
	else if(Menu==1 && Inc==3 && Enter==0)
	{
		Inc_State=1;
		if(Page_Erase3==0)
		{
			//Fill_RAM_12832(0x00);
			Page_Erase3=1,Page_Erase0=0,Page_Erase1=0,Page_Erase2=0,Page_Erase4=0;
			Show_String_12832(1,Menu_Disp2_1,1,4);
			Show_String_12832(1,Menu_Disp3,3,4);
			HAL_Delay(20);
		}
		Show_String_12832(1,Clear_Arrow0_1,0,4);
		Show_String_12832(1,Clear_Arrow0_2,1,4);
		Show_String_12832(1,Clear_Arrow0_3,2,4);
		Show_String_12832(1,Clear_Arrow0_4,3,4);
		Down_Up_Arrow(Arrow_Up);
		HAL_Delay(10);
		Show_String_12832(1,Clear_Arrow1_1,1,4);
	}
	else if(Menu==1 && Inc==4 && Enter==0)
	{
		Inc_State=1;
		if(Page_Erase4==0)
		{
			//Fill_RAM_12832(0x00);
			Page_Erase4=1,Page_Erase0=0,Page_Erase1=0,Page_Erase2=0,Page_Erase3=0;
			Show_String_12832(1,Menu_Disp2,1,4);
			Show_String_12832(1,Menu_Disp3_1,3,4);
			HAL_Delay(20);
		}
		Show_String_12832(1,Clear_Arrow0_1,0,4);
		Show_String_12832(1,Clear_Arrow0_2,1,4);
		Show_String_12832(1,Clear_Arrow0_3,2,4);
		Show_String_12832(1,Clear_Arrow0_4,3,4);
		Down_Up_Arrow(Arrow_Up);
		HAL_Delay(10);
		Show_String_12832(1,Clear_Arrow1_1,3,4);
	}
	else if(Menu==1 && Page_Erase1 == 1 && Enter == 1) //Password Login page
	{
		Password_Login_Page();
	}
	else if(Menu==1 && Page_Erase2 == 1 && Enter == 1)
	{
		//Password_Login_Page();
		Enter =3;
	}
	else if(Menu==1 && Page_Erase3 == 1 && Enter == 1)
	{
		Password_Login_Page();
	}
	else if(Menu==1 && Page_Erase4 == 1 && Enter == 1)
	{
		//Password_Login_Page();
		Enter =3;
	}
	else if(Menu==1 && Page_Erase1 == 1 && Enter == 2 && Password_Reset_Flag==1)
	{
		Reset_Passwords();
	}
	else if(Menu==1 && Page_Erase2 == 1 && Enter == 2 && Password_Reset_Flag==1)
	{
		Reset_Passwords();
	}
	else if(Menu==1 && Page_Erase3 == 1 && Enter == 2 && Password_Reset_Flag==1)
	{
		Reset_Passwords();
	}
	else if(Menu==1 && Page_Erase3 == 1 && Enter == 2 && Password_Reset_Flag==1)
	{
		Reset_Passwords();
	}
	else if(Menu==1 && Page_Erase1 == 1 && Enter == 3 && Login_Entry_Flag == 0 && Admin_Entry_flag == 0) //SET TX Frequency and channel number entry page
	{
		Inc_State=0;
		if(Get_Tx_CH_Flag==0)
		{
			Show_String_12832(1,(unsigned char *)Enter_CH_No,2,10);
			Inc = 1;
			if(Inc==0 || Inc == 1)
			{
				Show_String_12832(1,(unsigned char *)"   ",2,88);
			}
			Show_String_12832(1,(unsigned char *)channel,2,88);
		}
		else
		{
			Show_String_12832(1,TX_PARAMETERS,0,4);
			Show_String_12832(1,FREQ_str,2,4);
			if(Inc==0)
			{
				Show_String_12832(1,(unsigned char *)"          ",2,50);
				if(frequency[strlen(frequency)-1]==' ')
				{
					frequency[strlen(frequency)-1] = 0;
				}
				if(channel[strlen(channel)-1]=='_')
				{
					channel[strlen(channel)-1] = ' ';
				}
				if(channel[0]=='_')
				{
					channel[0]='0';
				}
				HAL_Delay(20);
				Show_String_12832(1,(unsigned char *)"  ",3,88);
				Back_Page =0;
			}
			if(frequency[0]==0)
				frequency[0] = '0';
			Show_String_12832(1,(unsigned char *)frequency,2,50);
			Show_String_12832(1,(unsigned char *)"MHz",2,110);
			Show_String_12832(1,CH_NO_Str,3,4);
			if(Inc==1)
			{
				Show_String_12832(1,(unsigned char *)"      ",3,56);
				HAL_Delay(20);
				if(frequency[strlen(frequency)-1]=='_')
				{
					frequency[strlen(frequency)-1] = ' ';
				}
				if(channel[strlen(channel)-1]==' ')
				{
					channel[strlen(channel)-1] = 0;
				}
				if(frequency[strlen(frequency)]==' ')
				{
					frequency[strlen(frequency)] = 0;
				}
				if(channel[strlen(channel)]==' ')
				{
					channel[strlen(channel)] = 0;
				}
				if(frequency[0]=='_')
				{
					frequency[0]='0';
				}
				Show_String_12832(1,(unsigned char *)"   ",3,88);
				Back_Page =0;
			}
			if(channel[0]==0)
				channel[0] = '0';
			Show_String_12832(1,(unsigned char *)channel,3,56);
			if(Inc == 2)
			{
				Back_Page =1;
				Reset_State = 0;
				YES_State =0;
				NO_State = 0;
				Show_String_12832(1,(unsigned char *)Clear_Arrow1_1,3,88);
			}
			Show_String_12832(1,Back_Str,3,100);
			Saved_flag0=0;
		}
	}
	else if(Menu==1 && Page_Erase1 == 1 && Enter == 4 && Login_Entry_Flag == 0 && Admin_Entry_flag == 0 && Get_Tx_CH_Flag == 0)
	{
		Read_CH_TX = atoi(channel);
		if(Read_CH_TX>=1 && Read_CH_TX<=100)
		{
			if(Read_CH_TX!=0)
			{
				EEPROM_TX_READ(Read_CH_TX,(unsigned char*)EEPROM_Freq, (unsigned char*)EEPROM_CH);
				printf("Read Freq:%s \t,CH: %s\r\n",EEPROM_Freq,EEPROM_CH);
			}
			if(EEPROM_Freq[0]==0xff)
			{
				Show_String_12832(1,Not_Available_in_memory,1,20);
				Show_String_12832(1,CH_NO_Str,3,20); //Need to check channel and assign current frequency
				if(channel[strlen(channel)-1]=='_')
				{
					channel[strlen(channel)-1] = 0;
				}
				else if(channel[strlen(channel)]=='_')
				{
					channel[strlen(channel)-1] = 0;
				}
				else if(channel[0]==0)
				{
					channel[0] = '0';
				}
				Show_String_12832(1,(unsigned char *)channel,3,74);
				HAL_Delay(1000);
				memset(EEPROM_CH,0,sizeof(EEPROM_CH));
				memset(EEPROM_Freq,0,sizeof(EEPROM_Freq));
				memset(frequency,0,sizeof(frequency));
				if(EEPROM_CH[0]==0)
				{
					EEPROM_CH[strlen(EEPROM_CH)]='0';
				}
				if(EEPROM_Freq[0]==0)
				{
					EEPROM_Freq[strlen(EEPROM_Freq)]='0';
				}
				if(frequency[0]==0)
				{
					frequency[strlen(frequency)]='0';
				}
				Fill_RAM_12832(0x00);
				Enter = 3;
				Get_Tx_CH_Flag = 1;
			}
			else
			{
				Show_String_12832(1,Featching_in_memory,1,4);
				Show_String_12832(1,CH_NO_Str,3,20); //Need to check channel and assign current frequency
				if(EEPROM_CH[0]==0)
				{
					EEPROM_CH[0] = '0';
				}
				Show_String_12832(1,(unsigned char *)EEPROM_CH,3,74);
				HAL_Delay(1000);
				Fill_RAM_12832(0x00);
				memcpy(frequency,EEPROM_Freq,sizeof(EEPROM_Freq));
				memcpy(channel,EEPROM_CH,sizeof(EEPROM_CH));
				if(frequency[0]==0)
				{
					frequency[0] = '0';
				}
				if(channel[0]==0)
				{
					channel[0] = '0';
				}
			}
			Enter = 3;
			Get_Tx_CH_Flag = 1;
		}
		else
		{
			Show_String_12832(1,Not_in_Range,1,20);
			Show_String_12832(1,CH_NO_Str,3,20); //Need to check channel and assign current frequency
			Show_String_12832(1,(unsigned char *)channel,3,74);
			HAL_Delay(500);
			memset(frequency,0,sizeof(frequency));
			if(frequency[0]==0)
			{
				frequency[strlen(frequency)]='0';
			}
			Fill_RAM_12832(0x00);
			Enter = 3;
			Get_Tx_CH_Flag = 0;
		}
	}
	else if(Menu==1 && Page_Erase1 == 1 && Enter == 4 && Back_Page == 1)
	{
		Save_Changes_Apply();
	}
	else if(Menu==1 && Page_Erase1 == 1 && Enter == 5 && Back_Page == 1 && YES_State == 1)
	{
		if(frequency[strlen(frequency)]==' ')
		{
			frequency[strlen(frequency)] = 0;
		}
		if(channel[strlen(channel)]==' ')
		{
			channel[strlen(channel)] = 0;
		}
		Save_CH_TX = atoi(channel);
		Save_frequency_double = atof(frequency);
		if(Save_CH_TX >=1 && Save_CH_TX <=100 && Save_frequency_double>=118 && Save_frequency_double<=136.97500)
		{
			EEPROM_TX_WRITE(Save_CH_TX, (unsigned char *)frequency, (unsigned char *)channel);
		}
		Enter = 4;
		Inc =0;
		Back_Page = 0;
		YES_State = 0;
		NO_State = 0;
	}
	else if(Menu==1 && Page_Erase1 == 1 && Enter == 5 && Back_Page == 1 && NO_State == 1)
	{
		Menu = 0;
		Enter = 0;
		Page_Erase1 =0;
		Inc  =0;
		Back_Page = 0;
		YES_State = 0;
		NO_State = 0;
		Index1 = 0,Index2=0;
		Prev_Channel_Int =Prev_Channel_Int +1;
		Prev_Rssi_Int = Prev_Rssi_Int+1;
		Prev_Frequency_Int=Prev_Frequency_Int+1;
		memset(frequency,0,sizeof(frequency));
		frequency[strlen(frequency)]='0';
		memset(channel,0,sizeof(channel));
		channel[strlen(channel)]='0';
	}
	else if(Menu==1 && Page_Erase2 == 1 && Enter == 3) //Frequency and channel number entry page
	{
		Inc_State =0;
		Get_Tx_CH_Flag =1;
		Show_String_12832(1,TX_PARAMETERS,0,4);
		Show_String_12832(1,FREQ_str,1,4);
		if(Inc==0)
		{
			Show_String_12832(1,(unsigned char *)"          ",1,50);
			Show_String_12832(1,(unsigned char *)"  ",3,88);
			if(frequency[strlen(frequency)-1]==' ')
			{
				frequency[strlen(frequency)-1] = 0;
			}
			memset(channel,0,sizeof(channel));
			if(CH_Erase_RX == 0)
			{
				CH_Erase_RX = 1;
				Show_String_12832(1,(unsigned char *)"      ",3,56);
			}
			channel[0] = '0';
			HAL_Delay(20);
		}
		Show_String_12832(1,(unsigned char *)frequency,1,50);
		Show_String_12832(1,(unsigned char *)"MHz",1,110);
		Show_String_12832(1,TX_PWR_str,2,4);
		Show_String_12832(1,(unsigned char *)TX_PWR_WT,2,56);
		Show_String_12832(1,(unsigned char *)"W",2,62);
		Show_String_12832(1,CH_NO_Str,3,4); //Need to check channel and assign current frequency
		if(Inc==1)
		{
			CH_Erase_RX = 0;
			Back_Page =0;
			Show_String_12832(1,(unsigned char *)"      ",3,56);
			Show_String_12832(1,(unsigned char *)"  ",3,88);
			HAL_Delay(20);
		}
		Show_String_12832(1,(unsigned char *)channel,3,56);
		if(Inc == 2)
		{
			CH_Erase_RX = 0;
			Back_Page =1;
			Reset_State = 0;
			YES_State =0;
			NO_State = 0;
			Show_String_12832(1,(unsigned char *)Clear_Arrow1_1,3,88);
		}
		Show_String_12832(1,Back_Str,3,100);
		Saved_flag1=0;
	}
	else if(Menu==1 && Page_Erase2 == 1 && Enter == 4 && Back_Page == 1)
	{
		Save_Changes_Apply();
	}
	else if(Menu==1 && Page_Erase2 == 1 && Enter == 5 && Back_Page == 1 && YES_State == 1)
	{
		Save_frequency_double = atof(frequency);
		if(Save_CH_TX >=1 && Save_CH_TX <=100 && Save_frequency_double>=118 && Save_frequency_double<=136.97500)
		{
			EEPROM_TX_WRITE(Save_CH_TX, (unsigned char *)frequency, (unsigned char *)channel);
		}
		Enter = 4;
		Inc =0;
		Back_Page = 0;
		YES_State = 0;
		NO_State = 0;
	}
	else if(Menu==1 && Page_Erase2 == 1 && Enter == 5 && Back_Page == 1 && NO_State == 1)
	{
		Menu = 0;
		Enter = 0;
		Page_Erase1 =0;
		Inc  =0;
		Back_Page = 0;
		YES_State = 0;
		NO_State = 0;
		Index1 = 0,Index2=0;
		Prev_Channel_Int =Prev_Channel_Int +1;
		Prev_Rssi_Int = Prev_Rssi_Int+1;
		Prev_Frequency_Int=Prev_Frequency_Int+1;
		memset(frequency,0,sizeof(frequency));
		frequency[strlen(frequency)]='0';
		memset(channel,0,sizeof(channel));
		channel[strlen(channel)]='0';
	}
	else if(Menu==1 && Page_Erase3 == 1 && Enter == 3 && Login_Entry_Flag == 0 && Admin_Entry_flag == 0) //Frequency and channel number entry page
	{
		Inc_State =0;
		if(Get_Rx_CH_Flag==0)
		{
			Show_String_12832(1,(unsigned char *)Enter_CH_No,2,10);
			//Inc = 1;
			if(Inc==0 || Inc == 1)
			{
				Show_String_12832(1,(unsigned char *)"    ",2,88);
			}
			Show_String_12832(1,(unsigned char *)channel,2,88);
		}
		else
		{
			Show_String_12832(1,RX_PARAMETERS,0,4);
			Show_String_12832(1,FREQ_str,1,4);
			if(Inc==0)
			{
				Back_Page =0;
				Show_String_12832(1,(unsigned char *)"          ",1,50);
				Show_String_12832(1,(unsigned char *)"  ",3,88);
				HAL_Delay(20);
				if(frequency[strlen(frequency)-1]==' ')
				{
					frequency[strlen(frequency)-1] = 0;
				}
				if(channel[strlen(channel)-1]=='_')
				{
					channel[strlen(channel)-1] = ' ';
				}
				if(channel[0]=='_')
				{
					channel[0]='0';
				}
			}
			if(frequency[0]==0)
				frequency[0] = '0';
			Show_String_12832(1,(unsigned char *)frequency,1,50);
			Show_String_12832(1,(unsigned char *)"MHz",1,110);
//			if(Inc == 1)
//			{
//				Back_Page =0;
//				Show_String_12832(1,(unsigned char *)"            ",2,4);
//				Show_String_12832(1,(unsigned char *)"  ",3,88);
//				HAL_Delay(10);
//			}
//			if(Squelth_Read_State == 0)
//			{
//				Show_String_12832(1,SQ_SET_OFF,2,4);
//				Squelth_Check_state = 0;
//			}
//			else if(Squelth_Read_State == 1)
//			{
//				Show_String_12832(1,SQ_SET_ON,2,4);
//				Squelth_Check_state = 1;
//			}
			Show_String_12832(1,CH_NO_Str,3,4); //Need to check channel and assign current frequency
			if(Inc==2)
			{
				Back_Page =0;
				Show_String_12832(1,(unsigned char *)"      ",3,56);
				Show_String_12832(1,(unsigned char *)"  ",3,88);
				if(frequency[strlen(frequency)-1]=='_')
				{
					frequency[strlen(frequency)-1] = ' ';
				}
				if(channel[strlen(channel)-1]==' ')
				{
					channel[strlen(channel)-1] = 0;
				}
				if(frequency[0]=='_')
				{
					frequency[0]='0';
				}
				HAL_Delay(20);
			}
			if(channel[0]==0)
				channel[0] = '0';
			Show_String_12832(1,(unsigned char *)channel,3,56);
			Show_String_12832(1,SQ_T_H,2,4);
			if(Inc == 1)
			{
				Back_Page =0;
				Show_String_12832(1,(unsigned char *)"   ",2,52);
				HAL_Delay(15);
			}
			Show_String_12832(1,(unsigned char *)SQ_T_H_Val,2,52);
			if(Inc == 3)
			{
				Back_Page =1;
				Reset_State = 0;
				YES_State =0;
				NO_State = 0;
				Show_String_12832(1,(unsigned char *)Clear_Arrow1_1,3,88);
			}
			Show_String_12832(1,Back_Str,3,100);
			Saved_flag2=0;
		}

	}
	else if(Menu==1 && Page_Erase3 == 1 && Enter == 4 && Login_Entry_Flag == 0 && Admin_Entry_flag == 0 && Get_Rx_CH_Flag == 0)
	{
		Read_CH_RX = atoi(channel);
		if(Read_CH_RX>=1 && Read_CH_RX<=100)
		{
			if(Read_CH_RX!=0)
			{
				EEPROM_RX_READ(Read_CH_RX,(unsigned char*)EEPROM_Freq, (unsigned char*)EEPROM_CH);
				printf("Read Freq:%s \t,CH: %s\r\n",EEPROM_Freq,EEPROM_CH);
			}
			if(EEPROM_Freq[0]==0xff)
			{
				Show_String_12832(1,Not_Available_in_memory,1,20);
				Show_String_12832(1,CH_NO_Str,3,20); //Need to check channel and assign current frequency
				if(channel[strlen(channel)-1]=='_')
				{
					channel[strlen(channel)-1] = 0;
				}
				else if(channel[strlen(channel)]=='_')
				{
					channel[strlen(channel)-1] = 0;
				}
				Show_String_12832(1,(unsigned char *)channel,3,74);
				HAL_Delay(1000);
				memset(EEPROM_CH,0,sizeof(EEPROM_CH));
				memset(EEPROM_Freq,0,sizeof(EEPROM_Freq));
				memset(frequency,0,sizeof(frequency));
				if(EEPROM_CH[0]==0)
				{
					EEPROM_CH[strlen(EEPROM_CH)]='0';
				}
				if(EEPROM_Freq[0]==0)
				{
					EEPROM_Freq[strlen(EEPROM_Freq)]='0';
				}
				if(frequency[0]==0)
				{
					frequency[strlen(frequency)]='0';
				}
				Fill_RAM_12832(0x00);
				Enter = 3;
				Get_Rx_CH_Flag = 1;
			}
			else
			{
				Show_String_12832(1,Featching_in_memory,1,4);
				Show_String_12832(1,CH_NO_Str,3,20); //Need to check channel and assign current frequency
				if(Current_CH[0]==0)
				{
					Current_CH[0] = '0';
				}
				Show_String_12832(1,(unsigned char *)EEPROM_CH,3,74);
				HAL_Delay(1000);
				Fill_RAM_12832(0x00);
				memcpy(frequency,EEPROM_Freq,sizeof(EEPROM_Freq));
				memcpy(channel,EEPROM_CH,sizeof(EEPROM_CH));
				if(frequency[0]==0)
				{
					frequency[0] = '0';
				}
				if(channel[0]==0)
				{
					channel[0] = '0';
				}
			}
			Enter = 3;
			Get_Rx_CH_Flag = 1;
		}
		else
		{
			Show_String_12832(1,Not_in_Range,1,20);
			Show_String_12832(1,CH_NO_Str,3,20); //Need to check channel and assign current frequency
			Show_String_12832(1,(unsigned char *)channel,3,74);
			HAL_Delay(500);
			memset(frequency,0,sizeof(frequency));
			if(frequency[0]==0)
			{
				frequency[strlen(frequency)]='0';
			}
			Fill_RAM_12832(0x00);
			Enter = 3;
			Get_Rx_CH_Flag = 0;
		}
	}
	else if(Menu==1 && Page_Erase3 == 1 && Enter == 4 && Back_Page == 1)
	{
		Save_Changes_Apply();
	}
	else if(Menu==1 && Page_Erase3 == 1 && Enter == 5 && Back_Page == 1 && YES_State == 1)
	{
		Enter = 4;
		Inc =0;
		Back_Page = 0;
		YES_State = 0;
		NO_State = 0;
	}
	else if(Menu==1 && Page_Erase3 == 1 && Enter == 5 && Back_Page == 1 && NO_State == 1)
	{
		Menu = 0;
		Enter = 0;
		Page_Erase1 =0;
		Inc  =0;
		Back_Page = 0;
		YES_State = 0;
		NO_State = 0;
		Index1 = 0,Index2=0;
		Prev_Channel_Int =Prev_Channel_Int +1;
		Prev_Rssi_Int = Prev_Rssi_Int+1;
		Prev_Frequency_Int=Prev_Frequency_Int+1;
		memset(frequency,0,sizeof(frequency));
		frequency[strlen(frequency)]='0';
		memset(channel,0,sizeof(channel));
		channel[strlen(channel)]='0';
	}
	else if(Menu==1 && Page_Erase4 == 1 && Enter == 3) //Frequency and channel number entry page
	{
		Inc_State =0;
		Show_String_12832(1,RX_PARAMETERS,0,4);
		Show_String_12832(1,FREQ_str,1,4);
		Get_Rx_CH_Flag = 1;
		if(Inc==0)
		{
			Show_String_12832(1,(unsigned char *)"          ",1,50);
			Show_String_12832(1,(unsigned char *)"  ",3,88);
			if(frequency[strlen(frequency)-1]==' ')
			{
				frequency[strlen(frequency)-1] = 0;
			}
			//Show_String_12832(1,(unsigned char *)"      ",3,56);
			memset(channel,0,sizeof(channel));
			if(CH_Erase_RX == 0)
			{
				CH_Erase_RX = 1;
				Show_String_12832(1,(unsigned char *)"      ",3,56);
			}
			channel[0] = '0';
			HAL_Delay(20);
		}
		Show_String_12832(1,(unsigned char *)frequency,1,50);
		Show_String_12832(1,(unsigned char *)"MHz",1,110);
//		if(Inc == 1)
//		{
//			Show_String_12832(1,(unsigned char *)"            ",2,4);
//			Show_String_12832(1,(unsigned char *)"  ",3,88);
//			HAL_Delay(10);
//		}
//		if(Squelth_Read_State == 0)
//		{
//			Show_String_12832(1,SQ_SET_OFF,2,4);
//			Squelth_Check_state = 0;
//		}
//		else if(Squelth_Read_State == 1)
//		{
//			Show_String_12832(1,SQ_SET_ON,2,4);
//			Squelth_Check_state = 1;
//		}
		Show_String_12832(1,CH_NO_Str,3,4); //Need to check channel and assign current frequency
		if(Inc==2)
		{
			CH_Erase_RX = 0;
			Show_String_12832(1,(unsigned char *)"      ",3,56);
			Show_String_12832(1,(unsigned char *)"  ",3,88);
			Back_Page =0;
			HAL_Delay(10);
		}
		Show_String_12832(1,(unsigned char *)channel,3,56);
		Show_String_12832(1,SQ_T_H,2,4);
		if(Inc == 1)
		{
			CH_Erase_RX = 0;
			Back_Page =0;
			Show_String_12832(1,(unsigned char *)"   ",2,52);
			HAL_Delay(15);
		}
		Show_String_12832(1,(unsigned char *)SQ_T_H_Val,2,52);
		if(Inc == 3)
		{
			Back_Page =1;
			Reset_State = 0;
			YES_State =0;
			NO_State = 0;
			Show_String_12832(1,(unsigned char *)Clear_Arrow1_1,3,88);
		}
		Show_String_12832(1,Back_Str,3,100);
		Saved_flag3=0;
	}
	else if(Menu==1 && Page_Erase4 == 1 && Enter == 4 && Back_Page == 1)
	{
		Save_Changes_Apply();
	}
	else if(Menu==1 && Page_Erase4 == 1 && Enter == 5 && Back_Page == 1 && YES_State == 1)
	{
		Enter = 4;
		Inc =0;
		Back_Page = 0;
		YES_State = 0;
		NO_State = 0;
	}
	else if(Menu==1 && Page_Erase4 == 1 && Enter == 5 && Back_Page == 1 && NO_State == 1)
	{
		Menu = 0;
		Enter = 0;
		Page_Erase1 =0;
		Inc  =0;
		Back_Page = 0;
		YES_State = 0;
		NO_State = 0;
		Index1 = 0,Index2=0;
		Prev_Channel_Int =Prev_Channel_Int +1;
		Prev_Rssi_Int = Prev_Rssi_Int+1;
		Prev_Frequency_Int=Prev_Frequency_Int+1;
		memset(frequency,0,sizeof(frequency));
		frequency[strlen(frequency)]='0';
		memset(channel,0,sizeof(channel));
		channel[strlen(channel)]='0';
	}
	else if(Menu==1 && Page_Erase1 == 1 && Enter == 4)
	{
		if(Saved_flag0==0)
		{
			Saved_flag0=1,Saved_flag1=0,Saved_flag2=0,Saved_flag3=0;
			Page_Erase0=0,Page_Erase1=1,Page_Erase2=0,Page_Erase3=0,Page_Erase4=0;
			Menu=1,Inc=0,Enter=3;
			Prev_Channel_Int =Prev_Channel_Int +1;
			Prev_Rssi_Int = Prev_Rssi_Int+1;
			if(Prev_Channel_Int == Channel_Int)
				Prev_Channel_Int =206;
			if(Prev_Frequency_Int == Frequency_Int)
				Prev_Frequency_Int = 207;
			if(frequency[strlen(frequency)-1] == '_')
			{
				frequency[strlen(frequency)-1]=0;
			}
			if(frequency[0]=='_')
			{
				frequency[0]='0';
			}
			if(frequency[0]==0)
			{
				frequency[0]='0';
			}
			if(frequency[9]==' ')
			{
				frequency[9]=0;
			}
			if(channel[strlen(channel)-1]=='_')
			{
				channel[strlen(channel)-1]=0;
			}
			if(channel[strlen(channel)]=='_')
			{
				channel[strlen(channel)] = 0;
			}
			if(channel[0]=='_')
			{
				channel[0]='0';
			}
			if(channel[0]==0)
			{
				channel[0]='0';
			}
			if(frequency[3] != '.')
			{
				dummy_freq[0] = frequency[0];
				dummy_freq[1] = frequency[1];
				dummy_freq[2] = frequency[2];
				dummy_freq[3] = '.';
				dummy_freq[4] = frequency[3];
				dummy_freq[5] = frequency[4];
				dummy_freq[6] = frequency[5];
				dummy_freq[7] = frequency[6];
				dummy_freq[8] = frequency[7];
				dummy_freq[9] = frequency[8];
				dummy_freq[10] = frequency[9];
				dummy_freq[11] = frequency[10];
				dummy_freq[12] = frequency[11];
				memset(frequency,0,sizeof(frequency));
				memcpy(frequency,dummy_freq,sizeof(dummy_freq));
			}
			Save_CH_TX = atoi(channel);
			Save_frequency_double = atof(frequency);
			if(Save_CH_TX >=1 && Save_CH_TX <=100 && Save_frequency_double>=118 && Save_frequency_double<=136.97500)
			{
				OLED_Frequency = Save_frequency_double;
				OLED_Frequency = OLED_Frequency*100000;
				OLED_Frequency = OLED_Frequency/100000;
				sprintf(frequency,"%0.5f",OLED_Frequency);
				Show_String_12832(1,Saved_in_memory,1,20);
				Show_String_12832(1,CH_NO_Str,3,20); //Need to check channel and assign current frequency
				Show_String_12832(1,(unsigned char *)channel,3,74);
				Save_CH_TX = atoi(channel);
				EEPROM_TX_WRITE(Save_CH_TX, (unsigned char *)frequency, (unsigned char *)channel);
				HAL_Delay(600);
				Fill_RAM_12832(0x00);
			}
			else
			{
				Show_String_12832(1,Not_in_Range,1,20);
				Show_String_12832(1,CH_NO_Str,3,20); //Need to check channel and assign current frequency
				Show_String_12832(1,(unsigned char *)channel,3,74);
				HAL_Delay(600);
				memset(frequency,0,sizeof(frequency));
				if(frequency[0]==0)
				{
					frequency[strlen(frequency)]='0';
				}
				Fill_RAM_12832(0x00);
			}
		}
	}
	else if(Menu==1 && Page_Erase2 == 1 && Enter == 4)
	{
		if(Saved_flag1==0)
		{
			Saved_flag0=0,Saved_flag1=1,Saved_flag2=0,Saved_flag3=0;
			Page_Erase0=0,Page_Erase1=0,Page_Erase2=1,Page_Erase3=0,Page_Erase4=0;
			Menu=1,Enter=3;
			Prev_Channel_Int =Prev_Channel_Int +1;
			Prev_Rssi_Int = Prev_Rssi_Int+1;
			Prev_Frequency_Int=Prev_Frequency_Int+1;
			if(Prev_Channel_Int == Channel_Int)
				Prev_Channel_Int =208;
			if(Prev_Frequency_Int == Frequency_Int)
				Prev_Frequency_Int = 209;
			if(frequency[strlen(frequency)-1] == '_')
			{
				frequency[strlen(frequency)-1]=0;
			}
			if(frequency[0]=='_')
			{
				frequency[0]='0';
			}
			if(frequency[0]==0)
			{
				frequency[0]='0';
			}
			if(frequency[9]==' ')
			{
				frequency[9]=0;
			}
			if(channel[strlen(channel)-1]=='_')
			{
				channel[strlen(channel)-1]=0;
			}
			if(channel[strlen(channel)]=='_')
			{
				channel[strlen(channel)] = 0;
			}
			Read_CH_TX = atoi(channel);
			if(Read_CH_TX>=1 && Read_CH_TX<=100)
			{
				if(Read_CH_TX!=0)
				{
					EEPROM_TX_READ(Read_CH_TX,(unsigned char*)EEPROM_Freq, (unsigned char*)EEPROM_CH);
					printf("Read Freq:%s \t,CH: %s\r\n",EEPROM_Freq,EEPROM_CH);
				}
				if(EEPROM_Freq[0]==0xff)
				{
					Show_String_12832(1,Not_Available_in_memory,1,20);
					Show_String_12832(1,CH_NO_Str,3,20); //Need to check channel and assign current frequency
					Show_String_12832(1,(unsigned char *)channel,3,74);
					HAL_Delay(600);
					memset(EEPROM_CH,0,sizeof(EEPROM_CH));
					memset(EEPROM_Freq,0,sizeof(EEPROM_Freq));
					memset(frequency,0,sizeof(frequency));
					if(EEPROM_CH[0]==0)
					{
						EEPROM_CH[strlen(EEPROM_CH)]='0';
					}
					if(EEPROM_Freq[0]==0)
					{
						EEPROM_Freq[strlen(EEPROM_Freq)]='0';
					}
					if(frequency[0]==0)
					{
						frequency[strlen(frequency)]='0';
					}
					Fill_RAM_12832(0x00);
					TX_Mode_Flag =0;
					Inc=1;
				}
				else
				{
					Show_String_12832(1,Featching_in_memory,1,4);
					Show_String_12832(1,CH_NO_Str,3,20); //Need to check channel and assign current frequency
					Show_String_12832(1,(unsigned char *)EEPROM_CH,3,74);
					HAL_Delay(600);
					Fill_RAM_12832(0x00);
					memcpy(frequency,EEPROM_Freq,sizeof(EEPROM_Freq));
					memcpy(channel,EEPROM_CH,sizeof(EEPROM_CH));
					memcpy(Current_Freq,frequency,sizeof(frequency));
					memcpy(Current_CH,channel,sizeof(channel));
					Frequency_Int = atof(frequency);
					//PTT_BT_Pressed_TX(Frequency_Int);
					Keypad_Frequency_RX(Frequency_Int);
					RX_CTRL_1;
					RX_MUTE_CTRL_1;
					Current_Freq_Write((unsigned char *)EEPROM_Freq, (unsigned char *)EEPROM_CH);
					TX_Mode_Flag = 1;
					Inc=1;
					Index2 = 0;
				}
			}
			else if(Read_CH_TX == 0)
			{
				Manual_Freq_Applying();
			}
			else
			{
				Show_String_12832(1,Not_in_Range,1,20);
				Show_String_12832(1,CH_NO_Str,3,20); //Need to check channel and assign current frequency
				Show_String_12832(1,(unsigned char *)channel,3,74);
				HAL_Delay(600);
				memset(EEPROM_CH,0,sizeof(EEPROM_CH));
				memset(frequency,0,sizeof(frequency));
				if(EEPROM_CH[0]==0)
				{
					EEPROM_CH[strlen(EEPROM_CH)]='0';
				}
				if(Current_Freq[0]==0)
				{
					EEPROM_Freq[strlen(EEPROM_Freq)]='0';
				}
				if(frequency[0]==0)
				{
					frequency[strlen(frequency)]='0';
				}
				Fill_RAM_12832(0x00);
				TX_Mode_Flag =0;
				Inc=1;
			}
		}
	}
	else if(Menu==1 && Page_Erase3 == 1 && Enter == 4)
	{
		if(Saved_flag2==0)
		{
			Saved_flag0=0,Saved_flag1=0,Saved_flag2=1,Saved_flag3=0;
			Page_Erase0=0,Page_Erase1=0,Page_Erase2=0,Page_Erase3=1,Page_Erase4=0;
			Menu=1,Inc=0,Enter=3;
			Prev_Channel_Int =Prev_Channel_Int +1;
			Prev_Rssi_Int = Prev_Rssi_Int+1;
			Prev_Frequency_Int=Prev_Frequency_Int+1;
			if(Prev_Channel_Int == Channel_Int)
				Prev_Channel_Int =300;
			if(Prev_Frequency_Int == Frequency_Int)
				Prev_Frequency_Int = 302;
			if(frequency[strlen(frequency)-1] == '_')
			{
				frequency[strlen(frequency)-1]=0;
			}
			if(frequency[0]==0)
			{
				frequency[0]='0';
			}
			if(channel[strlen(channel)-1]=='_')
			{
				channel[strlen(channel)-1]=0;
			}
			if(channel[0]==0)
			{
				channel[strlen(channel)]='0';
			}
			if(frequency[3] != '.')
			{
				dummy_freq[0] = frequency[0];
				dummy_freq[1] = frequency[1];
				dummy_freq[2] = frequency[2];
				dummy_freq[3] = '.';
				dummy_freq[4] = frequency[3];
				dummy_freq[5] = frequency[4];
				dummy_freq[6] = frequency[5];
				dummy_freq[7] = frequency[6];
				dummy_freq[8] = frequency[7];
				dummy_freq[9] = frequency[8];
				dummy_freq[10] = frequency[9];
				memset(frequency,0,sizeof(frequency));
				memcpy(frequency,dummy_freq,sizeof(dummy_freq));
			}
			Save_CH_RX = atoi(channel);
			Save_frequency_double = atof(frequency);
			if(Save_CH_RX >=1 && Save_CH_RX <=100 && Save_frequency_double>=118 && Save_frequency_double<=136.97500)
			{
				OLED_Frequency = Save_frequency_double;
				OLED_Frequency = OLED_Frequency*100000;
				OLED_Frequency = OLED_Frequency/100000;
				sprintf(frequency,"%0.5f",OLED_Frequency);
				Show_String_12832(1,Saved_in_memory,1,20);
				Show_String_12832(1,CH_NO_Str,3,20); //Need to check channel and assign current frequency
				Show_String_12832(1,(unsigned char *)channel,3,74);
				Save_CH_TX = atoi(channel);
				EEPROM_RX_WRITE(Save_CH_RX, (unsigned char *)frequency, (unsigned char *)channel);
				HAL_Delay(600);
				Fill_RAM_12832(0x00);
			}
			else
			{
				Show_String_12832(1,Not_in_Range,1,20);
				Show_String_12832(1,CH_NO_Str,3,20); //Need to check channel and assign current frequency
				Show_String_12832(1,(unsigned char *)channel,3,74);
				HAL_Delay(1000);
				memset(EEPROM_CH,0,sizeof(EEPROM_CH));
				memset(frequency,0,sizeof(frequency));
				if(EEPROM_CH[0]==0)
				{
					EEPROM_CH[strlen(EEPROM_CH)]='0';
				}
				if(EEPROM_Freq[0]==0)
				{
					EEPROM_Freq[strlen(EEPROM_Freq)]='0';
				}
				if(frequency[0]==0)
				{
					frequency[strlen(frequency)]='0';
				}
				Fill_RAM_12832(0x00);
			}
		}
	}
	else if(Menu==1 && Page_Erase4 == 1 && Enter == 4)
	{
		if(Saved_flag3==0)
		{
			Saved_flag0=0,Saved_flag1=0,Saved_flag2=0,Saved_flag3=1;
			Page_Erase0=0,Page_Erase1=0,Page_Erase2=0,Page_Erase3=0,Page_Erase4=1;
			Menu=1,Enter=3;
			Prev_Channel_Int =Prev_Channel_Int +1;
			Prev_Rssi_Int = Prev_Rssi_Int+1;
			Prev_Frequency_Int=Prev_Frequency_Int+1;
			if(Prev_Channel_Int == Channel_Int)
				Prev_Channel_Int =303;
			if(Prev_Frequency_Int == Frequency_Int)
				Prev_Frequency_Int = 304;
			if(channel[strlen(channel)-1]=='_')
			{
				channel[strlen(channel)-1] = 0;
			}
			if(frequency[strlen(frequency)-1]=='_')
			{
				frequency[strlen(frequency)-1] = 0;
			}
			if(channel[0] == 0)
			{
				channel[0] = '0';
			}
			Read_CH_RX = atoi(channel);
			if(Read_CH_RX>=1 && Read_CH_RX<=100)
			{
				if(Read_CH_RX!=0)
				{
					EEPROM_RX_READ(Read_CH_RX,(unsigned char*)EEPROM_Freq, (unsigned char*)EEPROM_CH);
					printf("Read Freq:%s \t,CH: %s\r\n",EEPROM_Freq,EEPROM_CH);
				}
				if(EEPROM_Freq[0]==0xff)
				{
					Show_String_12832(1,Not_Available_in_memory,1,20);
					Show_String_12832(1,CH_NO_Str,3,20); //Need to check channel and assign current frequency
					if(channel[strlen(channel)-1] == '_')
					{
						channel[strlen(channel)-1] = 0;
					}
					if(channel[0]==0)
					{
						channel[strlen(channel)]='0';
					}
					Show_String_12832(1,(unsigned char *)channel,3,74);
					HAL_Delay(600);
					memset(EEPROM_CH,0,sizeof(EEPROM_CH));
					memset(EEPROM_Freq,0,sizeof(EEPROM_Freq));
					memset(frequency,0,sizeof(frequency));
					if(EEPROM_CH[0]==0)
					{
						EEPROM_CH[strlen(EEPROM_CH)]='0';
					}
					if(EEPROM_Freq[0]==0)
					{
						EEPROM_Freq[strlen(EEPROM_Freq)]='0';
					}
					if(frequency[0]==0)
					{
						frequency[strlen(frequency)]='0';
					}
					Fill_RAM_12832(0x00);
					RX_Mode_Flag = 0;
					Inc=2;
				}
				else
				{
					Show_String_12832(1,Featching_in_memory,1,4);
					Show_String_12832(1,CH_NO_Str,3,20); //Need to check channel and assign current frequency
					Show_String_12832(1,(unsigned char *)EEPROM_CH,3,74);
					memcpy(frequency,EEPROM_Freq,sizeof(EEPROM_Freq));
					memcpy(channel,EEPROM_CH,sizeof(EEPROM_CH));
					memcpy(Current_Freq,frequency,sizeof(frequency));
					memcpy(Current_CH,channel,sizeof(channel));
					Frequency_Int = atof(EEPROM_Freq);
					Keypad_Frequency_RX(Frequency_Int);
					RX_CTRL_1;
					RX_MUTE_CTRL_1;
					Current_Freq_Write((unsigned char *)EEPROM_Freq, (unsigned char *)EEPROM_CH);
					RX_Mode_Flag = 1;
					Inc=2;
					Fill_RAM_12832(0x00);
				}
			}
			else if(Read_CH_RX == 0)
			{
				if(frequency[3] != '.')
				{
					dummy_freq[0] = frequency[0];
					dummy_freq[1] = frequency[1];
					dummy_freq[2] = frequency[2];
					dummy_freq[3] = '.';
					dummy_freq[4] = frequency[3];
					dummy_freq[5] = frequency[4];
					dummy_freq[6] = frequency[5];
					dummy_freq[7] = frequency[6];
					dummy_freq[8] = frequency[7];
					dummy_freq[9] = frequency[8];
					dummy_freq[10] = frequency[9];
					memset(frequency,0,sizeof(frequency));
					memcpy(frequency,dummy_freq,sizeof(dummy_freq));
				}
				Save_frequency_double = atof(frequency);
				if(Save_frequency_double>=118 && Save_frequency_double<=136.97500)
				{
					if(frequency[strlen(frequency)-1] == '_')
					{
						frequency[strlen(frequency)-1]=0;
					}
					OLED_Frequency = Save_frequency_double;
					OLED_Frequency = OLED_Frequency*100000;
					OLED_Frequency = OLED_Frequency/100000;
					sprintf(frequency,"%0.5f",OLED_Frequency);
					memcpy(Current_Freq,frequency,sizeof(frequency));
					memcpy(Current_CH,channel,sizeof(channel));
					if(Current_CH[0] == 0)
					{
						Current_CH[0] = '0';
					}
					Frequency_Int = atof(frequency);
					Keypad_Frequency_RX(Frequency_Int);
					RX_CTRL_1;
					RX_MUTE_CTRL_1;
					Current_Freq_Write((unsigned char *)Current_Freq, (unsigned char *)Current_CH);
					RX_Mode_Flag = 1;
					Inc=0;
					Show_String_12832(1,(unsigned char*)"  SET FREQUENCY - RX  ",2,4);
					HAL_Delay(1000);
					Show_String_12832(1,(unsigned char*)"                      ",2,4);
				}
				else
				{
					Show_String_12832(1,Not_in_Range,1,20);
					Show_String_12832(1,(unsigned char*)"FREQ : ",3,10);
					Show_String_12832(1,(unsigned char*)frequency,3,52);
					Show_String_12832(1,(unsigned char*)"MHz",3,106);
					HAL_Delay(600);
					memset(frequency,0,sizeof(frequency));
					if(Current_CH[0]==0)
					{
						Current_CH[strlen(Current_CH)]='0';
					}
					if(Current_Freq[0]==0)
					{
						Current_Freq[strlen(Current_Freq)]='0';
					}
					if(frequency[0]==0)
					{
						frequency[strlen(frequency)]='0';
					}
					Fill_RAM_12832(0x00);
					Inc=0;
				}
			}
			else
			{
				Show_String_12832(1,Not_in_Range,1,20);
				Show_String_12832(1,CH_NO_Str,3,20); //Need to check channel and assign current frequency
				Show_String_12832(1,(unsigned char *)channel,3,74);
				HAL_Delay(600);
				memset(frequency,0,sizeof(frequency));
				if(Current_CH[0]==0)
				{
					Current_CH[strlen(Current_CH)]='0';
				}
				if(Current_Freq[0]==0)
				{
					Current_Freq[strlen(Current_Freq)]='0';
				}
				if(frequency[0]==0)
				{
					frequency[strlen(frequency)]='0';
				}
				Fill_RAM_12832(0x00);
				RX_Mode_Flag = 0;
				Inc=2;
			}
		}
	}
	if(Menu==0 && Enter>=1 && Manual_FRQ_Entry == 0)
	{
		Enter =0;
		Prev_Channel_Int=Prev_Channel_Int+2;
		Prev_Rssi_Int=Prev_Rssi_Int+2;
		Prev_Frequency_Int=Prev_Frequency_Int+2;
		if(Prev_Channel_Int == Channel_Int)
			Prev_Channel_Int = 310;
		if(Prev_Frequency_Int == Prev_Frequency_Int)
			Prev_Channel_Int = 311;
		if(Prev_Rssi_Int == Prev_Rssi_Int)
			Prev_Rssi_Int = 311;
		PTT_RX_State=1;
	}
	if(Menu==0 && Enter>=2 && Manual_FRQ_Entry == 1)
	{
		Enter =0;
		Prev_Channel_Int=Prev_Channel_Int+3;
		Prev_Rssi_Int=Prev_Rssi_Int+3;
		Prev_Frequency_Int=Prev_Frequency_Int+2;
		if(Prev_Channel_Int == Channel_Int)
			Prev_Channel_Int = 312;
		if(Prev_Frequency_Int == Prev_Frequency_Int)
			Prev_Channel_Int = 313;
		if(Prev_Rssi_Int == Prev_Rssi_Int)
			Prev_Rssi_Int = 314;
		PTT_RX_State=1;
	}
	if(Login_Entry_Flag == 1 && Enter == 2 && Password_Reset_Flag == 0)
	{
		Enter = 1;
		Fill_RAM_12832(0x00);
	}
	if(Admin_Entry_flag == 1 && Enter == 3 && Password_Reset_Flag == 1)
	{
		Enter = 2;
		Fill_RAM_12832(0x00);
	}
	if(Prev_Channel_Int>400)
	{
		Prev_Channel_Int = 300;
	}
	if(Prev_Rssi_Int>400)
	{
		Prev_Rssi_Int = 300;
	}
	if(Prev_Frequency_Int>400)
	{
		Prev_Frequency_Int = 300;
	}
}
void Manual_Freq_Applying(void)
{
	if(frequency[3] != '.')
	{
		dummy_freq[0] = frequency[0];
		dummy_freq[1] = frequency[1];
		dummy_freq[2] = frequency[2];
		dummy_freq[3] = '.';
		dummy_freq[4] = frequency[3];
		dummy_freq[5] = frequency[4];
		dummy_freq[6] = frequency[5];
		dummy_freq[7] = frequency[6];
		dummy_freq[8] = frequency[7];
		dummy_freq[9] = frequency[8];
		dummy_freq[10] = frequency[9];
		dummy_freq[11] = frequency[10];
		dummy_freq[12] = frequency[11];
		memset(frequency,0,sizeof(frequency));
		memcpy(frequency,dummy_freq,sizeof(dummy_freq));
	}
	Save_frequency_double = atof(frequency);
	if(Save_frequency_double>=118 && Save_frequency_double<=136.97500)
	{
		if(frequency[strlen(frequency)-1] == '_')
		{
			frequency[strlen(frequency)-1]=0;
		}
		OLED_Frequency = Save_frequency_double;
		OLED_Frequency = OLED_Frequency*100000;
		OLED_Frequency = OLED_Frequency/100000;
		sprintf(frequency,"%0.5f",OLED_Frequency);
		memcpy(Current_Freq,frequency,sizeof(frequency));
		memcpy(Current_CH,channel,sizeof(channel));
		if(EEPROM_CH[0] == 0)
		{
			EEPROM_CH[0] = '0';
		}
		Frequency_Int = atof(frequency);
		//PTT_BT_Pressed_TX(Frequency_Int);
		Keypad_Frequency_RX(Frequency_Int);
		RX_CTRL_1;
		RX_MUTE_CTRL_1;
		Current_Freq_Write((unsigned char *)Current_Freq, (unsigned char *)Current_CH);
		TX_Mode_Flag = 1;
		Inc=0;
		Show_String_12832(1,(unsigned char*)"  SET FREQUENCY - RX  ",2,4);
		HAL_Delay(600);
		Show_String_12832(1,(unsigned char*)"                      ",2,4);
	}
	else
	{
		Show_String_12832(1,Not_in_Range,1,20);
		Show_String_12832(1,(unsigned char*)"Freq : ",3,10);
		Show_String_12832(1,(unsigned char*)frequency,3,52);
		Show_String_12832(1,(unsigned char*)"MHz",3,106);
		HAL_Delay(600);
		memset(EEPROM_CH,0,sizeof(EEPROM_CH));
		memset(frequency,0,sizeof(frequency));
		if(EEPROM_CH[0]==0)
		{
			EEPROM_CH[strlen(EEPROM_CH)]='0';
		}
		if(EEPROM_Freq[0]==0)
		{
			EEPROM_Freq[strlen(EEPROM_Freq)]='0';
		}
		if(frequency[0]==0)
		{
			frequency[strlen(frequency)]='0';
		}
		Fill_RAM_12832(0x00);
		Inc=0;
	}
}
void Password_Login_Page(void)
{
	Inc_State=0;
	if(PassWord_flag==0 && Inc == 0)
	{
		if(Pass0==0)
		{
			Pass0=1,Pass1=0,Pass2=0;Inc=1;
		}
	}
	if(PassWord_flag==0 && Inc == 1)
	{
		if(Pass1==0)
		{
			Login_Entry_Flag = 1;
			Pass0=0,Pass1=0,Pass2=0,Password_Reset_Flag=0;
			Show_String_12832(1,Enter_password_1,1,20);
			Show_String_12832(1,Reset_password,2,20);
			Show_String_12832(1,(unsigned char *)"     ",1,92);
			HAL_Delay(10);
			if(strlen(Password) == 0)
			{
				Password[0] = '0';
			}
			Show_String_12832(1,(unsigned char *)Password,1,92);
			HAL_Delay(10);
			if(strlen(passwordmask) == 0)
			{
				passwordmask[0] = '0';
			}
			Show_String_12832(1,(unsigned char *)passwordmask,1,92);
			HAL_Delay(100);
			Password_Length = strlen(Password);
			if(Password_Length>=5)
			{
				strcpy(Compare_PassWord,Password);
				if(Compare_PassWord[strlen(Compare_PassWord)-1]=='_')
				{
					Compare_PassWord[strlen(Compare_PassWord)-1]=0;
				}
				Enter_Password_State = strcmp(Saved_Password,Compare_PassWord);
				if(Enter_Password_State==0)
				{
					Login_Entry_Flag = 0;
					Enter =3;
					Inc=0;
					Fill_RAM_12832(0x00);
					memset(Password,0,sizeof(Password));//save password in EEPROM
					Password[strlen(Password)]='0';
					memset(passwordmask,0,sizeof(passwordmask));
					passwordmask[strlen(passwordmask)]='0';
					Index3=0;
					Show_String_12832(1,(unsigned char *)"    LOGIN Success     ",2,4);
					HAL_Delay(1000);
					Fill_RAM_12832(0x00);
				}
				else if(Enter_Password_State>1)
				{
					Enter_Password_State =1;
					memset(Password,0,sizeof(Password));
					Password[strlen(Password)]='0';
					memset(passwordmask,0,sizeof(passwordmask));
					passwordmask[strlen(passwordmask)]='0';
					Index3=0;
					Fill_RAM_12832(0x00);
					Show_String_12832(1,(unsigned char *)"    LOGIN Failed     ",2,4);
					HAL_Delay(1000);
					Fill_RAM_12832(0x00);
				}
			}
		}
	}
	if(PassWord_flag==0 && Inc == 2)
	{
		if(Pass2==0)
		{
			Pass0=0,Pass1=0,Pass2=0,Password_Reset_Flag=1;
			Show_String_12832(1,Enter_password,1,20);
			Show_String_12832(1,Reset_password_1,2,20);
		}
	}
}
void Save_Changes_Apply(void)
{
	if(Reset_State==0)
	{
		Reset_State = 1;
		Fill_RAM_12832(0x00);
	}
	Show_String_12832(1,(unsigned char *)"Save Settings",1,30);
	Show_String_12832(1,(unsigned char *)"YES",3,30);
	Show_String_12832(1,(unsigned char *)"NO",3,90);
	if(Inc==0)
	{
		YES_State =1;
		NO_State =0;
		Show_String_12832(1,(unsigned char *)Clear_Arrow1_1,3,18);
		Show_String_12832(1,(unsigned char *)"  ",3,78);
	}
	if(Inc==1)
	{
		NO_State =1;
		YES_State =0;
		Show_String_12832(1,(unsigned char *)Clear_Arrow1_1,3,78);
		Show_String_12832(1,(unsigned char *)"  ",3,18);
	}
}
void Reset_Passwords(void)
{
	if(Admin_Reset_State == 0)
	{
		Login_Entry_Flag = 0;
		Admin_Entry_flag = 1;
		Admin_Pass_Length = strlen(Admin_Password_Entry);
		Admin_Password_Entry_State =1;
		Admin_Password_State =5;
		if(Admin_Pass_Length>=5)
		{
			if(Admin_Password_Entry[strlen(Admin_Password_Entry)-1]=='_')
				Admin_Password_Entry[strlen(Admin_Password_Entry)-1]=0;
			else if(Admin_Password_Entry[strlen(Admin_Password_Entry)-1]==' ')
				Admin_Password_Entry[strlen(Admin_Password_Entry)-1]=0;
			else if(Admin_Password_Entry[strlen(Admin_Password_Entry)]=='_')
				Admin_Password_Entry[strlen(Admin_Password_Entry)]=0;
			Admin_Password_State = strcmp(Admin_Password,Admin_Password_Entry);
		}
		if(Admin_Password_State==5)
		{

			Show_String_12832(1, (unsigned char *)"Admin Pass: ",2,10);
			Show_String_12832(1,(unsigned char *)"      ",2,82);
			HAL_Delay(10);
			Show_String_12832(1,(unsigned char *)Admin_Password_Entry,2,82);
			HAL_Delay(10);
			Show_String_12832(1,(unsigned char *)Admin_Password_Entry_Mask,2,82);
			HAL_Delay(100);
		}
		else if(Admin_Password_State == 0)
		{
			Admin_Password_State = 0;
			Show_String_12832(1,(unsigned char *)Admin_Password_Entry,2,82);
			HAL_Delay(50);
			Show_String_12832(1,(unsigned char *)Admin_Password_Entry_Mask,2,82);
			HAL_Delay(20);
			Fill_RAM_12832(0x00);
			Show_String_12832(1, (unsigned char *)" Admin Login Success ",2,4);
			HAL_Delay(600);
			Fill_RAM_12832(0x00);
			Admin_Reset_State =1;
			Admin_Password_Entry_State =0;
			Index6 =0;
			memset(Admin_Password_Entry,0,sizeof(Admin_Password_Entry));
			Admin_Password_Entry[strlen(Admin_Password_Entry)]='0';
			memset(Admin_Password_Entry_Mask,0,sizeof(Admin_Password_Entry_Mask));
			Admin_Password_Entry_Mask[strlen(Admin_Password_Entry_Mask)]='0';
		}
		else if(Admin_Password_State >=1)
		{
			Admin_Entry_flag = 0;
			Show_String_12832(1,(unsigned char *)Admin_Password_Entry,2,82);
			HAL_Delay(20);
			Show_String_12832(1,(unsigned char *)Admin_Password_Entry_Mask,2,82);
			HAL_Delay(100);
			Fill_RAM_12832(0x00);
			Show_String_12832(1, (unsigned char *)" Admin Login Failed ",1,4);
			Show_String_12832(1, (unsigned char *)"Contact Administration",3,4);
			HAL_Delay(1500);
			Fill_RAM_12832(0x00);
			Index6 =0;
			memset(Admin_Password_Entry,0,sizeof(Admin_Password_Entry));
			Admin_Password_Entry[strlen(Admin_Password_Entry)]='0';
			memset(Admin_Password_Entry_Mask,0,sizeof(Admin_Password_Entry_Mask));
			Admin_Password_Entry_Mask[strlen(Admin_Password_Entry_Mask)]='0';
			Index6 =0;
			Page_Erase0=0,Page_Erase1=0,Page_Erase2=0,Page_Erase3=0,Page_Erase4=0;
			Menu=0,Inc=0,Enter=0;
			Prev_Channel_Int =Prev_Channel_Int +1;
			Prev_Rssi_Int = Prev_Rssi_Int+1;
			Prev_Frequency_Int=Prev_Frequency_Int+1;
			Password_Reset_Flag = 0;
			Admin_Password_Entry_State =0;
		}

	}
	else if(Admin_Reset_State==1)
	{
		Enter_Pass_Length = strlen(Enter_New_Pass);
		Re_Enter_Pass_Length = strlen(Re_Enter_Pass);
		Password_State=1;
		if(Enter_Pass_Length >= 5 && Re_Enter_Pass_Length >= 5)
		{
			if(Enter_New_Pass[strlen(Enter_New_Pass)-1]=='_')
				Enter_New_Pass[strlen(Enter_New_Pass)-1]=0;
			else if(Enter_New_Pass[strlen(Enter_New_Pass)-1]==' ')
				Enter_New_Pass[strlen(Enter_New_Pass)-1]=0;
			if(Re_Enter_Pass[strlen(Re_Enter_Pass)-1]=='_')
				Re_Enter_Pass[strlen(Re_Enter_Pass)-1] = 0;
			else if(Re_Enter_Pass[strlen(Re_Enter_Pass)-1]==' ')
				Re_Enter_Pass[strlen(Re_Enter_Pass)-1] = 0;
			Password_State = strcmp(Enter_New_Pass,Re_Enter_Pass);
		}
		if(Password_State == 1)
		{
			Show_String_12832(1,(unsigned char *)Enter_New_Pass_Disp,1,4);
			if(Enter_New_Pass[0]==0)
			{
				Enter_New_Pass[0] = '0';
			}
			else if(Enter_New_Pass[0]==' ')
			{
				Enter_New_Pass[0] = '0';
			}
			if(Inc==0)
			{
				Show_String_12832(1,(unsigned char *)"     ",1,102);
				HAL_Delay(10);
				if(Enter_New_Pass[strlen(Enter_New_Pass)-1]==' ')
				{
					Enter_New_Pass[strlen(Enter_New_Pass)-1] = 0;
				}
				if(Re_Enter_Pass[strlen(Re_Enter_Pass)-1]=='_')
				{
					Re_Enter_Pass[strlen(Re_Enter_Pass)-1] = ' ';
				}
				else if(Re_Enter_Pass[strlen(Re_Enter_Pass)]=='_')
				{
					Re_Enter_Pass[strlen(Re_Enter_Pass)] = ' ';
				}
				Show_String_12832(1,(unsigned char *)Enter_New_Pass,1,102);
				HAL_Delay(50);
				if(Undersore == 1)
				{
					Undersore = 0;
					Show_String_12832(1,(unsigned char *)"     ",3,96);
				}
			}
			if(Enter_New_Pass[0] != 0 || Enter_New_Pass[0] != '0')
			{
				if(Enter_New_Pass_Mask[0]!=0)
					Show_String_12832(1,(unsigned char *)Enter_New_Pass_Mask,1,102);
			}
			Show_String_12832(1,(unsigned char *)Re_Enter_Pass_Disp,3,4);
			if(Re_Enter_Pass[0]==0)
			{
				Re_Enter_Pass[0] = '0';
			}
			else if(Re_Enter_Pass[0]==' ')
			{
				Re_Enter_Pass[0] = '0';
			}
			if(Inc==1)
			{
				if(Undersore == 0)
				{
					Undersore = 1;
					Show_String_12832(1,(unsigned char *)"     ",1,102);
				}
				Show_String_12832(1,(unsigned char *)"     ",3,96);
				HAL_Delay(10);
				if(Re_Enter_Pass[strlen(Re_Enter_Pass)-1]==' ')
				{
					Re_Enter_Pass[strlen(Re_Enter_Pass)-1] = 0;
				}
				if(Enter_New_Pass[strlen(Enter_New_Pass)-1]=='_')
				{
					Enter_New_Pass[strlen(Enter_New_Pass)-1] = ' ';
				}
				else if(Enter_New_Pass[strlen(Enter_New_Pass)]=='_')
				{
					Enter_New_Pass[strlen(Enter_New_Pass)] = ' ';
				}
				if(Enter_New_Pass[0]=='_')
				{
					Enter_New_Pass[0] = '0';
				}
				if(Enter_New_Pass[0]==0)
				{
					Enter_New_Pass[0] = '0';
				}
				Show_String_12832(1,(unsigned char *)Re_Enter_Pass,3,96);
				HAL_Delay(50);
			}
			if(Re_Enter_Pass[0] != 0 || Re_Enter_Pass[0] != '0')
			{
				if(Re_Enter_Pass_Mask[0]!=0)
					Show_String_12832(1,(unsigned char *)Re_Enter_Pass_Mask,3,96);
			}
		}
		else if(Password_State == 0)
		{
			Fill_RAM_12832(0x00);
			Enter = 3;
			Inc=0;
			Password_Reset_Flag =0;
			Admin_Entry_flag = 0;
			Login_Entry_Flag = 0;
			Show_String_12832(1,(unsigned char *)"   Saved Password    ",1,4);
			Show_String_12832(1,(unsigned char *)"    Successfully     ",3,4);
			if(Enter_New_Pass[strlen(Enter_New_Pass)-1]=='_')
			{
				Enter_New_Pass[strlen(Enter_New_Pass)-1] = 0;
			}
			strcpy(Saved_Password,Enter_New_Pass);
			//printf("Saved_Password:%s\r\n",Saved_Password);
			EEPROM_Write(0,	 5, (unsigned char *)Saved_Password, strlen(Saved_Password));
			memset(Enter_New_Pass,0,sizeof(Enter_New_Pass));
			memset(Re_Enter_Pass,0,sizeof(Re_Enter_Pass));
			Enter_New_Pass[strlen(Enter_New_Pass)]='0';
			Re_Enter_Pass[strlen(Re_Enter_Pass)]='0';
			memset(Re_Enter_Pass_Mask,0,sizeof(Re_Enter_Pass_Mask));
			Re_Enter_Pass_Mask[strlen(Re_Enter_Pass_Mask)]='0';
			memset(Enter_New_Pass_Mask,0,sizeof(Enter_New_Pass_Mask));
			Enter_New_Pass_Mask[strlen(Enter_New_Pass_Mask)]='0';
			Index4=0,Index5=0;
			Admin_Reset_State =0;
			HAL_Delay(1000);
			Fill_RAM_12832(0x00);
		}
		else
		{
			Password_State=1;
			Fill_RAM_12832(0x00);
			memset(Enter_New_Pass,0,sizeof(Enter_New_Pass));
			memset(Re_Enter_Pass,0,sizeof(Re_Enter_Pass));
			Enter_New_Pass[strlen(Enter_New_Pass)]='0';
			Re_Enter_Pass[strlen(Re_Enter_Pass)]='0';
			memset(Re_Enter_Pass_Mask,0,sizeof(Re_Enter_Pass_Mask));
			Re_Enter_Pass_Mask[strlen(Re_Enter_Pass_Mask)]='0';
			memset(Enter_New_Pass_Mask,0,sizeof(Enter_New_Pass_Mask));
			Enter_New_Pass_Mask[strlen(Enter_New_Pass_Mask)]='0';
			Index5=0;
			Index4=0;
			Show_String_12832(1,(unsigned char *)"Passwords Not Matched",1,4);
			Show_String_12832(1,(unsigned char *)"Please Re Enter",3,30);
			Inc=0;//add on
			HAL_Delay(1000);
			Fill_RAM_12832(0x00);
		}
	}
}
void Frequency_Reading(void)
{
	Index1 = strlen(frequency)-1;
	if(Index1<9)
	{
		frequency[Index1]=Keydata;
		frequency[strlen(frequency)]='_';
		Index1++;
	}
	if(Index1==3)
	{
		frequency[Index1]='.';
		frequency[strlen(frequency)]='_';
		Index1++;
	}
	Keydata =0;
}
void Channel_Reading(void)
{
	Index2=strlen(channel)-1;
	if(Index2<0)
		Index2=0;
	if(Index2>=0 && Index2<3)
	{
		channel[Index2]=Keydata;
		channel[strlen(channel)]='_';
		Index2++;
	}
	Keydata =0;
}
void Frequency_Deleting(void)
{
	if(frequency[strlen(frequency)-1]=='_')
	{
		frequency[strlen(frequency)-1]=0;
		Index1=strlen(frequency);
		Index1--;
		if(Index1<0)
			Index1= 0;
		Size = Index1;
		frequency[Size]='_';
		Keydata =0;
	}
	else
	{
		Index1 = strlen(frequency);
		if(Index1<0)
			Index1 = 0;
		frequency[Index1]='_';
		Index1 = strlen(frequency);
		if(Index1!=0)
			Index1--;
		Keydata=0;
	}
}
void Channel_Deleting(void)
{
	if(channel[strlen(channel)-1]=='_')
	{
		Index2 = strlen(channel);
		if(Index2<0)
			Index2 = 0;
		channel[Index2-1]=0;
		Index2 = strlen(channel);
		if(Index2!=0)
			Index2--;
		Size = Index2;
		channel[Size]='_';
		Keydata=0;
	}
	else
	{
		Index2 = strlen(channel);
		if(Index2<0)
			Index2 = 0;
		channel[Index2]='_';
		Index2 = strlen(channel);
		if(Index2!=0)
			Index2--;
		Keydata=0;
	}
}
void Manual_FREQ_Reading(void)
{
	if(Keydata=='*')
	{
		Frequency_Deleting();
	}
	else
	{
		if(Keydata!='#' && Keydata!=0)
		{
			Frequency_Reading();
		}
	}
}
void Freq_Channel_Reading(void)
{
	if(Keydata=='*')
	{
		if(Keydata!=0 && Inc ==0 && Get_Tx_CH_Flag == 0 && Page_Erase1 ==1)
		{
			Channel_Deleting();
		}
		if(Keydata!=0 && Inc ==0 && Get_Rx_CH_Flag == 0 && Page_Erase3 ==1)
		{
			Channel_Deleting();
		}
		else if(Keydata!=0 && Inc ==0 )
		{
			if(Page_Erase1 == 1 || Page_Erase3 == 1)
			{
				Frequency_Deleting();
			}
			else if(Page_Erase2 == 1 || Page_Erase4 == 1)
			{
				Frequency_Deleting();
			}
		}
		else if(Keydata!=0 && Inc >=1)
		{
			if(Page_Erase1 == 1)
			{
				if(Inc == 1)
					Channel_Deleting();
			}
			else if(Page_Erase2 == 1)
			{
				if(Inc == 1)
					Channel_Deleting();
			}
			else if(Page_Erase3 == 1)
			{
				if(Get_Rx_CH_Flag == 0)
				{
					//if(Inc == 0 || Inc == 1)
						Channel_Deleting();
				}
				if(Inc == 2)
					Channel_Deleting();
			}
			else if(Page_Erase4 == 1)
			{
				if(Get_Rx_CH_Flag == 0)
				{
					//if(Inc == 0 || Inc == 1)
						Channel_Deleting();
				}
				if(Inc == 2)
					Channel_Deleting();
			}
		}
		Keydata=0;
	}
	else
	{
		if(Keydata!='#')
		{

			if(Keydata!=0 && Inc ==0 && Get_Tx_CH_Flag == 0 && Page_Erase1 ==1)
			{
				Channel_Reading();
			}
			if(Keydata!=0 && Inc ==0 && Get_Rx_CH_Flag == 0 && Page_Erase3 ==1)
			{
				Channel_Reading();
			}
			else if(Keydata!=0 && Inc ==0)
			{
				if(Page_Erase1 == 1 || Page_Erase3 == 1)
				{
					Frequency_Reading();
				}
				else if(Page_Erase2 == 1 || Page_Erase4 == 1)
				{
					Frequency_Reading();
				}
			}
			else if(Keydata!=0 && Inc >=1)
			{
				if(Page_Erase1 == 1 || Page_Erase2 == 1)
				{
					if(Inc ==1)
					{
						Channel_Reading();
					}
				}
				else if(Page_Erase3 == 1)
				{
					if(Get_Rx_CH_Flag == 0)
					{
						Channel_Reading();
					}
					if(Inc ==1)
					{
//						if(Keydata=='1')
//						{
//							Squelth_Read_State = 1;
//						}
//						else if(Keydata == '2')
//						{
//							Squelth_Read_State = 0;
//						}
					}
					if(Inc ==2)
					{
						Channel_Reading();
					}
				}
				else if(Page_Erase4== 1)
				{
					if(Inc ==1)
					{
//						if(Keydata=='1')
//						{
//							Squelth_Read_State = 1;
//						}
//						else if(Keydata == '2')
//						{
//							Squelth_Read_State = 0;
//						}
					}
					if(Inc ==2)
					{
						Channel_Reading();
					}
				}
			}
			Keydata=0;
		}
	}
}
void Reset_Password_Reading(void)
{
	if(Admin_Reset_State==0)
	{
		if(Keydata=='*')
		{
			if(Admin_Password_Entry[strlen(Admin_Password_Entry)-1]=='_')
			{
				Index6 = strlen(Admin_Password_Entry);
				if(Index6<0)
					Index6 = 0;
				Admin_Password_Entry[Index6-1]=0;
				Admin_Password_Entry_Mask[strlen(Admin_Password_Entry_Mask)-1] = 0;
				if(Admin_Password_Entry_Mask[0]==0)
						Admin_Password_Entry_Mask[0] = '0';
				Index6 = strlen(Admin_Password_Entry);
				if(Index6!=0)
					Index6--;
				Size = Index6;
				Admin_Password_Entry[Size]='_';
				Keydata=0;
			}
			else
			{
				Index6 = strlen(Admin_Password_Entry);
				if(Index6<0)
					Index6 = 0;
				Admin_Password_Entry[Index6]='_';
				Admin_Password_Entry_Mask[strlen(Admin_Password_Entry_Mask)-1] = 0;
				if(Admin_Password_Entry_Mask[0]==0)
					Admin_Password_Entry_Mask[0] = '0';
				Index6 = strlen(Admin_Password_Entry);
				if(Index6!=0)
					Index6--;
				Keydata=0;
			}
		}
		else
		{
			if(Keydata!='#' && Keydata!=0)
			{
				Index6 = strlen(Admin_Password_Entry)-1;
				if(Index6<0)
				{
					Index6=0;
				}
				if(Index6>=0 && Index6<4)
				{
					Admin_Password_Entry[Index6]=Keydata;
					Admin_Password_Entry_Mask[Index6]='X';
					Admin_Password_Entry[strlen(Admin_Password_Entry)]='_';
					Index6++;
				}
				Keydata =0;
			}
		}
		if(Admin_Reset_State==1)
		{
			Inc=0;
		}
	}
	else if(Admin_Reset_State ==1)
	{
		if(Inc == 0)
		{
			if(Keydata=='*')
			{
				if(Enter_New_Pass[strlen(Enter_New_Pass)-1]=='_')
				{
					Index4 = strlen(Enter_New_Pass);
					if(Index4<0)
						Index4 = 0;
					Enter_New_Pass[Index4-1]=0;
					Enter_New_Pass_Mask[strlen(Enter_New_Pass_Mask)-1] = 0;
					if(Enter_New_Pass_Mask[0]==0)
						Enter_New_Pass_Mask[0]='0';
					Index4 = strlen(Enter_New_Pass);
					if(Index4!=0)
						Index4--;
					Size = Index4;
					Enter_New_Pass[Size]='_';
					Keydata=0;
				}
				else
				{
					Index4 = strlen(Enter_New_Pass);
					if(Index4<0)
						Index4 = 0;
					Enter_New_Pass[Index4]='_';
					Enter_New_Pass_Mask[strlen(Enter_New_Pass_Mask)-1] = 0;
					Index4 = strlen(Enter_New_Pass);
					if(Index4!=0)
						Index4--;
					Keydata=0;
				}
			}
			else
			{
				if(Keydata!='#' && Keydata!=0)
				{
					if(Index4<0)
					{
						Index4=0;
					}
					if(Index4>=0 && Index4<4)
					{
						Enter_New_Pass[Index4]=Keydata;
						Enter_New_Pass_Mask[Index4]='X';
						Enter_New_Pass[strlen(Enter_New_Pass)]='_';
						Index4++;
					}
					Keydata =0;
				}
			}
		}
		else if(Inc == 1)
		{
			if(Keydata=='*')
			{
				if(Re_Enter_Pass[strlen(Re_Enter_Pass)-1]=='_')
				{
					Index5 = strlen(Re_Enter_Pass);
					if(Index5<0)
						Index5 = 0;
					Re_Enter_Pass[Index5-1]=0;
					Re_Enter_Pass_Mask[strlen(Re_Enter_Pass_Mask)-1] = 0;
					if(Re_Enter_Pass_Mask[0]==0)
						Re_Enter_Pass_Mask[0]='0';
					Index5 = strlen(Re_Enter_Pass);
					if(Index5!=0)
						Index5--;
					Size = Index5;
					Re_Enter_Pass[Size]='_';
					Keydata=0;
				}
				else
				{
					Index5 = strlen(Re_Enter_Pass);
					if(Index5<0)
						Index5 = 0;
					Re_Enter_Pass[Index5]='_';
					Re_Enter_Pass_Mask[strlen(Re_Enter_Pass_Mask)-1] = 0;
					if(Re_Enter_Pass_Mask[0]==0)
						Re_Enter_Pass_Mask[0]='0';
					Index5 = strlen(Re_Enter_Pass);
					if(Index5!=0)
						Index5--;
					Keydata=0;
				}
			}
			else
			{
				if(Keydata!='#' && Keydata!=0)
				{
					if(Index5<0)
					{
						Index5=0;
					}
					if(Index5>=0 && Index5<4)
					{
						Re_Enter_Pass[Index5]=Keydata;
						Re_Enter_Pass_Mask[Index5]='X';
						Re_Enter_Pass[strlen(Re_Enter_Pass)]='_';
						Index5++;
					}
					Keydata =0;
				}
			}
		}
	}
}
void Password_Reading(void)
{
	if(PassWord_flag==0 && Inc == 1)
	{
		if(Keydata=='*')
		{
			if(Password[strlen(Password)-1]=='_')
			{
				Index3 = strlen(Password);
				if(Index3<0)
					Index3 = 0;
				Password[Index3-1]=0;
				passwordmask[strlen(passwordmask)-1]=0;
				if(passwordmask[0] == 0)
						passwordmask[0] = '0';
				Index3 = strlen(Password);
				if(Index3!=0)
					Index3--;
				Size = Index3;
				Password[Size]='_';
				Keydata=0;
			}
			else
			{
				if(Index3<0)
					Index3 = 0;
				Password[Index3]='_';
				Index3 = strlen(Password);
				passwordmask[strlen(passwordmask)-1]=0;
				if(passwordmask[0] == 0)
					passwordmask[0] = '0';
				if(Index3!=0)
					Index3--;
				Keydata=0;
			}
		}
		else
		{
			if(Keydata!='#' && Keydata!=0)
			{
				if(Index3<0)
				{
					Index3=0;
				}
				if(Index3>=0 && Index3<4)
				{
					Password[Index3]=Keydata;
					passwordmask[Index3]='X';
					Password[strlen(Password)]='_';
					Index3++;
				}
				Keydata =0;
			}
		}
	}
}
void Keypad_Application(void)
{
	if(Keydata=='a')
	{
		Menu=Menu+1;
		if(Menu<0)
		{
			Menu =0;
		}
		if(Menu>1)
		{
			Menu =0;
			Inc=0;
			PTT_Switch_state=0;
			Prev_Channel_Int =Prev_Channel_Int +1;
			Prev_Rssi_Int = Prev_Rssi_Int+1;
			Prev_Frequency_Int=Prev_Frequency_Int+1;
		}
		Saved_flag0=0,Saved_flag1=0,Saved_flag2=0,Saved_flag3=0;
		Page_Erase0=0,Page_Erase1=0,Page_Erase2=0,Page_Erase3=0,Page_Erase4=0;
		Pass0=0,Pass1=0,Pass2=0;
		Index1=0,Index2=0;
		Enter=0;
		Keydata=0;
		Inc=0;
		Get_Tx_CH_Flag =0;//Reading data before set parameters
		Get_Rx_CH_Flag=0;
		PTT_RX_State=1;
		Manual_FRQ_Entry =0;
		Fill_RAM_12832(0x00);
	}
	else if(Keydata=='c')
	{
		if(Manual_FRQ_Entry == 1)
			Frequency_Step_Inc = 1;
		Inc=Inc-1;
		if(Menu==1 && Inc_State==1)
		{
			if(Inc<=0)
				Inc=1;
		}
		else
		{
			if(Inc<=0)
				Inc=0;
		}
		if(Inc<0)
			Inc=0;
		Keydata=0;
	}
	else if(Keydata=='d')
	{
		Inc=Inc+1;
		if(Manual_FRQ_Entry == 1)
		{
			Frequency_Step_Dnc = 1;
			if(Inc>1)
			{
				Inc =1;
			}
		}
		if(Menu==1 && Page_Erase1 == 1 && Enter == 1)
		{
			if(Inc>2)
			{
				Inc=2;
			}
		}
		else if(Menu==1 && Page_Erase2 == 1 && Enter == 1)
		{
			if(Inc>2)
			{
				Inc=2;
			}
		}
		else if(Menu==1 && Page_Erase3 == 1 && Enter == 1)
		{
			if(Inc>2)
			{
				Inc=2;
			}
		}
		else if(Menu==1 && Page_Erase4 == 1 && Enter == 1)
		{
			if(Inc>2)
			{
				Inc=2;
			}
		}
		else if(Menu==1 && Inc_State==1)
		{
			if(Inc>4)
				Inc=4;
		}
		else if(Menu==1 && Page_Erase1 == 1 && Enter == 3)
		{
			if(Inc>2)
			{
				Inc=2;
			}
		}
		else if(Menu==1 && Page_Erase2 == 1 && Enter == 3)
		{
			if(Inc>2)
			{
				Inc=2;
			}
		}
		else if(Menu==1 && Page_Erase3 == 1 && Enter == 3)
		{
			if(Inc>3)
			{
				Inc=3;
			}
		}
		else if(Menu==1 && Page_Erase4 == 1 && Enter == 3)
		{
			if(Inc>3)
			{
				Inc=3;
			}
		}
		else if(Menu==1 && Page_Erase1 == 1 && Enter == 4 && Back_Page == 1)
		{
			if(Inc>1)
			{
				Inc=1;
			}
		}
		else if(Menu==1 && Page_Erase1 == 1 && Enter == 2 && Password_Reset_Flag==1)
		{
			if(Inc>2)
			{
				Inc=2;
			}
		}
		else if(Menu==1 && Page_Erase2 == 1 && Enter == 2 && Password_Reset_Flag==1)
		{
			if(Inc>2)
			{
				Inc=2;
			}
		}
		else if(Menu==1 && Page_Erase3 == 1 && Enter == 2 && Password_Reset_Flag==1)
		{
			if(Inc>2)
			{
				Inc=2;
			}
		}
		else if(Menu==1 && Page_Erase4 == 1 && Enter == 2 && Password_Reset_Flag==1)
		{
			if(Inc>2)
			{
				Inc=2;
			}
		}
		Keydata=0;
	}
	else if(Keydata=='b')
	{
		Enter = Enter+1;
		if(Enter==1)
		{
			Inc=0;
		}
		if(Enter>5)
		{
			Enter =0;
		}
//		if(Page_Erase3 == 1)
//			Get_Rx_CH_Flag = 0;
		Keydata=0;
		Inc=0;
		Fill_RAM_12832(0x00);
	}
	else if(Keydata=='#')
	{
		memcpy(frequency,Current_Freq,sizeof(Current_Freq));
		if(frequency[0] == 0)
			frequency[0] = '0';
		Manual_FRQ_Entry=Manual_FRQ_Entry+1;
		if(Manual_FRQ_Entry>2)
		{
			Manual_FRQ_Entry = 1;
			Prev_Frequency_Int = 435;
			Prev_Channel_Int = 436;
			Prev_Rssi_Int = 437;
		}
		Inc = 0;
		if(Manual_FRQ_Entry != 2)
			Fill_RAM_12832(0x00);
		Keydata=0;
	}
	if(Manual_FRQ_Entry == 1)
	{
		Manual_FREQ_Reading();
	}
	else if(Manual_FRQ_Entry == 2)
	{
		if(Keydata == '*')
		{
			SQ_T_H_Val[0] = '0';
		}
		else
		{
			if(Keydata != '#' && Keydata!=0)
			{
				if(Keydata == '0' || Keydata == '1' || Keydata == '2' || Keydata == '3' || Keydata == '4')
				{
					SQ_T_H_Val[0] = Keydata;
				}
			}
		}
		Keydata = 0;
	}
	if(Menu==1 && Page_Erase1 == 1 && Enter == 1) //Password Login page
	{
		Password_Reading();
	}
	if(Menu==1 && Page_Erase2 == 1 && Enter == 1) //Password Login page
	{
		Password_Reading();
	}
	if(Menu==1 && Page_Erase3 == 1 && Enter == 1) //Password Login page
	{
		Password_Reading();
	}
	if(Menu==1 && Page_Erase4 == 1 && Enter == 1) //Password Login page
	{
		Password_Reading();
	}
	if(Menu==1 && Page_Erase1 == 1 && Enter == 2 && Password_Reset_Flag==1)
	{
		Reset_Password_Reading();
	}
	if(Menu==1 && Page_Erase2 == 1 && Enter == 2 && Password_Reset_Flag==1)
	{
		Reset_Password_Reading();
	}
	if(Menu==1 && Page_Erase3 == 1 && Enter == 2 && Password_Reset_Flag==1)
	{
		Reset_Password_Reading();
	}
	if(Menu==1 && Page_Erase4 == 1 && Enter == 2 && Password_Reset_Flag==1)
	{
		Reset_Password_Reading();
	}
	if(Menu==1 && Page_Erase1 == 1 && Enter == 3)
	{
		Freq_Channel_Reading();
	}
	if(Menu==1 && Page_Erase2 == 1 && Enter == 3)
	{
		Freq_Channel_Reading();
	}
	if(Menu==1 && Page_Erase3 == 1 && Enter == 3)
	{
		if(Inc != 1)
		{
			Freq_Channel_Reading();
		}
		else if(Inc == 1)
		{
			if(Keydata == '*')
			{
				SQ_T_H_Val[0] = '0';
			}
			else
			{
				if(Keydata != '#' && Keydata!=0)
				{
					if(Keydata == '0' || Keydata == '1' || Keydata == '2' || Keydata == '3' || Keydata == '4')
					{
						SQ_T_H_Val[0] = Keydata;
					}
				}
			}
			Keydata = 0;
		}
	}
	if(Menu==1 && Page_Erase4 == 1 && Enter == 3 )
	{
		if(Inc != 1)
		{
			Freq_Channel_Reading();
		}
		else if(Inc == 1)
		{
			if(Keydata == '*')
			{
				SQ_T_H_Val[0] = '0';
			}
			else
			{
				if(Keydata != '#' && Keydata!=0)
				{
					if(Keydata == '0' || Keydata == '1' || Keydata == '2' || Keydata == '3' || Keydata == '4')
					{
						SQ_T_H_Val[0] = Keydata;
					}
				}
			}
			Keydata = 0;
		}
	}
}
char Read_keypad(void)
{
	HAL_GPIO_WritePin (Keypad_C0_GPIO_Port, Keypad_C0_Pin, GPIO_PIN_SET);         //Pull the c0 high
	HAL_GPIO_WritePin (Keypad_C0_GPIO_Port, Keypad_C1_Pin, GPIO_PIN_RESET);     // Pull the c1 low
	HAL_GPIO_WritePin (Keypad_C0_GPIO_Port, Keypad_C2_Pin, GPIO_PIN_RESET);    // Pull the c2 low
	HAL_GPIO_WritePin (Keypad_C0_GPIO_Port, Keypad_C3_Pin, GPIO_PIN_RESET);    // Pull the c3 low
	if(HAL_GPIO_ReadPin(Keypad_R0_GPIO_Port, Keypad_R0_Pin))
	{
		return '1';
	}
	else if(HAL_GPIO_ReadPin(Keypad_R0_GPIO_Port, Keypad_R1_Pin))
	{
		return '4';
	}
	else if(HAL_GPIO_ReadPin(Keypad_R0_GPIO_Port, Keypad_R2_Pin))
	{
		return '7';
	}
	else if(HAL_GPIO_ReadPin(Keypad_R0_GPIO_Port, Keypad_R3_Pin))
	{
		return '*';
	}
	HAL_GPIO_WritePin (Keypad_C0_GPIO_Port, Keypad_C0_Pin, GPIO_PIN_RESET);         //Pull the c0 high
	HAL_GPIO_WritePin (Keypad_C0_GPIO_Port, Keypad_C1_Pin, GPIO_PIN_SET);     // Pull the c1 low
	HAL_GPIO_WritePin (Keypad_C0_GPIO_Port, Keypad_C2_Pin, GPIO_PIN_RESET);    // Pull the c2 low
	HAL_GPIO_WritePin (Keypad_C0_GPIO_Port, Keypad_C3_Pin, GPIO_PIN_RESET);    // Pull the c3 low
	if(HAL_GPIO_ReadPin(Keypad_R0_GPIO_Port, Keypad_R0_Pin))
	{
		return '2';
	}
	else if(HAL_GPIO_ReadPin(Keypad_R0_GPIO_Port, Keypad_R1_Pin))
	{
		return '5';
	}
	else if(HAL_GPIO_ReadPin(Keypad_R0_GPIO_Port, Keypad_R2_Pin))
	{
		return '8';
	}
	else if(HAL_GPIO_ReadPin(Keypad_R0_GPIO_Port, Keypad_R3_Pin))
	{
		return '0';
	}
	HAL_GPIO_WritePin (Keypad_C0_GPIO_Port, Keypad_C0_Pin, GPIO_PIN_RESET);         //Pull the c0 high
	HAL_GPIO_WritePin (Keypad_C0_GPIO_Port, Keypad_C1_Pin, GPIO_PIN_RESET);     // Pull the c1 low
	HAL_GPIO_WritePin (Keypad_C0_GPIO_Port, Keypad_C2_Pin, GPIO_PIN_SET);    // Pull the c2 low
	HAL_GPIO_WritePin (Keypad_C0_GPIO_Port, Keypad_C3_Pin, GPIO_PIN_RESET);    // Pull the c3 low
	if(HAL_GPIO_ReadPin(Keypad_R0_GPIO_Port, Keypad_R0_Pin))
	{
		return '3';
	}
	else if(HAL_GPIO_ReadPin(Keypad_R0_GPIO_Port, Keypad_R1_Pin))
	{
		return '6';
	}
	else if(HAL_GPIO_ReadPin(Keypad_R0_GPIO_Port, Keypad_R2_Pin))
	{
		return '9';
	}
	else if(HAL_GPIO_ReadPin(Keypad_R0_GPIO_Port, Keypad_R3_Pin))
	{
		return '#';
	}
	HAL_GPIO_WritePin (Keypad_C0_GPIO_Port, Keypad_C0_Pin, GPIO_PIN_RESET);         //Pull the c0 high
	HAL_GPIO_WritePin (Keypad_C0_GPIO_Port, Keypad_C1_Pin, GPIO_PIN_RESET);     // Pull the c1 low
	HAL_GPIO_WritePin (Keypad_C0_GPIO_Port, Keypad_C2_Pin, GPIO_PIN_RESET);    // Pull the c2 low
	HAL_GPIO_WritePin (Keypad_C0_GPIO_Port, Keypad_C3_Pin, GPIO_PIN_SET);    // Pull the c3 low
	if(HAL_GPIO_ReadPin(Keypad_R0_GPIO_Port, Keypad_R0_Pin))
	{
		return 'a';
	}
	else if(HAL_GPIO_ReadPin(Keypad_R0_GPIO_Port, Keypad_R1_Pin))
	{
		return 'b';
	}
	else if(HAL_GPIO_ReadPin(Keypad_R0_GPIO_Port, Keypad_R2_Pin))
	{
		return 'c';
	}
	else if(HAL_GPIO_ReadPin(Keypad_R0_GPIO_Port, Keypad_R3_Pin))
	{
		return 'd';
	}
	else
		return 0;
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1)
	{
	}
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
	/* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
