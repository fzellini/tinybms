#include <Arduino.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <avr/power.h>


#define TX_PIN PIN_PB2
#include "ATtinySerialOut.hpp"

#include "Bsm.h"
#include "Adc.h"

// Interrupt Service Routine per il Watchdog Timer
ISR(WDT_vect) {
}

void setupWatchdog() {
    // Configura il Watchdog Timer per risvegliarsi dopo circa 8 secondi
    MCUSR &= ~(1 << WDRF); // Resetta il flag del Watchdog

    // Abilita il Watchdog Timer con timeout di 8s (configurazione prescaler)
    WDTCR = (1 << WDCE) | (1 << WDE); // Abilita la modifica dei registri WDT
    WDTCR = (1 << WDP3); // Imposta il timeout a 4s

    WDTCR |= (1 << WDIE); // Abilita l'interrupt del Watchdog Timer
}

void enterSleep() {

    set_sleep_mode(SLEEP_MODE_PWR_DOWN); // Modalità di sleep a basso consumo
    cli(); // Disabilita gli interrupt globali
    sleep_enable(); // Abilita la modalità di sleep

    sei(); // Riabilita gli interrupt globali
    power_all_disable();

    sleep_cpu(); // Mette il microcontrollore in sleep

    power_all_enable();

    sleep_disable(); // Disabilita la modalità sleep dopo il risveglio
    sei(); // Assicurati che gli interrupt siano abilitati
}


void set_state(BsmState state){
    if (state==BsmState::OFF){
        PORTB |= (1 << PB0);
        Serial.println(F("OFF"));
    }
    if (state==BsmState::ON){
        PORTB &= ~(1 << PB0);
        Serial.println(F("ON"));
    }
}

// todo: add checksum and set low-hi outside of constructor
/*
>>> (3.15/1.1/43)*10*1024
681.9450317124735
>>> (3.80/1.1/43)*10*1024
822.6638477801268

*/
Bsm bsm(681,822, set_state);
Adc adc;

int main(){

    init();

    DDRB |= ((1 << PB0)|(1<<PB4));
    set_state(BsmState::OFF);

    initTXPin(); // tiny serial 
    setupWatchdog();


    Serial.println(F("START " __FILE__ " from " __DATE__ "\r\nUsing library version " VERSION_ATTINY_SERIAL_OUT));
    delay(200);
    
    // setup ADC
    adc.setup();


    while (true){

        PORTB |= (1 << PB4);  // accendo il mosfet del partitore
        adc.enable();         // abilito l'ADC
        uint16_t raw = adc.read();
        bsm.sm(raw);
        PORTB &= ~(1 << PB4); // spengo il mosfet del partitore
        Serial.print(F("adc: "));
        Serial.println(raw);
        adc.disable();        // disattivo l'ADC
        enterSleep();         // entra in sleep fino al prossimo interrupt

    }

}




