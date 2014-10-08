/* ========================================================================== */
/*                                                                            */
/*   timer.h                                                                  */
/*   (c) 2012 Author Wouter van Ooijen                                        */
/*   comment by Daniël van den Berg                                           */
/*                                                                            */
/*   Description                                                              */
/*                                                                            */
/* ========================================================================== */

//!@file
//! This library is used to keep track of a timer specified by the LPC_TMR32B0

#ifndef TIMER_H_
#define TIMER_H_

//!initializes the timer
//
//!This function is used to initialize the timer by setting it to zero, setting the correct interval and starting it.
void timer_init(void);
//!returns the current time
//
//!@return the time since the timer has been started.
unsigned int now();
//!awaits for a certain time
//
//!Places the code into a while loop to wait until the specified time has passed.
//!@param t the time to wait for relative to the moment the timer started.
void await(unsigned int t);

//!hangs the code for a certain time
//
//!Places the code into a while loop to wait until the specified amount of nanoseconds has passed.
//!@param t the time to wait in nanoseconds.
void delay(unsigned int t);

//!hangs the code for a certain time
//
//!Places the code into a while loop to wait until the specified amount of milliseconds has passed.
//!@param t the time to wait in milliseconds.
void delay_millis(unsigned int t);
                                   
//!hangs the code for a certain time
//
//!Places the code into a while loop to wait until the specified amount of seconds has passed.
//!@param t the time to wait in seconds.
void delay_seconds(unsigned int t);

#endif
