# 4. Configure the software
  

## Get the Arduino sketch
[![](button_LoRa_Arduino_sketch.png)](LoRa_Arduino_quickstart_April2016.ino?raw=true)  

If you don't already have it, get the [Arduino IDE](https://www.arduino.cc/en/Main/Software).  Copy-paste our template code for [the LoRa-Arduino sketch](LoRa_Arduino_quickstart_April2016.ino?raw=true) into a clean Arduino sketch. 


You must customize your Arduino code so that it works with your specific mDot.  You'll now get your unique identifiers from Senet that you'll copy/paste into the LoRa-Arduino sketch.   
  
## Register the mDot with Senet and get identifiers
You'll need to register the mDot with Senet to get identifiers for the Arduino sketch.  First, go to the [Senet portal](https://portal.senetco.com/) and register your device by providing the device ID and nickname (similar to what you did with the NorAm mote). Next, click on the mDot name to open a webpage with the mDot's information and then follow these steps: 

1. Click on the gear to open the menu.
2. Click on `Device Edit` to reveal the `Device Setup/Edit` sub-menu.
3. Take note of `mDot_name`, the nickname you gave the mDot
4. Take note of `XXXX`, the last 4 digits of the ID number on the mDot
5. Take note of the Senet `App Key`, which is referred to as the `Network_key` in the Arduino sketch.
6. Take note of the Senet `Application` number, which is referred to as the `Network_ID` in the Arduino sketch. 

![](Senet_select_device0.png)  
   
![](Senet_menu12.png)

![](Senet_register_mDot3456.png)


## Customize your Arduino sketch
You'll need to modify the code with your values from Senet. Starting at Line 27 is a template for you to modify: 

```Arduino
// *** Select the current mDot **
// Update the mDot data below to reflect the name, network key, and network identifier your mDot(s) 
// You need to adjust the comments in the code to select the mDot you're currently using
/* AAA with ID ending in A7:77 */    //  const String mDot_name = "AAA"; const String Network_key = "11223344551122334455112233445566"; const String Network_ID = "00250C0000010001"; 
/* BBB with ID ending in 33:44 */    //  const String mDot_name = "BBB"; const String Network_key = "11223344551122334455112233445566"; const String Network_ID = "00250C0000010001"; 
/* CCC with ID ending in 55:66 */    //  const String mDot_name = "CCC"; const String Network_key = "11223344551122334455112233445566"; const String Network_ID = "00250C0000010001"; 
```

Replace `AAA` with your mDot nickname, `11:22` with the last 4 digits of your mDot ID number, as well as the `Network_key` and `Network_ID` with your unique values from Senet.  Note that Senet provides the ID in the format `0x11,0x22,0x33,...` and `112233` -- you'll want to use the cleaner format.

This template assumes that you have three mDots (you're ambitious!) named `AAA`, `BBB` and `CCC` with ID numbers ending in `11:22`, `33:44` and `55:66`, respectively, since we found it useful to work with a few mDots.  If you've only got one mDot, then simply delete the lines for `BBB` and `CCC`. If you've got multiple mDots, then you can update `BBB` and `CCC` with your mDot information. When you run the sketch, you'll need to ensure that the code is commented with `//` so that only one mDot is selected at a time.  Be sure to save your customized file. 

#### How the sketch works

We've commented the Arduino .ino file extensively so that you can understand how the code works.  In brief, in `setup()` the device sends AT commands to join the LoRa network and in `loop()` the device samples the sensors every 100ms and sends a LoRa data payload every 15 minutes.  The sketch gathers sensor data to demonstrate *LoRa* by monitoring activities:  

 1. `Click` Count how many times users click a button.  
 2. `Sound` Count how many times there's a loud noise.   
 3. `Light` Meausre the average light level.

Here's a brief explanation of the the AT commands used in this tutorial. [Full details](http://www.multitech.com/manuals/s000643_1_1.pdf) are available from Multitech.   

* `ATE0` turns off the echo so that the mDot doesn't echo back the commands the Arduino sends it.  This makes it easier for the Arduino sketch to parse the mDot's responses.  
* `ATV0`  turns off verbose mode so that the mDot provides brief responses.  This makes it easier for the Arduino sketch to parse the mDot's responses.  
* `AT+TXDR=9` sets the spreading factor to 9, with a corresponding data rate of 1 and maxium payload size of 53 bytes.  To improve operation, you can set AT+TXDR=10 to the reduce the payload size to 11 bytes (SF=10; DR=0), however you'll need to reduce the size of the data payload. 
* `AT+TXP=20` sets the mDot to the maximum transmit power, 20.  
* `AT+PN=1` puts the mDot in public network mode.  
* `AT+FSB=1` sets the frequency sub-band of the mDot. (**1**: 902.3-903.7 MHz, **2**: 903.9-905.3 MHz,..., **8**: 913.5-914.9 MHz)
* `AT+NJM=1` sets the Over The Air network join mode of the mDot.  
* `AT+JD=5` sets the join delay of the mDot to 5 seconds.  
* `AT` elicts an echo of "OK" from the mDot. This command was used for debugging and is not strictly necessary.  
* `AT+NI=0, <Network Identifier>` sets the network identifier for Senet network. 
* `AT+NK=0, <Network Key>` sets the network key for your device in your Senet account in the Developer Portal Device Setup window. 
* `AT+JOIN` joins your mDot to the Senet network. 

Additional tools that could be useful when you build off this quickstart tutorial:  

* `AT&W` saves the configuration to your mDot.  
* `AT&V` makes the mDot respond with its configuration which is useful to verify that your settings were properly applied. 
* The [MultiConnect mDot Micro Developer Kit](http://www.multitech.com/brands/micro-mdot-devkit) was a crucial tool for issuing AT commands directly to the mDot via the Terminal without the Arduino.  We used the `screen` command on our MacBook to connect with the mDot. The full command is: `screen /dev/tty.usbmodem14121 115200`, where the `usbmodem` number changes with your device and port and `115200` corresponds to the baud rate, which is 115200 by default for the mDot. 
  
### Next tutorial
5.[Assemble the hardware](5_AssembleHardware.md) 
