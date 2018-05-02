Andrea Fernandes

Use:
-----------------------------------------------------------------------
Calibrate the first 7 ADC ports on the ESP 32 (sparkfun chip).
The pins that the code currently reads voltages from are 36, 37, 38, 39, 32, 33, 34
This code is ment to interface with 7 capacitive sensors. The sensors used are the single tact sensors
    https://www.singletact.com/micro-force-sensor/calibrated-sensors/15mm-450-newton/
There is no attenuation set for the ports. Therefore, the full scale voltage read by the MC is 0-1.1V
    This was sufficient for our project since the sensors voltage output for the force of interest is 0.5V to 1V



To Compile and Build:
----------------------------------------------------------------------
make app : builds the bianary file
make config : configure the ESP-32
make flash : uploads binary file onto the MC

***********************************************************************
Calibration experimental procedure
***********************************************************************

1) Make sure that the sparkfun board has headerpins soldered on so that it can be interfaced with a breadboard
2) Connect Vout wire of a sensor to a rail. Feed the voltage from this rail to each of the active ports on the MC
3) Connect the V+ and V- wire of the sensor to a 5V source. Also, make sure that there is a comman ground between the sensor interface board and the MC by connecting the V- of the sensor to any GND pin of the MC
4) Connect the Vout rail to a multimeter to read the voltage. 
5) Run the code to see the output of the ports vs the value of the multimeter
    - Note that the columns are as follows: pin 36, pin 37, pin 38, pin 39, pin 32, pin 33, pin 34, Start Time, End Time, Elapsed Time
    - Columns 1-7 are in milivolts, columns 8-10 are in microseconds





