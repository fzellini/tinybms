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


# Second version, with periodic reset
I added a periodic reset feature in the second version, utilizing a MOSFET to briefly pull down the RESET signal of the main processor.\
Firmware default is a reset each 24 hours

## Schematic diagram
![schematic diagram](https://github.com/user-attachments/assets/02e85fed-28e0-4027-a678-af4b723c1b3a)

## pcb 3d model
![image](https://github.com/user-attachments/assets/5aae3512-722c-4964-b96a-51b669fd800a)


# First version
## Schematic diagram
![schematic diagram](tinybms.png)

## pcb 3d model
![image](https://github.com/user-attachments/assets/e9e6ac01-f1af-419d-802d-0e3b83629764)

## video
[first prototype video](https://www.youtube.com/watch?v=vm5XbdbVL_Y)

## PCB
![first PCB](https://github.com/user-attachments/assets/c3d5bb65-e20c-4ee7-ad5e-f71f9faf53b6)

**Warning!!**, this version of pcb has non-standard programming socket.\
Double check pinout before programming
