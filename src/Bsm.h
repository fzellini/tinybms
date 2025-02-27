#include <stdint.h>
#include "Filt.h"

#ifndef bsm_h
  #define bsm_h

uint16_t fast_crc16(uint8_t *, uint8_t);

enum class BsmState{
    UNDEFINED,
    OFF,
    ON    
};

enum class BsmEvent{
    IN_RANGE,
    BELOW,
    ABOVE    
};


class Bsm{
  public:
    // constructor
    Bsm(uint16_t low, uint16_t high, Filt * , Filt *, void (*set_state_fn)(BsmState));
    void sm(uint16_t voltage);
    
    BsmState get_state();
    void set_state(BsmState new_state);

private:
    BsmState state;
    BsmState last_state;
    uint16_t low;
    uint16_t high;
    Filt *filter_off;
    Filt *filter_on;
    void (*set_state_fn)(BsmState);

};



#endif
