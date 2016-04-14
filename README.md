# LoRa-Arduino quickstart project

_(Anna to insert cool image of sending data via LoRa here)_

# Overview
###This project allows you to quickly try LoRa. 
LoRa is an exciting new technology that enables **Lo**ng **Ra**nge, low power, low cost communciations.  

Public LoRa networks are being deployed world wide, including by [Orange in France](http://www.orange.com/en/Press-and-medias/press-releases-2016/press-releases-2015/Orange-deploys-a-network-for-the-Internet-of-Things). This

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

## Get Firmware
### mDot 
AT commands  
9600 baud  
Drag & Drop in MacBook. If Windows is different, please let us know how you loaded the mDot firmware and we'll update the instructions here. 

### Adruino Uno
You'll need the Arduino IDE. 

## Validate Senet coverage 



These two files allow an Arduino to communicate via LoRa.  To reduce the dynamic memory of this sketch, reduce or remove the text in the Serial.print() statements.  The .ino file is for the Arduino.  The .bin file is for the Multitech mDot LoRa node. The sketch gathers sensor data to demonstrate *LoRa* by indicating activities in the kitchen: 
 1. Click. Count how many times users click a button. 
 2. Sound. Count how many times there's a loud noise.  
 3. Light. Meausre the average light level.    


## Acknolwedgements

Mike Vladimer & Anna Aflalo developed this project at the [Orange IoT Studio](http://orangeiotstudio.com), part of [Orange Silicon Valley](http://www.orangesv.com/). We welcome feedback -- you can contact us via twitter: [@orangeiotstudio](https://twitter.com/orangeiotstudio), 
 [@mikevladimer](https://twitter.com/mikevladimer) & [@anna_aflalo](https://twitter.com/anna_aflalo).  
**Thanks to** Dave Kjendal & Shaun Nelson @ Senet, Joe Knapp @ Semtech. 
 
April 2016 

