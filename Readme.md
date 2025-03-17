# One cell lithium battery switch control 

This system implements a voltage-controlled switch designed to protect the battery from excessive discharge. \
If the battery voltage drops below a minimum safe threshold, the system disconnects the load to prevent further depletion. \
The load will only be reconnected once the battery voltage recovers above a maximum safe threshold.

Both the minimum and maximum thresholds are configurable, providing flexibility for different battery types and applications.\
To configure the thresholds, modify **platformio.ini** build_flags:

`-D VLOW=$VLOW -D VHIGH=$VHIGH`

$VLOW is low voltage disconnect, $VHIGH is high voltage reconnect

E.g. 

`-D VLOW=3.1 -D VHIGH=3.82`

# Third version
More compact design 

![image](https://github.com/user-attachments/assets/296d596c-38b9-4501-847a-0b7628501080)


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
