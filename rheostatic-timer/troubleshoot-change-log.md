### v. 1-? Date Unknown
Internal 5v regulator on Nano broke
No way to supply power to any of the componenets that need 5v instead of the 12v direct supply

Implemneting an external 5v regulator (L7805CV) and smoothing caps (maybe heatsink?) to replace the Arduino's internal reg
Still need a way to power the Arduino while making sure it has common ground with the other components.


### v. 2.20 05302018
UPDATE: Fixed code and power reg issues fully.
Everything functions as intended except the buttons and changeVals won't work
This indicated the possibility of the arduino & buttons, pots not having a common ground

Current method of power delivery to arduino by hacked-together USB cable won't work, need another solution.

MOST LIKELY - connect arduino +5v and GND pins (UNREG) to common supply lines
buttons on different pin(s)?
change method of input, pulldown on +5v instead of pullup on GND, physical pullup resistor, etc?

