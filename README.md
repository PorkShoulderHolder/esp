# esp

This is a modified version of https://github.com/nodemcu/nodemcu-firmware along with python tools for flashing the firmware and user scripts

## getting started
Here are some notes for setting up in mac OS 10.10, which generally follow the steps outlined in http://www.allaboutcircuits.com/projects/guts-of-the-iot-part-1-building-nodemcu-from-source-for-the-esp8266/.

1. if using the nodemcu amica devkit install the appropriate usb driver to communicate using the onboard micro-usb, otherwise connect your board to an external FTDI converter, like this: https://moderndevice.com/product/usb-bub-i/. 
2. Either way, ensure that you can communicate with the chip with the preloaded AT command set. You can use screen for this:
`$ screen /dev/<your device name> 115200`
A possible device name for the amica devkit is something like `cu.SLAB_USBtoUART`, but this will depend on what you did for step 1. Once connected on screen you should be able to enter `OK` by hitting return followed by control-J
```
AT

OK
```
You can configure all sorts of things on the boards with the AT command set. See http://www.pridopia.co.uk/pi-doc/ESP8266ATCommandsSet.pdf
3. Now that we have established sucessful communication with the esp8266 we can move on to compiling and flashing the NodeMCU Source



 
 

