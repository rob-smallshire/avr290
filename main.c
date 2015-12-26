
/* This file has been prepared for Doxygen automatic documentation generation.*/
/*! \file *********************************************************************
*
* \brief  AVR290: Avoiding TWI Clock Stretching in ATtiny AVRs
*
*
* \par Application note:
*      AVR290:Avoiding TWI Clock Stretching in ATtiny AVRs
*
*
* \author
*      Name : Marv Kausch
*      Atmel Corporation: http://www.atmel.com \n
*      Support email: touch@atmel.com
*
* $Revision: 1.0 $
* $Date: 2011_03 02  $  \n
* Copyright (c) 2011, Atmel Corporation All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*
* 1. Redistributions of source code must retain the above copyright notice,
* this list of conditions and the following disclaimer.
*
* 2. Redistributions in binary form must reproduce the above copyright notice,
* this list of conditions and the following disclaimer in the documentation
* and/or other materials provided with the distribution.
*
* 3. The name of ATMEL may not be used to endorse or promote products derived
* from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
* WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE EXPRESSLY AND
* SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR ANY DIRECT,
* INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
* THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*****************************************************************************/

#include <avr/io.h>
#include <avr/interrupt.h>

#include <util/delay.h>

#include "drvr.h"


/* Prototypes here */

void          delay100ms(unsigned char);
void          init_demo(void);

/* End of prototypes */
/*!  Two variables used to send data to TWI Slave */
unsigned char count1, command; 

/*! 100 msec delay to be tuned by the user, based on choice of AVR and its clock speed */
#define DELAY_VAL_100_MS          0xFFFF-0xe000;  //User: this is an example value when CPU clock speed is 8 MHz.  Adjust this value get
                                                  // your delay value, 100 msec if desired
#define YES                       1
#define NO                        0

/*! Enable/disable LEDs on Slave STK500 or other demo board */
#define LED_DEMO_ON               YES            // PORTA is to display 8 bits of count1 value and PA7 is high while in TWI interrupt servie
#define LED_OUT_CTRL              DDRA           // for defining outputs on LEDs.   May be changed to another port or eliminated.
#define LED_DATA                  PORTA          // For diaplaying count1 value on LEDa

int main(void)
{
  
  /* initialize Demo */
  init_demo();
  
  /* initialise TWI Slave */
  twi_slave_initialise();
  
  /* Enable interrupts. */
  sei();
  

/*! /brief example code
*
* The SRAM variable count1 increments approximately once every 2 seconds.
* If LED_DEMO_ON is set to YES, 7 LEDs on Slave board will show the changing value oc count1
 *
*/
    while(1){
    count1++;
    
#if LED_DEMO_ON
   LED_DATA = ~count1;  // Use PORTA to display count1 data
        _delay_ms(1000);
#endif
    }

  
}
/*! /user initialalize function
*
*  Set tinyAVR clock speed from default 1 MHz to 8 Mhz
*  
*  IF LED_DEMO is enabled, set LED port pins to outputs
*/
void init_demo(void){

    /* Clear CLKPSR bits to get AVR to run at 8 MHz  */
  CCP = 0xD8; // write access signature to Configuration Change Protection Register
  CLKPSR = 0;  // This should change clock from 1 MHz default to 8 MHz
  
#if LED_DEMO_ON
  LED_OUT_CTRL = 0x7f;  //PA7 is SCL on tiny20/tiny40 AVR
#endif
}

/*!  \brief  TWI slave twi_data_to_master driver callback function
 * 
 *   This is an example of a command-specific response logic
 *   to demonstrate how the TWI driver gets data from the application main code
 *   Input: none
 *   Output: for demo purposes, this function returns a value in count1 or ~count1 based on the
 *   value of command, previously sent from the master to the slave and this code
 *   The user may change this function's respoonse to suit a particular application
 *
 */
unsigned char twi_data_to_master(void){
  unsigned char data_to_master;
    if (command == 1) data_to_master = count1;  
    else data_to_master = ~count1;  
  return data_to_master;
}


/*!  \brief  TWI slave twi_data_from_master driver callback function
 *
 *   This is an example of a command-specific response logic
 *   to demonstrate how the TWI driver can send a command from teh TWI master
 *    to the slave code's application
 *   Input:  data from master to slave then to user�s code
 *   
 *   Output: User code receives new data via the function returning a value
 *  
 */
void twi_data_from_master(unsigned char data){

   command = data;

}
