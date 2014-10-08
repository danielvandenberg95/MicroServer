/* ========================================================================== */
/*                                                                            */
/*   PWM.h                                                               */
/*   (c) 2014 Author Daniël van den Berg                                    */
/*                                                                            */
/*   Description                                                              */
/*                                                                            */
/* ========================================================================== */

//!initualizes the pwm
//
//!Initializes the pwm system, so that pin 1_1 can be used as pwm output.
void pwm_init();
//!set the value of the pwm
//
//!Sets the value of the pwm. Value has to be between 0 and 100, 0 being off and 100 being on.
void pwm_set_value(int value);