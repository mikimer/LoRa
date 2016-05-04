# 5. Assemble the hardware 

Assemble the hardware to match the diagram and photo below:

* To allow the mDot to mount properly, bend the two pins in the XB_TX row that are next to digital pins 11 and 12.    
* The Arduino transmit pin (Tx) needs to connect to the mDot's receive pin (Rx), and vice versa. To do this, place the jumpers on the XBee shield to connect mDot's receive Pin 2 (XB_RX) to Arduino pin 11. 
* Connect the mDot's transmit Pin 3 (XB_TX) to Arduino pin 10. In our Arduino sketch we configure Arduino pins 10 and 11 as Tx and Rx; if you change the pins defined in `SoftwareSerial` in the Arduino sketch, you'll need to change the hardware accordingly.  
* Mount the mDot on top of the XBee shield. Then mount the XBee shield on top of the Arduino Uno.  
* Use a wire to connect mDot reset pin 5 to Arduino analog pin A0.  
* Connect the Arduino 5V and GND to the power and ground rails (pins) on the sensors.   
* Connect the Arduino analog pins A1, A2 and A3 to the sound trigger, light sensor, and button, respectively. 

![](assets/arduino_pinout_img4.png)  
  
![](assets/LittleBits_Sensors_LoRa.png)    
    
##In case you're using Adafruit sensors  
 
 ![](assets/adafruit_img1.png)  
 
 ![](assets/adafruit_img2.png)   
 
 ![](assets/adafruit_img3.png)  
 
 ![](assets/adafruit_img4.png)

You can test your basic wiring ( Arduino Uno and sensors only - no mDot ) with this [Arduino code](assets/adafruit_wiring_test.ino).  

## Next tutorial
6.[Test drive LoRa](6_TestLoRa.md) 
