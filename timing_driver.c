/**
 * @file 		timing_driver.c 
 * 
 * @author 		Stephen Kairu (kairu@pheenek.com) 
 * 
 * @brief	    This file contains the definitions for a low-level timing driver
 *            for generating accurate, non-blocking ms timing clock ticks
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
#include "timing_driver.h"

volatile unsigned long timing_millis = 0; /*< Variable to keep track of the number of elapsed milliseconds */

/**
 * @brief	 Function to initialize and setup Timer 2
 *         Prescaler 64, with the overflow interrupt enabled (overflows in approximately 1ms)
 * 
 * @param none
 * @return none
 */
void timer_init(void)
{
  // enable timer2 (prescaler 64)
  TCCR2B |= (1 << CS22);
  // enable timer2 overflow interrupt
  TIMSK2 |= (1 << TOIE2);
}

/**
 * @brief	Returns the number of elapsed milliseconds
 * 
 * @param none
 * @return unsigned long -> elapsed time in milliseconds
 */
unsigned long get_timing_millis(void)
{
	return timing_millis;
}

/**
 * @brief	Timer overflow ISR
 *        Executed when Timer 2 overflows (approximately every 1ms) 
 */
ISR(TIMER2_OVF_vect)
{
  // increment on every overflow (1 ms elapsed)
  timing_millis+= 1;
}
