# STM32_CANbus_Multiple_MCUs
 
Three MCUs were used in this project. F446 Nucleo Board sends temperature and humidity value to F407 Discovery Board thanks to DHT11 sensor and F103 Nucleo Board sends distance value to F407 Discovery Board through HC SR04 sensor. Finally, F407 writes incoming data on LCD

CAN communication is provided with MCP2551 transceiver module

MCP2551 Datasheet : http://ww1.microchip.com/downloads/en/devicedoc/21667e.pdf

F103 Connection
---------------

PB8 => CAN_RX
PB9 => CAN_TX

PA15 => TIM2_CH1 for Echo Pin
PA1  => GPIO_Output for Trigger Pin

Input Capture Mode is used for calculate Echo signal length

F446 Connection
---------------

PB8 => CAN_RX
PB9 => CAN_TX

PA10 => GPIO_Output for DHT11 Signal Pin
Timer 6 is enable for delay in microsecond

F407 Connection
---------------

PB8 => CAN_RX
PB9 => CAN_TX

PB6 => I2C1 SCL
PB7 => I2C1 SDA

I2C communication is used for LCD!

[CANBUS DIAGRAM](https://user-images.githubusercontent.com/65983840/142001002-34ecfe2c-73dc-4a6a-b1a4-7017b0820e98.jpg)

