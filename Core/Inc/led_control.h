/**
 * @file       led_control.h
 * @date       2025-08-19
 * @author     Nam Dang
 *
 * @brief      LED control interface for STM32 boards.
 *
 * This header file provides macro definitions to control the user LEDs
 * (turn ON/OFF) on supported STM32 development boards such as
 * NUCLEO-H7A3ZI-Q and STM32F407 Discovery.
 *
 * The file abstracts LED GPIO operations into simple macros so that
 * application code can toggle LEDs without directly dealing with
 * GPIO registers or HAL function calls.
 *
 * @attention  Make sure GPIOs for LEDs are initialized before using
 *             these macros (e.g., using HAL_GPIO_Init).
 */


/* Define to prevent recursive inclusion ------------------------------ */
#ifndef __LED_CONTROL_H
#define __LED_CONTROL_H

/* Includes ----------------------------------------------------------- */

/* Public defines ----------------------------------------------------- */
#define BOARD_NUCLEO_H7A3ZI_Q        // Use this if working with Nucleo H7A3ZI-Q board
//#define BOARD_STM32F407_DISCOVERY      // Use this if working with STM32F407 Discovery board
//==============================================================
// Configure LEDs and Button depending on the selected board
//==============================================================
#ifdef BOARD_NUCLEO_H7A3ZI_Q

    // --- NUCLEO H7A3ZI-Q board pin mapping ---
    #define LED1_GPIO_PORT         GPIOB       // LED1 is connected to GPIOB
    #define LED1_PIN               GPIO_PIN_7  // LED1 uses pin PB7
    #define LED2_GPIO_PORT         GPIOE       // LED2 is connected to GPIOE
    #define LED2_PIN               GPIO_PIN_1  // LED2 uses pin PE1
    #define LED3_GPIO_PORT         GPIOB       // LED3 is connected to GPIOB
    #define LED3_PIN               GPIO_PIN_14 // LED3 uses pin PB14

    #define B1_BUTTON_GPIO_PORT    GPIOC       // User Button is connected to GPIOC
    #define B1_BUTTON_PIN          GPIO_PIN_13 // Button uses pin PC13

#elif defined BOARD_STM32F407_DISCOVERY

    // --- STM32F407 Discovery board pin mapping ---
    #define LED1_GPIO_PORT         GPIOD       // LED1 is connected to GPIOD
    #define LED1_PIN               GPIO_PIN_12 // LED1 uses pin PD12
    #define LED2_GPIO_PORT         GPIOD       // LED2 is connected to GPIOD
    #define LED2_PIN               GPIO_PIN_13 // LED2 uses pin PD13
    #define LED3_GPIO_PORT         GPIOD       // LED3 is connected to GPIOD
    #define LED3_PIN               GPIO_PIN_14 // LED3 uses pin PD14
	#define LED4_GPIO_PORT         GPIOD       // LED3 is connected to GPIOD
    #define LED4_PIN               GPIO_PIN_15 // LED3 uses pin PD15

    #define B1_BUTTON_GPIO_PORT    GPIOA       // User Button is connected to GPIOA
    #define B1_BUTTON_PIN          GPIO_PIN_0 // Button uses pin PA0

#endif
/* Public enumerate/structure ----------------------------------------- */

/* Public macros -------------------------------------------------/**
 * @brief  Turn LED1 ON.
 *
 * @attention  This macro sets the corresponding GPIO pin of LED1 to HIGH.
 */
#define LED1_H()    HAL_GPIO_WritePin(LED1_GPIO_PORT, LED1_PIN, GPIO_PIN_SET)

/**
 * @brief  Turn LED2 ON.
 *
 * @attention  This macro sets the corresponding GPIO pin of LED2 to HIGH.
 */
#define LED2_H()    HAL_GPIO_WritePin(LED2_GPIO_PORT, LED2_PIN, GPIO_PIN_SET)

/**
 * @brief  Turn LED3 ON.
 *
 * @attention  This macro sets the corresponding GPIO pin of LED3 to HIGH.
 */
#define LED3_H()    HAL_GPIO_WritePin(LED3_GPIO_PORT, LED3_PIN, GPIO_PIN_SET)

/**
 * @brief  Turn LED1 OFF.
 *
 * @attention  This macro resets the corresponding GPIO pin of LED1 to LOW.
 */
#define LED1_L()    HAL_GPIO_WritePin(LED1_GPIO_PORT, LED1_PIN, GPIO_PIN_RESET)

/**
 * @brief  Turn LED2 OFF.
 *
 * @attention  This macro resets the corresponding GPIO pin of LED2 to LOW.
 */
#define LED2_L()    HAL_GPIO_WritePin(LED2_GPIO_PORT, LED2_PIN, GPIO_PIN_RESET)

/**
 * @brief  Turn LED3 OFF.
 *
 * @attention  This macro resets the corresponding GPIO pin of LED3 to LOW.
 */
#define LED3_L()    HAL_GPIO_WritePin(LED3_GPIO_PORT, LED3_PIN, GPIO_PIN_RESET)

/* Public variables --------------------------------------------------- */

/* Public function prototypes ----------------------------------------- */
/**
 * @brief  Turn ON LED1
 *
 * @attention Make sure GPIO pins are initialized before calling.
 */
 void turn_on_led1(void);
 /**
  * @brief  Turn ON LED2
  *
  * @attention Make sure GPIO pins are initialized before calling.
  */
 void turn_on_led2(void);
 /**
  * @brief  Turn ON LED3
  *
  * @attention Make sure GPIO pins are initialized before calling.
  */
 void turn_on_led3(void);
 /**
  * @brief  Turn OFF LED1
  *
  * @attention Make sure GPIO pins are initialized before calling.
  */
 void turn_off_led1(void);
 /**
  * @brief  Turn OFF LED2
  *
  * @attention Make sure GPIO pins are initialized before calling.
  */
 void turn_off_led2(void);
 /**
  * @brief  Turn OFF LED3
  *
  * @attention Make sure GPIO pins are initialized before calling.
  */
 void turn_off_led3(void);

#endif // __LED_CONTROL_H

/* End of file -------------------------------------------------------- */
