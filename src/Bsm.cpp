#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "Filt.h"
#include "Bsm.h"



Bsm::Bsm(uint16_t low_spike,Filt *filter_spike, uint16_t low, Filt* filter_off, uint16_t high, Filt *filter_on, void (*set_state_fn)(BsmState)) {
    this->low_spike = low_spike;
    this->filter_spike = filter_spike;
    this->low = low;
    this->filter_off = filter_off;
    this->high = high;
    this->filter_on = filter_on;

    this->set_state_fn = set_state_fn;
    this->state = BsmState::UNDEFINED;
    this->last_state = BsmState::UNDEFINED;    
}

BsmState Bsm::get_state(){
    return this->state;
}

void Bsm::set_state(BsmState new_state){
    if (this->state != new_state){
        this->set_state_fn(new_state);
    }
    this->last_state = this->state;
    this->state = new_state;    
}


void  Bsm::sm(uint16_t voltage){
    // get voltage from filters
    uint16_t voltage_off = this->filter_off->push(voltage);
    uint16_t voltage_on = this->filter_on->push(voltage);
    uint16_t voltage_lowspike = this->filter_spike->push(voltage);

    switch (this->state){
        case BsmState::UNDEFINED: // at startup
            if (voltage_off < this->low) {
                this->set_state(BsmState::OFF);
                break;
            }
            this->set_state(BsmState::ON);
            break;
        case BsmState::OFF:
            if (voltage_off > this->high){
                this->set_state(BsmState::ON);
            }
            break;
        case BsmState::ON:
            if ( voltage_on < this->low || voltage_lowspike < this->low_spike){
                this->set_state(BsmState::OFF);
            }
            break;
    }
    
}