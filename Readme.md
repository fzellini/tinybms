# One cell lithium battery switch control 

This system implements a voltage-controlled switch designed to protect the battery from excessive discharge. \
If the battery voltage drops below a minimum safe threshold, the system disconnects the load to prevent further depletion. \
The load will only be reconnected once the battery voltage recovers above a maximum safe threshold.

Both the minimum and maximum thresholds are configurable, providing flexibility for different battery types and applications.\
To configure the thresholds, modify the instantiation of the Bsm object in main.c as follows:

`Bsm bsm(681,822, set_state);`

The first parameter (681) corresponds to the scaled value of the low voltage threshold.\
The second parameter (822) corresponds to the scaled value of the high voltage threshold.\
To calculate these scaled values for a given voltage, use the formula:

`scaled_value = int((VOLTAGE / 1.1 / 43) * 10 * 1024)`

For example:

A voltage of 3.15V corresponds to a scaled value of 682.\
A voltage of 3.8V corresponds to a scaled value of 822

## Schematic diagram
![schematic diagram](tinybms.png)


[first prototype video](https://www.youtube.com/watch?v=vm5XbdbVL_Y)

