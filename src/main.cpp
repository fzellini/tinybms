#include <Arduino.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <avr/power.h>


#define TX_PIN PIN_PB2

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


void goToSleep ()  {
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    power_all_disable ();  // power off ADC, Timer 0 and 1, serial interface
    cli();
    sleep_enable();
    sei();
    sleep_cpu();
    sleep_disable();
    power_all_enable();    // power everything back on
    sei();
}


void set_state(BsmState state){
    if (state==BsmState::OFF){
        #ifndef LDO
            PORTB |= (1 << PB0);
        #else
            PORTB &= ~(1 << PB0);
        #endif
    }
    if (state==BsmState::ON){
        #ifndef LDO
            PORTB &= ~(1 << PB0);
        #else
            PORTB |= (1 << PB0);
        #endif
        // small delay after power on
        delay(200);
    }
}


#define BMS_SPIKE (uint16_t)((VSPIKE/1.1/(RLOW+RHIGH))*RLOW*1024.)
#define BMS_LOW  (uint16_t)((VLOW/1.1/(RLOW+RHIGH))*RLOW*1024.)
#define BMS_HIGH (uint16_t)((VHIGH/1.1/(RLOW+RHIGH))*RLOW*1024.)


#define FILT_PAR_OFF 2  // taps = 2<<FILT_PAR
#define FILT_PAR_ON 9  // taps = 2<<FILT_PAR
#define FILT_PAR_SPIKE 1


// averaging filter
Filt filt_on(FILT_PAR_ON);

Filt filt_off(FILT_PAR_OFF);
Filt filt_spike(FILT_PAR_SPIKE);

Bsm bsm( BMS_SPIKE, &filt_spike ,BMS_LOW, &filt_off, BMS_HIGH, &filt_on,  set_state);
Adc adc;

uint32_t offset = 0;

void setMillis(uint32_t newMillis) {
  offset = newMillis - millis();
}

uint32_t getMillis() {
  return millis() + offset;
}

#define RESET_COUNTER_TIMEOUT (86400*1000)

int main(){
    uint16_t v;

    init();
    setMillis(0);

    #ifndef LDO
    // PB0 drives PMOS switch ( low = ON)
    // PB4 drive NMOS switch to read battery voltage ( high=READ)
    // PB2/SCK drive RESET, deploying a periodic pulse

        PORTB |= ((1 << PB0));      // default high
        DDRB  |= ((1 << PB0)|(1 << PB2)|(1 << PB4));
    #else
    // PB0 drives PMOS switch ( high = ON)
    // PB4 drive NMOS switch to read battery voltage ( high=READ)
    // PB2/SCK drive RESET, deploying a periodic pulse

        DDRB  |= ((1 << PB0)|(1 << PB2)|(1 << PB4));

    #endif

    setupWatchdog();
    delay(100);
    // setup ADC
    adc.setup();

    // preload digital filter
    adc.enable();
    PORTB |= (1 << PB4);  // accendo il mosfet del partitore
    for (int i=0; i< (2<<(FILT_PAR_OFF+1)); i++) {
        v = adc.read();
        v = filt_off.push(v);
        delay(10);
    }
    filt_on.init(v);
    filt_spike.init(v);

    while (true){

        uint16_t raw = adc.read();
        bsm.sm(raw);

        if (bsm.get_state() == BsmState::OFF){
            PORTB &= ~(1 << PB4); // spengo il mosfet del partitore
            setMillis(0);
            adc.disable();
            goToSleep();         // entra in sleep fino al prossimo interrupt
            adc.enable();
            PORTB |= (1 << PB4);  // accendo il mosfet del partitore
        } else {
            delay(10);
            uint32_t ms = getMillis();
            if (ms > RESET_COUNTER_TIMEOUT){
                // generate reset pulse
                PORTB|=(1<<PB2);
                delay(150);
                PORTB&=~(1<<PB2);
                setMillis(0);
            }
        }
    }
}




