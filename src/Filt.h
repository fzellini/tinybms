#include <stdint.h>

#ifndef filt_h
  #define filt_h


// a simple low pass filter

class Filt {
  public:

    // constructor
    Filt(uint16_t coefficient);

    uint16_t push(uint16_t value);
    void init(uint16_t value);
    
  private:
    uint32_t _accumulator;
    uint16_t  _coefficient;
};



#endif
