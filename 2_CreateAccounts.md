# Create your accounts
You'll need to create accounts with the following websites to get this project to work.  

## Senet
[Senet](https://portal.senetco.com/) is continuing to expand their public LoRa network in the United States. You'll need to create an account with Senet to view and access your data.  

[ ![Senet portal](assets/Senet_Development_Portal.png) ](https://portal.senetco.com/)  

## Zapier
[Zapier](https://zapier.com) automates the flow of data between Senet and Google Docs. For this project, we'll have Senet do an HTTP Post to a Zapier webhook. Zapier then puts the contents of Senet's JSON payload into a row in a Google Spreadsheet (gDocs). 

[ ![Zapier home](assets/Zapier_home.png) ](https://zapier.com/)

## Google Drive
You'll use [Google Spreadsheets](https://docs.google.com/spreadsheets/u/0/) to view and analyze your data in this project. Since you probably already have a Google Drive account, we highly recommend that you do all work on this project in new spreadsheets, as opposed to tabs in existing spreadsheets.  This safety measure will help ensure that if things go wrong, they don't affect your other projects. 

## ARM mbed (optional)
If you're buying your own equipment, then you'll need an [ARM mbed account](https://developer.mbed.org/account/login/?next=/teams/Senet/code/Senet-NAMote/) to configure the firmware for your Semtech NorAm LoRa mote to operate on the Senet LoRa network.  If you're using a kit from the Orange IoT Studio, then your mote will come pre-configured so that you do not need to install firmware yourself. 
