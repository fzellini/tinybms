#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "Filt.h"
#include "Bsm.h"



Bsm::Bsm(uint16_t low, uint16_t high, Filt* filter_off, Filt *filter_on, void (*set_state_fn)(BsmState)) {
    this->low = low;
    this->high = high;
    this->set_state_fn = set_state_fn;
    this->state = BsmState::UNDEFINED;
    this->last_state = BsmState::UNDEFINED;    
    this->filter_off = filter_off;
    this->filter_on = filter_on;
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
    BsmEvent event = BsmEvent::IN_RANGE;
    // get voltage from filters
    uint16_t voltage_off = this->filter_off->push(voltage);
    uint16_t voltage_on = this->filter_on->push(voltage);

    switch (this->state){
        case BsmState::UNDEFINED: // at startup
            if (voltage_off > this->high) {
                this->set_state(BsmState::ON);
                break;
            }
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
            if (voltage_on < this->low){
                this->set_state(BsmState::OFF);
            }
            break;
    }
    
}