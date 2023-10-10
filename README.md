# Arduino driver for PMW3901 optical flow sensor

This library is based on on the C 
[driver code](https://github.com/bitcraze/crazyflie-firmware/blob/9343aa686600aa5f04beb43549c81bdfda1f9cb8/src/drivers/src/pmw3901.c#L218)
from  Bitcraze.  Unlike the PMW3901 [ Arduino repository](https://github.com/bitcraze/Bitcraze_PMW3901), their C driver provides a data
structure that includes a flag for whether motion was detected.  I made the following modifications to the C code:

1. Header-only C++

2. Cross-platform (works with Arduino and Raspberry Pi)

