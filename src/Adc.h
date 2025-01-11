#include <stdint.h>

#ifndef adc_h
  #define adc_h


class Adc{
  public:
    // constructor
    Adc();
    void setup();
    uint16_t read();
    void disable();
    void enable();
};



#endif
