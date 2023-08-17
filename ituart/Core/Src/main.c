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
#include "fatfs.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "string.h"
#include <stdbool.h>
#include <stdio.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
char time[30];
char date[30];
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
RTC_HandleTypeDef hrtc;

SPI_HandleTypeDef hspi2;

UART_HandleTypeDef huart5;

/* USER CODE BEGIN PV */
RTC_TimeTypeDef sTime;
RTC_DateTypeDef sDate;
char combinedTime[50];
char combinedDate[50];
uint8_t rx_data[1];
char UART_BUFFER[512];
char a[512];
uint8_t flag = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_UART5_Init(void);
static void MX_SPI2_Init(void);
static void MX_RTC_Init(void);
/* USER CODE BEGIN PFP */
void HAL_UART_RxCpltCallback (UART_HandleTypeDef * );

void UartBufferReset(){
	for(int i=0; i<sizeof(UART_BUFFER); i++){
		UART_BUFFER[i]='\0';
				a[i] = '\0';
	}
}

void send_uart(char *data){
	data[strlen(data)+1]='\n';
	 HAL_UART_Transmit_IT(&huart5, data, strlen(data));
}
void process_SD_card( void );
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_UART5_Init();
  MX_SPI2_Init();
  MX_FATFS_Init();
  MX_RTC_Init();
  /* USER CODE BEGIN 2 */
HAL_UART_Transmit(&huart5, "Hello!", 6, 100);
HAL_UART_Transmit(&huart5, "\n", 1, 100);

HAL_UART_Receive_IT(&huart5, rx_data, 1);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
	  HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);

	  sprintf((char*)time,"%02d:%02d:%02d",sTime.Hours, sTime.Minutes, sTime.Seconds);
	  sprintf((char*)date,"%02d-%02d-%2d",sDate.Date, sDate.Month, 2000 + sDate.Year);

	  //HAL_UART_Transmit(&huart5, (uint8_t *) date, strlen(date), 300);
	  //HAL_UART_Transmit(&huart5, (uint8_t *) time, strlen(time), 300);
	  sprintf(combinedTime, "%s\n", time);
	  sprintf(combinedDate, "%s\n", date);

	  if(flag){
		  if(strcmp(UART_BUFFER, "green_on\r\n") == 0){
			  if(HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_12)){
			  send_uart("yesil led zaten aktif\n");
			  strcpy(a,"yesil led zaten aktif\n") ;
			  process_SD_card();

			  }else{
			  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_SET);
			  send_uart("yesil led aktif edildi\n");
			  strcpy(a,"yesil led aktif edildi\n") ;
			  process_SD_card();
			  }
			  UartBufferReset();
			  flag=0;

		  }
		  if(strcmp(UART_BUFFER, "green_off\r\n") == 0){
			 if(HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_12) == 0){
			 send_uart("yesil led zaten kapali\n");
			 strcpy(a,"yesil led zaten kapali\n") ;
			 process_SD_card();
			 }
			 else{
			 HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_RESET);
			 send_uart("yesil led kapatildi\n");
			 strcpy(a,"yesil led kapatildi\n") ;
			 process_SD_card();
			 }
			 UartBufferReset();
			 flag=0;
		  }
		  if(strcmp(UART_BUFFER, "red_on\r\n") == 0){
		  	 if(HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_14)){
		  	 send_uart("kirmizi led zaten aktif\n");
		  	 strcpy(a,"kirmizi led zaten aktif\n") ;
		  	 process_SD_card();
   		     }else{
		  	 HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_SET);
		  	 send_uart("kirmizi led aktif edildi\n");
		  	 strcpy(a,"kirmizi led aktif edildi\n") ;
		  	 process_SD_card();
   		     }
		  	 UartBufferReset();
		  	 flag=0;
		     }
		   if(strcmp(UART_BUFFER, "red_off\r\n") == 0){
		     if(HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_14) == 0){
     		 send_uart("kirmizi led zaten kapali\n");
		  	 strcpy(a,"kirmizi led zaten kapali\n") ;
		  	 process_SD_card();
		  	 }
    		 else{
		  	 HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_RESET);
	  		 send_uart("kirmizi led kapatildi\n");
		  	 strcpy(a,"kirmizi led kapatildi\n") ;
		  	 process_SD_card();
  			 }
		  	 UartBufferReset();
		  	 flag=0;
		  	 }
		  if(strcmp(UART_BUFFER, "blue_on\r\n") == 0){
		  	 if(HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_15)){
		  	 send_uart("mavi led zaten aktif\n");
		  	 strcpy(a,"mavi led zaten aktif\n") ;
		  	 process_SD_card();
		  	 }else{
		     HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_SET);
    		 send_uart("mavi led aktif edildi\n");
		  	 strcpy(a,"mavi led aktif edildi\n") ;
		  	 process_SD_card();
		  	 }
        	 UartBufferReset();
		  	 flag=0;
		  	 }
		   if(strcmp(UART_BUFFER, "blue_off\r\n") == 0){
		     if(HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_15) == 0){
			 send_uart("mavi led zaten kapali\n");
		  	 strcpy(a,"mavi led zaten kapali\n") ;
		  	 process_SD_card();
		 	 }
			 else{
		  	 HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_RESET);
		  	 send_uart("mavi led kapatildi\n");
		  	 strcpy(a,"mavi led kapatildi\n") ;
		  	 process_SD_card();
	     	 }
		     UartBufferReset();
		  	 flag=0;
		     }
		  if(strcmp(UART_BUFFER, "orange_on\r\n") == 0){
		     if(HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_13)){
		 	 send_uart("turuncu led zaten aktif\n");
		  	 strcpy(a,"turuncu led zaten aktif\n") ;
		  	 process_SD_card();
		     }else{
		     HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_SET);
		     send_uart("turuncu led aktif edildi\n");
		  	 strcpy(a,"turuncu led aktif edildi\n") ;
		  	 process_SD_card();
			 }
		 	 UartBufferReset();
		     flag=0;
	         }
		  if(strcmp(UART_BUFFER, "orange_off\r\n") == 0){
		  	 if(HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_13) == 0){
		  	 send_uart("turuncu led zaten kapali\n");
		  	 strcpy(a,"turuncu led zaten kapali\n") ;
		  	 process_SD_card();
		  	 }
		  	 else{
		  	 HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_RESET);
		  	 send_uart("turuncu led kapatildi\n");
		  	 strcpy(a,"turuncu led kapatildi\n") ;
		  	 process_SD_card();
		     }
		  	 UartBufferReset();
		  	 flag=0;
		     }
		  	 else{
		  		UartBufferReset();
		  		flag=0;
		  	 }



	 }



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

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE|RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief RTC Initialization Function
  * @param None
  * @retval None
  */
static void MX_RTC_Init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */

  /* USER CODE END RTC_Init 0 */

  RTC_TimeTypeDef sTime = {0};
  RTC_DateTypeDef sDate = {0};

  /* USER CODE BEGIN RTC_Init 1 */

  /* USER CODE END RTC_Init 1 */

  /** Initialize RTC Only
  */
  hrtc.Instance = RTC;
  hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
  hrtc.Init.AsynchPrediv = 127;
  hrtc.Init.SynchPrediv = 255;
  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }

  /* USER CODE BEGIN Check_RTC_BKUP */

  /* USER CODE END Check_RTC_BKUP */

  /** Initialize RTC and set the Time and Date
  */
  sTime.Hours = 0x9;
  sTime.Minutes = 0x54;
  sTime.Seconds = 0x0;
  sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sTime.StoreOperation = RTC_STOREOPERATION_RESET;
  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
  sDate.WeekDay = RTC_WEEKDAY_THURSDAY;
  sDate.Month = RTC_MONTH_AUGUST;
  sDate.Date = 0x17;
  sDate.Year = 0x23;

  if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RTC_Init 2 */

  /* USER CODE END RTC_Init 2 */

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
  hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
  hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi2.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI2_Init 2 */

  /* USER CODE END SPI2_Init 2 */

}

/**
  * @brief UART5 Initialization Function
  * @param None
  * @retval None
  */
static void MX_UART5_Init(void)
{

  /* USER CODE BEGIN UART5_Init 0 */

  /* USER CODE END UART5_Init 0 */

  /* USER CODE BEGIN UART5_Init 1 */

  /* USER CODE END UART5_Init 1 */
  huart5.Instance = UART5;
  huart5.Init.BaudRate = 115200;
  huart5.Init.WordLength = UART_WORDLENGTH_8B;
  huart5.Init.StopBits = UART_STOPBITS_1;
  huart5.Init.Parity = UART_PARITY_NONE;
  huart5.Init.Mode = UART_MODE_TX_RX;
  huart5.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart5.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart5) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN UART5_Init 2 */

  /* USER CODE END UART5_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15, GPIO_PIN_RESET);

  /*Configure GPIO pin : PB12 */
  GPIO_InitStruct.Pin = GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : PD12 PD13 PD14 PD15 */
  GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
void HAL_UART_RxCpltCallback (UART_HandleTypeDef * huart)
{
	if(huart -> Instance == UART5)
	{
		UART_BUFFER[strlen(UART_BUFFER)] = rx_data[0];
		HAL_UART_Receive_IT(&huart5, rx_data, 1);
		if(strstr((char*)UART_BUFFER, "\r\n") !=NULL){
			flag = 1;
		}

/*
		if(index == (2) && (data_array[index - 1] == '\n'))
		{
			HAL_GPIO_TogglePin(GPIOD,( GPIO_PIN_12 << ((data_array[0] -49) & 0x0F)));


		HAL_UART_Transmit(&huart5, "OK", 2, 100);
		HAL_UART_Transmit(&huart5, "\n", 1, 100);
		index = 0;
	}
	else if (index > (2) )
	{
		HAL_UART_Transmit(&huart5, "Error", 5, 100);
		HAL_UART_Transmit(&huart5, "\n", 1, 100);
		index = 0;
	}
	*/
	HAL_UART_Receive_IT(&huart5, rx_data,1);
	}
}

#ifdef __GNUC__
  /* With GCC, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
int __io_putchar(int ch);
#else
int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

void process_SD_card(void)
{
  FATFS       FatFs;                //Fatfs handle
  FIL         fil;                  //File handle
  FRESULT     fres;                 //Result after operations
  char        buf[100];
  int x = 0;
  do
  {
    //Mount the SD Card
    fres = f_mount(&FatFs, "", 1);    //1=mount now
    if (fres != FR_OK)
    {
      //printf("No SD Card found : (%i)\r\n", fres);
      break;
    }


    //Open the file
    fres = f_open(&fil, "log.txt", FA_WRITE | FA_READ | FA_OPEN_APPEND );
    if(fres != FR_OK)
    {
      //printf("File creation/open Error : (%i)\r\n", fres);
      break;
    }

    //printf("Writing data!!!\r\n");
    //write the data
    f_puts(a, &fil);
    f_puts(date, &fil);
    f_puts(time, &fil);
    //close your file
    f_close(&fil);

    //Open the file
  /*  fres = f_open(&fil, "123.txt", FA_READ);
    if(fres != FR_OK)
    {
      //printf("File opening Error : (%i)\r\n", fres);
      break;
    }

    //read the data
    f_gets(buf, sizeof(buf), &fil);

    //printf("Read Data : %s\n", buf);

    //close your file
    f_close(&fil);
    //printf("Closing File!!!\r\n"); */
#if 0
    //Delete the file.
    fres = f_unlink(EmbeTronicX.txt);
    if (fres != FR_OK)
    {
      printf("Cannot able to delete the file\n");
    }
#endif
  } while( false );

  //We're done, so de-mount the drive
  //f_mount(NULL, "", 0);
  //printf("SD Card Unmounted Successfully!!!\r\n");
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
