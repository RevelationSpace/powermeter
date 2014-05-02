powermeter
==========

This project reads out several S0 outputs from power meters and dumps the calculated wattage for each of them to serial.

How to use
==========

Connect the positive sides of the S0 outputs to the same I/O port on the Arduino. Also connect these inputs to +5V with a pull-up resistor. Connect the negative sides of the S0 outputs to the Arduino's ground.

Check that NUM\_INPUTS, INPUT\_PIN and PULSE\_ENERGY are correct for your setup.

Output
======
Each pulse triggers an output of the form:

    n: x W

* `n` is the input ID (bit on the I/O port)
* `x` is the current wattage or N/A for the first pulse (so you can still count pulses if you want)
* W is the unit, watts
