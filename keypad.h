/**
 * @file 		keypad.h 
 * 
 * @author 		Stephen Kairu (kairu@pheenek.com) 
 * 
 * @brief	    This file contains the definitions for a Keypad class which controls interactions
 *            with the low-level keypad driver
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
#ifndef ACCESS_CTL_KEYPAD_H
#define ACCESS_CTL_KEYPAD_H

#include "keypad_driver.h"

typedef enum {
  IDLE_STATE,
  DEFAULT_STATE,
  PIN_STATE,
  NAVIGATION_STATE
}KeypadStates_t;

class AccessCtlKeypad
{
  private:
    KeypadStates_t currentKeypadState = DEFAULT_STATE;
  public:
    AccessCtlKeypad(void);
    ~AccessCtlKeypad(void) {}

    void attachKeypadCallback(void (*callback)(char, KeyEdge_t));
    KeypadStates_t getCurrentKeypadState(void);
    void changeKeypadToState(KeypadStates_t keypadState);
};

#endif
 
