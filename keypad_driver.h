/**
 * @file 		keypad_driver.h 
 * 
 * @author 		Stephen Kairu (kairu@pheenek.com) 
 * 
 * @brief	    This file contains the definitions for a low-level driver for a 4x4
 *            keypad on the AVR ATMega328P microcontroller
 * 
 * @version 	0.1 
 * 
 * @date 		2023-09-16
 * 
 * ***************************************************************************
 * @copyright Copyright (c) 2023, Stephen Kairu
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
 * associated documentation files (the “Software”), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do
 * so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all copies or substantial
 * portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS
 * OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 * ***************************************************************************
 * 
 */
#ifndef ACCESS_CTL_KEYPAD_DRIVER_H
#define ACCESS_CTL_KEYPAD_DRIVER_H

#ifdef __cplusplus
extern "C" {
#endif

#ifndef __AVR_ATmega328P__
#define __AVR_ATmega328P__
#endif

#include <avr/io.h>
#include <avr/interrupt.h>
#include "timing_driver.h"

/**
 * The number of rows and columns on the keypad
 */
#define NUM_ROWS   4
#define NUM_COLS   4
 
 /**
  * Enumeration defining all the available rows on the keypad
  */
typedef enum ROWS {
  ROW_1 = 0,
  ROW_2,
  ROW_3,
  ROW_4,
  ROW_NONE,
}Row_t;

/**
 * Enumeration defining all the available columns on the keypad
 */
typedef enum COLS {
  COL_1 = 0,
  COL_2,
  COL_3,
  COL_4,
  COL_NONE,
}Col_t;

/**
 * Enumeration defining the key-press modes provided by the driver
 */
typedef enum KEY_PRESS {
  SHORT_PRESS,
  LONG_PRESS
}KeyPress_t;

/**
 * Enumeration defining the key-signal mode (rising or falling)
 */
typedef enum KEY_EDGE {
  FALLING_EDGE,
  RISING_EDGE,
  EDGE_NONE
}KeyEdge_t;

/**
 * @brief	 Function for setting up the keypad hardware
 * 
 * @param none
 * @return none
 */
void keypad_setup(void);

/**
 * @brief	 Function to attach a function callback, which is signaled
 *        when a key-event occurs
 * 
 * @param callback 
 */
void attach_event_callback(void (*callback)(char, KeyEdge_t));

#ifdef __cplusplus
}
#endif

#endif
