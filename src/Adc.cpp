#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <avr/power.h>

#include "ATtinySerialOut.h"
#include "Adc.h"

Adc::Adc(){
}

void Adc::setup(){
/* this function initialises the ADC 

  For more information, see table 17.5 "ADC Prescaler Selections" in 
  chapter 17.13.2 "ADCSRA – ADC Control and Status Register A"
  (pages 140 and 141 on the complete ATtiny25/45/85 datasheet, Rev. 2586M–AVR–07/10)

  
  // 10-bit resolution
  // set ADLAR to 0 to disable left-shifting the result (bits ADC9 + ADC8 are in ADC[H/L] and 
  // bits ADC7..ADC0 are in ADC[H/L])
  // use uint16_t variable to read ADC (intead of ADCH or ADCL) 
  
  */

  ADMUX =
	    (0 << ADLAR) |     // do not left shift result (for 10-bit values)
            (0 << REFS2) |     // 
            (1 << REFS1) |     // 
            (0 << REFS0) |     // 010 = internal 1.1 voltage 
            (0 << MUX3)  |     // 
            (0 << MUX2)  |     // 
            (1 << MUX1)  |     // 
            (1 << MUX0);       // 0011 = ADC3(PB3)

  ADCSRA = 
            (1 << ADEN)  |     // Enable ADC 
            (1 << ADPS2) |     // 
            (0 << ADPS1) |     // 
            (0 << ADPS0);      // 100 = divide by 16

}

uint16_t Adc::read(){
    uint8_t adc_lobyte;
    uint16_t raw_adc;
    ADCSRA |= (1 << ADSC);         // start ADC measurement
    while (ADCSRA & (1 << ADSC) ); // wait till conversion complete 

    // for 10-bit resolution:
    adc_lobyte = ADCL; // get the sample value from ADCL
    raw_adc = ADCH<<8 | adc_lobyte;   // add lobyte and hibyte    
    return raw_adc;

}

void Adc::disable(){
    // power_adc_disable();
    // ADCSRA &=~(1<<ADEN);
    // PRR |= (1 << PRADC);    
    // write also PRR ?

}

void Adc::enable(){
    // PRR &= ~(1 << PRADC); 
    // ADCSRA |=(1<<ADEN);
    // power_adc_enable();
    
}
