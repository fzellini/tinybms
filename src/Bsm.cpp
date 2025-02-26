#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "Filt.h"
#include "Bsm.h"



Bsm::Bsm(uint16_t low, uint16_t high, Filt* filter,void (*set_state_fn)(BsmState)) {
    this->low = low;
    this->high = high;
    this->set_state_fn = set_state_fn;
    this->state = BsmState::UNDEFINED;
    this->last_state = BsmState::UNDEFINED;    
    this->lp_filter = filter;
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


void Bsm::sm(uint16_t voltage){
    BsmEvent event = BsmEvent::IN_RANGE;
/*
    if (this->state == BsmState::UNDEFINED){
        this->lp_filter->init(voltage);
    }
*/
    // get voltage from filter
    voltage = this->lp_filter->push(voltage);

    if (voltage > this->high) event = BsmEvent::ABOVE;
    if (voltage < this->low) event = BsmEvent::BELOW;

    switch(event) {
        case BsmEvent::IN_RANGE:
            switch (this->state) {
                case BsmState::UNDEFINED:
                    // nell'intervallo: accendo la prima volta
                    this->set_state(BsmState::ON);
                    break;
            }
            break;
        case BsmEvent::ABOVE:
            switch (this->state) {
                case BsmState::UNDEFINED:
                    // accendo la prima volta
                    this->set_state(BsmState::ON);
                    break;        
                case BsmState::OFF:
                    // sopra la soglia: accendo
                    this->set_state (BsmState::ON);
                    break;        

            }
            break;
        case BsmEvent::BELOW:        
            switch (this->state) {
                case BsmState::UNDEFINED:
                    // first time switch on 
                    this->set_state(BsmState::OFF);
                    break;        
                case BsmState::ON:
                    // sotto la soglia: spengo
                    this->set_state (BsmState::OFF);
                    break;        
            }
            break;
    }
    
}