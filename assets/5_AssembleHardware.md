# 5. Assemble the hardware 

Assemble the hardware to match the diagram and photo below:

* To allow the mDot to mount properly, bend the two pins in the XB_TX row that are next to digital pins 11 and 12.    
* The Arduino transmit pin (TX1) needs to connect to the mDot's receive pin (RX). To do this, use a male-female wire to connect XBee shield to connect mDot's receive Pin 2 (XB_RX) to Arduino pin 18. 
* The Arduino receive pin (RX1) needs to connect to the mDot's transmit pin (TX). To do this, use a male-female wire to connect XBee shield to connect mDot's receive Pin 3 (XB_TX) to Arduino pin 19.
* Mount the mDot on top of the XBee shield. Then mount the XBee shield on top of the Arduino Mega.  
* Use a wire to connect mDot reset pin 5 to Arduino analog pin A0.  
* Connect the Arduino 5V and GND to the power and ground rails (pins) on the sensors.   
* Connect the Arduino analog pins A1, A2 and A3 to the sound trigger, light sensor, and button, respectively. 

![](arduino_pinout_img4.png)  
  
![](LittleBits_Sensors_LoRa.png)    
    
##In case you're using Adafruit sensors  
 
 ![](adafruit_img1.png)  
 
 ![](adafruit_img2.png)   
 
 ![](adafruit_img3.png)  
 
 ![](adafruit_img4.png)

You can test your basic wiring ( Arduino Mega and sensors only - mDot not mounted yet ) with this [Arduino code](adafruit_wiring_test.ino).  

### Next tutorial
6.[Test drive LoRa](6_TestLoRa.md) 
