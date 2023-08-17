# STM32F407 UART and SPI SD Card Project

This project demonstrates UART communication and SPI SD card interfacing using the STM32F407 microcontroller.

## Introduction

This project showcases how to communicate via UART and interface with an SD card using the SPI protocol on the STM32F407 microcontroller.

## Getting Started

Follow these steps to set up and run the project on your STM32F407 development board:

1. Clone the project repository: `git clone https://github.com/dundar98/stm32f407-uart-spi-sdcard.git`
2. Open the project in your preferred STM32 development environment (e.g., STM32CubeIDE).
3. Configure the project settings for your specific microcontroller and debugging interface.
4. Build the project and upload it to the STM32F407 microcontroller.

## Requirements

- STM32CubeIDE or similar development environment
- STM32F407 Discovery or compatible development board
- UART terminal software for communication
- SD card module compatible with SPI

## Pin Configuration

- Green LED: GPIO Pin PD12
- Red LED: GPIO Pin PD14
- Blue LED: GPIO Pin PD15
- Orange LED: GPIO Pin PD13
- UART5 TX: GPIO Pin PC12
- UART5 RX: GPIO Pin PD2
- SPI2 SCK: GPIO Pin PB13
- SPI2 MISO: GPIO Pin PB14
- SPI2 MOSI: GPIO Pin PB15
- SPI2 CS: GPIO Pin PB12
- SD Card CS: GPIO Pin (Adjust According to Your Wiring)

## Usage

1. Connect the STM32F407 microcontroller to your computer.
2. Open a UART terminal (e.g., PuTTY, Tera Term) and establish a connection to the microcontroller using the appropriate baud rate and settings.
3. The UART terminal will display the project's startup message.
4. You can send commands via the terminal to control the LED outputs and interact with the SD card:
   - "led_on": Turns on the LED.
   - "led_off": Turns off the LED.
   - ...

