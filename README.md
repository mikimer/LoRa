# LoRa-Arduino quickstart project

_(Anna to insert cool image of sending data via LoRa here)_

# Overview
###This project allows you to quickly try LoRa communications. 
**LoRa** is an exciting new technology for communicating at **Lo**ng **Ra**nge, low power, and low cost.  This project sends data from an Arduino Uno and a [Multitech mDot LoRa node](http://www.multitech.com/models/94557148LF) to the [Senet LoRa network](https://app.senetco.com/senetdev/login.aspx). Senet's JSON data payloads are converted by [Zapier](https://zapier.com) into data rows in a Google Spreadsheet. 

This project was developed by the [Orange IoT Studio](http://orangeiotstudio.com/) to promote the consideration and adoption of LoRa. To that end, the IoT Studio is [supporting companies](http://orangeiotstudio.com/) interested in LoRa by lending out hardware kits and by funding co-development. 

### Why LoRa? 
LoRa is an ideal communications technology for many internet-of-things (IoT) devices because **internet connectivity** is a key feature of IoT but also one of IoT devices' the biggest burdens.  Specifically, pairing devices with WiFi or Bluetooth is tedious; cellular connectivity is easy to implement and can transmit megabytes (MB) of data, but its often not affordable for simple IoT devices.  For simple IoT devices that only need to send kilobytes (KB) of data or less, LoRa is an ideal solution.  

Public LoRa networks are being deployed world wide, including by [Orange in France](http://www.orange.com/en/Press-and-medias/press-releases-2016/press-releases-2015/Orange-deploys-a-network-for-the-Internet-of-Things). This project uses the [Senet LoRa network](https://app.senetco.com/senetdev/login.aspx) since it was developed by the [Orange IoT Studio](http://orangeiotstudio.com/), part of [Orange Silicon Valley](http://orangesv.com/) in San Francisco. 
 
### Disclaimer
LoRa is a new technology and this project is in alpha, so the project could break. If you're cool trying new things that aren't 100% reliable, proceed and have fun. 

## 1. Get Hardware
It should take about 30 minutes to order these parts. Hopefully they'll arrive within 1 week.
### Bill of Materials (BOM)
LittleBits
Sparkfun
### Apply to borrow a kit from the IoT Studio
The IoT Studio is [engaging with the developer community](http://orangeiotstudio.com/) to encourage use of LoRa. 
[Apply](http://orangeiotstudio.com/)

## 2. Create Accounts
It should take about 10 minutes to create these accounts.
### Senet
Senet provides a public LoRa network in North America. They're contining to increase coverage, so you can use a NorAm mote to determine whether you have coverage in your area.
### Zapier
Zapier automates the flow of data between Senet and Google Docs. 
### Google Drive
If you don't have a Google account, wow ;) You'll need [Google Spreadsheets](https://docs.google.com/spreadsheets/u/0/) to view and analyze the data.

## Verify LoRa coverage 
If you're using the Senet network, we strongly recommend that you ensure LoRa coverage.  If you omit this step, you can save the cost of the NorAm mote.
  
*Anna to make graphic of NorAm mote with GPS coming in and LoRa going out .*

*include photo of the NorAm mote*

*include link to sample gdoc w/ proper formulas from PDU to Lat/Long*

*include video with Mike explaining our NorAm mote*

These two files allow an Arduino to communicate via LoRa.  To reduce the dynamic memory of this sketch, reduce or remove the text in the Serial.print() statements.  The .ino file is for the Arduino.  The .bin file is for the Multitech mDot LoRa node. The sketch gathers sensor data to demonstrate *LoRa* by indicating activities in the kitchen: 
 1. Click. Count how many times users click a button. 
 2. Sound. Count how many times there's a loud noise.  
 3. Light. Meausre the average light level.    


## Upload Firmware
### mDot 
AT commands  
9600 baud  
Drag & Drop in MacBook. If Windows is different, please let us know how you loaded the mDot firmware and we'll update the instructions here. 

### Adruino Uno
You'll need the Arduino IDE. 

**You must customize your Arduino code so that it works with your mDot**

`/* AAA with ID ending in 11:22 */     const String mDot_name = "AAA"; const String Network_key =  "11:22:33:44:55:11:22:33:44:55:11:22:33:44:11:22"; const String Network_ID = "11:22:33:44:55";` 

*show cassis and how we labeled the mDot and tied the device # to the name*

## Assemble the hardware
Plug stuff in to match the photo

## Test drive LoRa
Power up and run it!
Let us know that it worked

## Build on LoRa, Promote your vision
We hope that you'll use this project 
Share your experience -- we're featuring the most interesting projects on our website.

## Acknolwedgements

Mike Vladimer & Anna Aflalo developed this project at the [Orange IoT Studio](http://orangeiotstudio.com), part of [Orange Silicon Valley](http://www.orangesv.com/). We welcome feedback -- you can contact us via twitter: [@orangeiotstudio](https://twitter.com/orangeiotstudio), 
 [@mikevladimer](https://twitter.com/mikevladimer) & [@anna_aflalo](https://twitter.com/anna_aflalo).  
**Thanks to** Dave Kjendal & Shaun Nelson @ Senet, Joe Knapp @ Semtech. 
 
April 2016 

