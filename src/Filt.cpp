#include <Arduino.h>
#include "Filt.h"


Filt::Filt(uint16_t coefficient){
  _coefficient = coefficient;
  _accumulator = 0;  
}

uint16_t Filt::push (uint16_t value){
  _accumulator -= _accumulator >> _coefficient;
  _accumulator += (uint32_t) value;
  return (uint16_t) (_accumulator >> _coefficient) ;
}

void Filt::init (uint16_t value){
  _accumulator = ((uint32_t) value << this->_coefficient);
}
