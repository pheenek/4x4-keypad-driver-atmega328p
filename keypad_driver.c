/**
 * @file 		keypad_driver.c 
 * 
 * @author 		Stephen Kairu (kairu@pheenek.com) 
 * 
 * @brief	    This file contains the implementations for a low-level driver for a 4x4
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
#include "keypad_driver.h"

volatile uint16_t timerCnt = 0;

// Mappings for the keypad rows and columns
char key[NUM_ROWS][NUM_COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'},
};

// Program variable definitions
volatile Row_t currentRow = ROW_NONE;
volatile Col_t currentCol = COL_NONE;
volatile KeyEdge_t currentKeyEdge = EDGE_NONE;

volatile uint8_t keyState = 0;
volatile uint8_t previousKeyState = 0;

volatile unsigned long lastKeyPressMillis = 0;
volatile unsigned long pinChangeMillis = 0;

// Function pointer variable for storing function pointer to defined function callback
void (*keypad_event_callback)(char, KeyEdge_t);

// Function declarations for private functions
void keypad_event_listener(void);
void timer1_init(void);

/**
 * @brief	 Function for setting up the keypad hardware
 * 
 * @param none
 * @return none
 */
void keypad_setup(void)
{
  timer_init();
  // enable pin change interrupts
  PCICR |= (1 << PCIE1);
  
  // set pins to inputs
  DDRC &= ~(1 << DDC0) & ~(1 << DDC1) & ~(1 << DDC2) & ~(1 << DDC3);
  // enable internal pull-ups
  PORTC |= (1 << PORTC0) | (1 << PORTC1) | (1 << PORTC2) | (1 << PORTC3);
  // enable interrupts on pins
  PCMSK1 |= (1 << PCINT8) | (1 << PCINT9) | (1 << PCINT10) | (1 << PCINT11);
  // pull all columns low
  DDRD |= (1 << DDD4) | (1 << DDD5) | (1 << DDD6) | (1 << DDD7);
  PORTD &= ~(1 << PORTD4) & ~(1 << PORTD5) & ~(1 << PORTD6) & ~(1 << PORTD7);
}

/**
 * @brief	 Function to initialize Timer1 with no prescaler
 *        Timer1 is used to set precise delays in clock ticks for reading the keypad
 * 
 * @param none
 * @return none
 */
void timer1_init(void)
{
  TCCR1B = _BV(CS10);
}

/**
 * @brief	 Function called when a keypad-event is detected
 *        This function in-turn calls the keypad callback with the key-character,
 *        and the signal state of the detected signal change (rising or falling)
 * 
 * @param none
 * @return none
 */
void keypad_event_listener(void)
{
  // If a callback is not defined, return
  if(!keypad_event_callback) return;

  keypad_event_callback(key[currentRow][currentCol], currentKeyEdge);

  if (currentKeyEdge == RISING_EDGE) currentKeyEdge = EDGE_NONE;
}

/**
 * @brief	 Function to attach a function callback, which is signaled
 *        when a key-event occurs
 * 
 * @param callback 
 */
void attach_event_callback(void (*callback)(char, KeyEdge_t))
{
  keypad_event_callback = callback;
}

/**
 * @brief	ISR for detecting and responding to keypad events.
 */
ISR(PCINT1_vect)
{
  // include debounce time 
  if ((get_timing_millis() - lastKeyPressMillis) >= 50)
  {
    lastKeyPressMillis = get_timing_millis();
        
    previousKeyState = keyState;
    // shift readings of PC0 - PC4 into the upper 4 bits of the 8-bit value
    keyState = (PINC << 4);
    
    // If the keystate has changed (the interrupt is called both on the rising and falling edges)
    if (previousKeyState != keyState)
    {
      if (keyState ^ 0xF0) // falling edge (check for zero in keyState)
      {
        currentKeyEdge = FALLING_EDGE;
        
        // Serach for the row for which the change occurred
        if (bit_is_clear(PINC, PC0)) currentRow = ROW_1;
        if (bit_is_clear(PINC, PC1)) currentRow = ROW_2;
        if (bit_is_clear(PINC, PC2)) currentRow = ROW_3;
        if (bit_is_clear(PINC, PC3)) currentRow = ROW_4;
    
        // pull cols high *verified*
        PORTD |= (1 << PORTD4) | (1 << PORTD5) | (1 << PORTD6) | (1 << PORTD7);

        // Wait for the columns to change state
        while ((PIND >> 4) != 0x0F);
    
        // loop through columns 
        for (int col = 0; col < NUM_COLS; col++)
        {
          timerCnt = 0;
          TCNT1 = 0;
          // set a single column low at a time *verified*
          PORTD &= ~(1 << (col+4));
    
          // delay for external change to take effect
          // For some reason it takes different times for the change to take effect
          // Imperically: 24clks for row 1, 34 clks for row 2, 44 clks for row 3, 54 clks for row 4
          // And some sporadic bursts of up to 65clks (as per observations
          while (TCNT1 < 80); // delay for 5us
          
          /**
           * If the row has changed state, then we've found the culprit column!
           * Else return the column to its original state and proceed to test the next column
           */
          if (bit_is_clear(PINC, currentRow))
          {
            currentCol = col;
            break;
          }
          else
          {
            PORTD |= (1 << (col+4));
          }
        }
    
        // Reset the states of all the columns
        PORTD &= ~(1 << PORTD4) & ~(1 << PORTD5) & ~(1 << PORTD6) & ~(1 << PORTD7);
  
      }
      else if (keyState == 0xF0) // rising edge (we already know the key, hence no need to search again)
      {
        currentKeyEdge = RISING_EDGE;
      }

      // Execute the keypad-event detected routine
      keypad_event_listener();
    }
  }

}
 
