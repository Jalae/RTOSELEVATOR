/* 
 * File:   LED_Utils.h
 * Author: sean.connolly
 *
 * Created on March 12, 2013, 11:31 PM
 */

#ifndef LED_UTILS_H
#define	LED_UTILS_H

#ifdef	__cplusplus
extern "C" {
#endif

    //Useful Constants:
#define LED_PORTB_MASK  0x00000030
#define LED_RB4_MASK    0x00000020
#define LED_RB5_MASK    0x00000010
#define LED_PORTD_MASK  0x00000007
#define LED_RD0_MASK    0x00000001
#define LED_RD1_MASK    0x00000002
#define LED_RD2_MASK    0x00000004


    //Macro Definitions:
#define LED_Init()\
ODCBCLR = LED_PORTB_MASK;\
TRISBCLR = LED_PORTB_MASK;\
LATBCLR = LED_PORTB_MASK;\
ODCDCLR = LED_PORTD_MASK;\
TRISDCLR = LED_PORTD_MASK;\
LATDCLR = LED_PORTD_MASK;

#define LED_B_Set(leds)\
LATBCLR = leds & LED_PORTB_MASK

#define LED_B_Clr(leds)\
LATBSET = leds & LED_PORTB_MASK

#define LED_B_Inv(leds)\
LATBINV = leds & LED_PORTB_MASK

#define LED_D_Set(leds)\
LATDCLR = leds & LED_PORTD_MASK

#define LED_D_Clr(leds)\
LATDSET = leds & LED_PORTD_MASK

#define LED_D_Inv(leds)\
LATDINV = leds & LED_PORTD_MASK

    //Function Definitions:




#ifdef	__cplusplus
}
#endif

#endif	/* LED_UTILS_H */

