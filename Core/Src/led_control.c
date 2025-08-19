/**
 * @file       <file_name>.c
 * @copyright  Copyright (C) 2019 ITRVN. All rights reserved.
 * @license    This project is released under the Fiot License.
 * @version    major.minor.patch
 * @date       yyyy-mm-dd
 * @author     <first_name_1> <last_name_1>
 * @author     <first_name_2> <last_name_2>
 *
 * @brief      <A brief description of the content of the file>
 *
 * @note
 * @example    example_file_1.c
 *             Example_1 description
 * @example    example_file_2.c
 *             Example_2 description
 */

/* Includes ----------------------------------------------------------- */
#include "led_control.h"

/* Private defines ---------------------------------------------------- */

/* Private enumerate/structure ---------------------------------------- */

/* Private macros ----------------------------------------------------- */

/* Public variables --------------------------------------------------- */

/* Private variables -------------------------------------------------- */

/* Private function prototypes ---------------------------------------- */

/* Function definitions ----------------------------------------------- */
void turn_on_led1(void)
{
	LED1_H();
}
void turn_on_led2(void)
{
	LED2_H();
}
void turn_on_led3(void)
{
	LED3_H();
}
void turn_off_led1(void)
{
	LED1_L();
}
void turn_off_led2(void)
{
	LED2_L();
}
void turn_off_led3(void)
{
	LED3_L();
}
/* Private definitions ----------------------------------------------- */

/* End of file -------------------------------------------------------- */
