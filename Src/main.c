/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Auto-generated by STM32CubeIDE
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

/********************APPLICATION********************************'/
 * THIS CODE IMPLEMENTS AN INTERRUPT TRIGGER USING A GPIOB 0 AS INPUT
 * ******************STEPS TO SETUP THE GPIO AS TRIGER FOR THE INTERRUPTION********************
 * 1 The button is connected to a GPIO pin of the MCU that is supported by the external interrupts EXTI, in this case PIN B0
 * 2 The GPIO pin should be configured as input mode
 * 3 Setting SYSCFGEN to 1 in order to be able to write the configurations of EXTI in syscfg
 * 4 Create the link between a GPIO port and the relevant EXTI must be established using SYSCFG_EXTICRx register
 * 5 Setting the interrupt for the EXTI writing to the interrupt mask register
 * 6 Configure the trigger detection for relevant EXTI line
 * 7 Enable the NVIC IRQ with the register specified by the processor manufacturer, in this case is the Cortex m4 and the register is NVIC_ISER0 - NVIC_ISER7 E000E100
 * 8 Implement the handler to service the interrupt
 */

/*******************GENERAL STEPS TO SETUP THE NECESSARY********************'/
* 1 Configure RCC registers for AHB2 AND APB2
*
*/

#include <stdint.h>

#define RCC_BASE_ADDR 0x40021000UL
#define AHB2_EN_OFFSET 0x4CUL
#define AHB2_EN_RC_ADDR (RCC_BASE_ADDR + AHB2_EN_OFFSET)

#define APB2_EN_OFFSET 0x60UL
#define APB2_EN_ADDR (RCC_BASE_ADDR + APB2_EN_OFFSET)

#define RCC_CFG_REG_OFFSET 0x08UL
#define RCC_CFG_REG_ADDR (RCC_BASE_ADDR + RCC_CFG_REG_OFFSET)

#define GPIOB_BASE_ADDR 0x48000400UL
#define GPIO_MOD_REG_OFFSET 0x00UL
#define GPIOB_MOD_REG_ADDR (GPIOB_BASE_ADDR + GPIO_MOD_REG_OFFSET)

#define GPIO_PORT_INTPUT_DATA_REG_OFFSET 0x10
#define GPIOB_PORT_INTPUT_DATA_REG_OFFSET (GPIOB_BASE_ADDR + GPIO_PORT_INTPUT_DATA_REG_OFFSET)

#define SYSCFG_BASE_ADDR 0x40010000UL
#define SYSCFG_EXTICR1_OFFSET 0x08UL
#define SYSCFG_EXTICR1_ADDR (SYSCFG_BASE_ADDR + SYSCFG_EXTICR1_OFFSET)

#define EXTI_BASE_ADDR 0x40010400UL
#define INT_MASK_REG_OFFSET 0x00UL
#define INT_MASK_REG_ADDR (EXTI_BASE_ADDR + INT_MASK_REG_OFFSET)

#define RISING_TRIGGER_OFFSET 0x08UL
#define RISING_TRIGGER_ADDR (EXTI_BASE_ADDR + RISING_TRIGGER_OFFSET)

#define NVICIRQEnReg 0xE000E100UL

#define PENDING_REG_OFFSET 0x14
#define PENDING_REG_ADDR (EXTI_BASE_ADDR + PENDING_REG_OFFSET)


/****TO SEE CLOCK CYCLE IN THE LOGIC ANALYZER*********/
#define GPIOA_BASE_ADDR 0x48000000UL
#define GPIO_MOD_REG_ADDR (GPIOA_BASE_ADDR + GPIO_MOD_REG_OFFSET)

#define GPIO_ALTFUNCHIGH_MOD_REG_OFFSET 0x24UL
/*****************************************************/

#define GPIO_PORT_BIT_SET_RESET_OFFSET 0x18UL
#define GPIOA_PORT_BIT_SET_RESET_OFFSET (GPIOA_BASE_ADDR + GPIO_PORT_BIT_SET_RESET_OFFSET)

void Analyzer_clock_init(void);
void LED_blink_init(void);
void Button_init(void);
void funcion_feature1(void);
void funcion2_feature1(void);
void funcion3_feature1(void);
void funcion4_feature1(void);
void funcion5_feature1(void);
/*******Global Variables******/
// SET LD2
uint32_t *pGpioaSetResetReg = (uint32_t*)GPIOA_PORT_BIT_SET_RESET_OFFSET;

uint32_t *pGpioModeReg = (uint32_t*)GPIO_MOD_REG_ADDR;

int main(void)
{

	Analyzer_clock_init();
	LED_blink_init();
	Button_init();

}



void Analyzer_clock_init(void)
{

	/*creating a pointer pointing to the RCC CFG REGISTER*/
	uint32_t *pRccCfgrReg = (uint32_t*) RCC_CFG_REG_ADDR; //

	//1. Configure the clock configuration register (RCC_CFGR) MCOSEL to select the HSI as clock source and prescaler as 2
	*pRccCfgrReg &= ~(15 << 24); //clearing the bit fields for MCOSEL
	*pRccCfgrReg |= (35 << 24); //writing the configuration for HSI that is 0011 and the prescaler divided by 4 that is 010

	/*creating a pointer pointing to the AHB2 ENABLE REGISTER*/
	uint32_t *pAhb2EnReg = (uint32_t*) AHB2_EN_RC_ADDR;

	//2. Configure PA8 to AF0 mode to behave as MCO signal

	//a ) Enable the peripheral clock for GPIOA peripheral
	*pAhb2EnReg |= (1 << 0);

	//b ) Configure the mode of GPIOA pin 8 as alternate function mode

	*pGpioModeReg &= ~(3 << 16); //clearing the bit fields for MODE8
	*pGpioModeReg |= (2 << 16); //writing the configuration for Alternate mode in mode 8

	//c ) Configure the alternation function high register to set the mode 0 for PA8
	uint32_t *pGpioAltModeReg = (uint32_t*)(GPIOA_BASE_ADDR + GPIO_ALTFUNCHIGH_MOD_REG_OFFSET);
	*pGpioAltModeReg &= ~(15 << 0); //clearing the bit fields for AFSEL8 AND writing the configuration for AFSEL8


	//e ) Turn on HSI peripheral clock
	uint32_t *pRCC_CR = (uint32_t*)(RCC_BASE_ADDR + 0x00); //Clock control register (RCC_CR) = 0x00
	*pRCC_CR |= (1U << 8);

}

void LED_blink_init(void)
{
	// Configure the mode of GPIOA pin 5 as output

	*pGpioModeReg &= ~(3 << 10); //clearing the bit fields for MODE8
	*pGpioModeReg |= (1 << 10); //writing the configuration for output

	// TURNING ON LD2
	*pGpioaSetResetReg |= (1 << 5);
}

void EXTI0_IRQHandler(void)
{
	// Configure the mode of GPIOA pin 5 as output
	uint32_t *pGpioModeReg = (uint32_t*)GPIO_MOD_REG_ADDR;
	*pGpioModeReg &= ~(3 << 10); //clearing the bit fields for PIN A5
	*pGpioModeReg |= (1 << 10); //writing the configuration for PIN A5

	*pGpioaSetResetReg |= (0 << 5);
	*pGpioaSetResetReg |= (1 << 21);

	// clearing pending register
	uint32_t *pPendingReg = (uint32_t*)PENDING_REG_ADDR;
	*pPendingReg |= (1 << 0);
}

void Button_init(void)
{

	/*creating a pointer pointing to the AHB2 ENABLE REGISTER*/
	uint32_t volatile *pAhb2EnReg = (uint32_t*) AHB2_EN_RC_ADDR;

	//a ) Enable the peripheral clock for GPIOB peripheral
	*pAhb2EnReg |= (1 << 1);

	/*creating a pointer pointing to the GPIOB MODE REGISTER */
	uint32_t volatile *pGpiobModeReg = (uint32_t*) GPIOB_MOD_REG_ADDR;

	/*Setting Input mode to pin B0 */
	*pGpiobModeReg &= ~(3 << 0);

	/*creating a pointer pointing to the APB2 ENABLE REGISTER*/
	uint32_t volatile *pApb2EnReg = (uint32_t*) APB2_EN_ADDR;

	/*Setting SYSCFGEN to 1 in order to be able to write the configurations of syscfg*/
	*pApb2EnReg |= (1 << 0);

	/*creating a pointer pointing to the SYSCFG external interrupt configuration register 1 */
	uint32_t volatile *pSysCfgReg = (uint32_t*) SYSCFG_EXTICR1_ADDR;

	/*Clearing EXTI0 */
	*pSysCfgReg &= ~(15 << 0);
	/*Setting EXTI0 to use PB0 as output */
	*pSysCfgReg |= (1 << 0);

	/*creating a pointer pointing to the Interrupt mask register 1 (EXTI_IMR1) */
	uint32_t volatile *pIntMaskReg = (uint32_t*) INT_MASK_REG_ADDR;

	/*Setting interrupt for EXT10*/
	*pIntMaskReg |= (1 << 0);

	/*creating a pointer pointing to the Rising trigger selection register 1 (EXTI_RTSR1) */
	uint32_t volatile *pRisingTriggerReg = (uint32_t*) RISING_TRIGGER_ADDR;
	/*Setting interrupt for EXT10*/
	*pRisingTriggerReg |= (1 << 0);

	/*NVIC irq enable */
	uint32_t volatile *pNVICEnReg= (uint32_t*) NVICIRQEnReg;
	*pNVICEnReg |= (1 << 6);

}

void funcion_feature1(void)
{

}

void funcion2_feature1(void)
{

}

void funcion3_feature1(void)
{

}

void funcion4_feature1(void)
{


}
void funcion5_feature1(void)
{

}
