# LAB1: LED Animations
This session contains guides about starting a STM32 MCU project on STMicroelectronis applications and Proteus simulation.  
# How to use
## Get this repo
Clone this branch into your desired directory using `git clone` command:
```
git clone -b LAB1 https://github.com/Huy-pixel/CO3009-Laboratory.git
```
This command will download branch name LAB1 into your desired directory by using `cd` command.
## How to run exercises
All exercises of this LAB is integrated into one main source file `main.c`. Macros for running exercises located in header sections.<br>
All exercises must be run sequentially, so user must uncomment the desired exercise for testing and comment out all the others.
# Report
## Exercise 1
This exercise requires making 2 LEDs toggle every 2 seconds, red and yellow in sequence. Those are shown in figure below:
<img width="1441" height="670" alt="image" src="https://github.com/user-attachments/assets/12ccb81d-9f33-4b24-9efc-5e02f0cf6d74" />
The source code in `while` loop is:
```
uint8_t counter = 0;
while(1)
{
  HAL_GPIO_WritePin(YELLOW_x_GPIO_Port, YELLOW_x_Pin, counter >> 1);
  HAL_GPIO_WritePin(RED_x_GPIO_Port, RED_x_Pin, !(counter >> 1));

  counter = (counter + 1) % 4;
}
```
Counter is vary from 0 to 3, which is in binary is {00, 01, 10, 11}. Then get the LSB of each number, it is, two first seconds has LSB of 0, the rest has 1. Those are used as state of GPIO pin.
## Exercise 2
