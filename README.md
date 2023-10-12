<a href="https://www.bitcraze.io/products/flow-deck-v2/"><img src="media/flowdeckv2.jpg"></a>

# Arduino driver for PMW3901 optical flow sensor

This library is based on on the C 
[driver code](https://github.com/bitcraze/crazyflie-firmware/blob/9343aa686600aa5f04beb43549c81bdfda1f9cb8/src/drivers/src/pmw3901.c#L218)
for the Crazyflie quadcopter from  Bitcraze.  Unlike the Bitcraze PMW3901 [ Arduino repository](https://github.com/bitcraze/Bitcraze_PMW3901), 
this C driver provides a data structure that includes a flag for whether motion was detected, which you may find usef.  
I made the following modifications to the C code:

1. Header-only C++

2. Support cross-platform development defining an 
[API](https://github.com/simondlevy/PMW3901/blob/master/src/spi_compat.h)
of required methods for SPI communication

