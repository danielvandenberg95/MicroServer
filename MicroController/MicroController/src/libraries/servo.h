/* ========================================================================== */
/*                                                                            */
/*   servo.h                                                               */
/*   (c) %YEAR% Author Dani�l van den Berg                                    */
/*                                                                            */
/*   Description                                                              */
/*                                                                            */
/* ========================================================================== */

void servo_init( int port2, int pin2 );
void servo_pulse( unsigned int length );
void servo_high( void );
void servo_low( void ); 