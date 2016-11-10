/*  
 * This sketch gathers sensor data to demonstrate *LoRa* by indicating activities in the kitchen:
 *   1. Click. Users can choose to push a button.
 *   2. Sound. The sound sensor triggers when people are talking or making lots of noise.
 *   3. Light. The light sensor shows when the lights come on, i.e., working hours.  
 *   
 * To reduce the dynamic memory of this sketch, reduce or remove the text in the Serial.print() statements.  
 * 
 * Thanks to: Dave Kjendal & Shaun Nelson @ Senet, Joe Knapp @ Semtech
 * 
 * by Mike Vladimer & Anna Aflalo
 * at Orange Silicon Valley http://www.orangesv.com/
 * March 2016
 *  
 * We welcome feedback via twitter: @mikevladimer @anna_aflalo
 */

// How to prepare the mDot LoRa node
/* The mDot default baud rate is 115200 but the Arduino & mDot have trouble communicating so quickly
 * so you need to ensure that the mDot baud rate is 9600. Here's the firmware .bin file: 
 * https://github.com/mikimer/LoRa/blob/master/mDot_for_Arduino9600_MTS_MDOT_F411RE.bin  
 * To upload the firmware, you'll need the MultiTech Micro Developer Kit (MTMDK) to change the mDot firmware: 
 * http://www.multitech.com/models/94558020LF
*/

// *** Select the current mDot **
// Update the mDot data below to reflect your mDot name, network key, and network identifier 
// You can adjust the comments in the code to select the mDot you're currently using
/* GRAPE with ID ending in A3:20 */   const String mDot_name = "GRAPE"; const String Network_key = "9B:77:EE:F7:F7:A8:51:91:8C:00:A1:40:62:C6:52:A5"; const String Network_ID = "00:25:0C:00:00:01:00:01"; 
/* AAA with ID ending in 11:22 */   // const String mDot_name = "AAA"; const String Network_key =  "11:22:33:44:55:11:22:33:44:55:11:22:33:44:11:22"; const String Network_ID = "11:22:33:44:55"; 
/* BBB with ID ending in 33:44 */    // const String mDot_name = "BBB"; const String Network_key = "11:22:33:44:55:11:22:33:44:55:11:22:33:44:33:44"; const String Network_ID = "11:22:33:44:55"; 
/* CCC with ID ending in 55:66 */    // const String mDot_name = "CCC"; const String Network_key = "11:22:33:44:55:11:22:33:44:55:11:22:33:44:55:66"; const String Network_ID = "11:22:33:44:55"; 

//create software-defined serial port on pins 10 & 11
//from: https://www.arduino.cc/en/Tutorial/SoftwareSerialExample
#include <SoftwareSerial.h>         // Include software serial to communicate with mDot
SoftwareSerial mDotSerial(10, 11);  // The Arduino receive (Rx) is on pin 10; transmit (tx) is on pin 11. 
#include <math.h>                   // For exponent, pow(), and absolute value, abs().

// Declare pins
const int mDotResetPin = A0;        // Important: Ensure Arduino A0 is connected to mDot pin 5!
const int click_sensor = A1;        // push-button sensor, digital (binary) values  
const int sound_sensor = A2;        // sound sensor , digital (binary) values
const int light_sensor = A3;        // light sensor, analog (0-1023) values  

// Declare variables
int click_value = 0;           // current push button value
int sound_value = 0;           // current sound value
int light_value = 0;           // current light value
int stored_click_value = 0;    // stored push-button for sending
int stored_sound_value = 0;    // stored sound button for sending  
int stored_light_value = 0;    // stored light button for sending  
int light_variation = 0;       // biggest light variation within 1s
#define total_tenths_of_a_second 10
int light_values_array[total_tenths_of_a_second];    // array to store light values during 1s = 10 samples x 100ms/sample
int min_light_value = 1500;    // min light value
int max_light_value = 0;       // max light value
int sum_light_value = 0;       // sum light values
int average_light_value = 0;   // average light values
int light_treshold = 150;      // light values comparaison treshhold
unsigned int i;                // counter variable 
unsigned int x;                // temp variable
String mDotString;             // variable to hold mDot response
unsigned int count = 0;        // count (in seconds) how many times we've read the input
unsigned int count_tenths = 0; // tenths of a second on the count (we need this since we sample every 0.1sec and count is an integer)
bool threshold_exceeded;       // track whether one of the sensors exceed a threshold, which triggers the Arduino to send data
String LoRa_payload;        // the LoRa data payload. In the Senet portal, this appears as the PDU (Packet Data Unit). Limited to 50 ascii characters
#define count_start_exp 5      // start sending data when count = 2 ^ count_start_exp; 2^5 = 32 seconds
#define count_reset_exp 12     // when count = 2 ^ count_reset_exp, then reset count = 0; 2^12 = 4,096 seconds ~ 3,600 seconds = 1 hour

// AT commands to stop echo, set transmit power to 20 (maximum), and set the public network
#define GenericATcommand_count 4
const String GenericATcommands[GenericATcommand_count] = {"ATE0", "ATV0", "AT+TXP=20","AT+PN=1"};


void setup() {
  //  configure the input sensors 
  pinMode(click_sensor, INPUT);
  pinMode(sound_sensor, INPUT);
  pinMode(light_sensor, INPUT);
  
  // configure the Arduino serial port (D0,D1) to your computer (the Serial Monitor) 
  Serial.begin(9600); 
  Serial.println("");
  Serial.println("  ***  This is a quickstart project to send sensor data to the internet via LoRa ***  ");
  delay(500);
  Serial.println("       Courtesy of the IoT Studio by Orange. http://orangeiotstudio.com/");
  Serial.println("");
   
  delay(500);  // allow for the rails to power up so that the mDot is running properly 
  Serial.print("This sketch works with mDot named '");
  Serial.print(mDot_name);
  Serial.println("' on the Senet LoRa network. If your mDot or network different, please edit the Arduino sketch");
  Serial.println("Arduino is trying to communicate with the mDot LoRa node...");
  // reset the mDot 
  pinMode(mDotResetPin, OUTPUT);
  digitalWrite(mDotResetPin, HIGH);
  delay(200); 
  digitalWrite(mDotResetPin, LOW); //send the LOW signal to reset
  delay(200); 
  digitalWrite(mDotResetPin, HIGH);
  delay(500); //delay to let mDot reset 
   
  // configure Arduino Serial to mDot
  mDotSerial.begin(9600); //initialize the serial port. 
  delay(100);

  //clear any existing outputs and clear out whatever might have been in the mDot receive buffer
  Serial.flush();
  mDotSerial.flush();
  while (mDotSerial.available() != 0) mDotSerial.read();

  // issue the generic AT commands
  int successful_attempt = 0; // set counter
  for (i=0; i<GenericATcommand_count; i++) {
    mDotSerial.println(GenericATcommands[i]);  // issue the AT command 
    delay(100);  // it is important to have a delay after every interaction between the Arduino & mDot
    while (mDotSerial.available()) { mDotString = mDotSerial.readString(); }; // get the mDot's response
    delay (100);
    if ( Validate (mDotString) ) {  // validate the mDot's response
      Serial.print(".");  // Print a single '.' to show that the Arduino successfully sent the AT command 
      successful_attempt++;
    }
  };// end for loop for issuing generic AT commands 

  //update user on program status
  Serial.println("");
  if (successful_attempt = 0){
    Serial.println("  Error: The Arduino couldn't send AT commands to the mDot. That's all we know.");
  }

  Serial.println("Trying to join the LoRa network...");
  // issue the mDot-specific AT commands
    mDotSerial.print("AT+NI=0,");   // Set Network Identifier for Senet account
    mDotSerial.println(Network_ID);   
    delay(100); 
    while (mDotSerial.available()) { mDotString = mDotSerial.readString(); }; // get the mDot's response
    delay (100);
   if ( Validate(mDotString) ) {  // validate the mDot's response
      Serial.print("."); } // Print a single '.' to show that this is working 
     
    mDotSerial.print("AT+NK=0,");   // Set Network Key for mDot
    mDotSerial.println(Network_key);   
    delay(100);
    while (mDotSerial.available()) { mDotString = mDotSerial.readString(); }; // get the mDot's response
    if ( Validate (mDotString) ) {  // validate the mDot's response
      Serial.println("."); // Print a single '.' to show that this is working 
    }
    
    // try to join the network three times
    successful_attempt = 1;  // use this as a counter and a flag
    while (successful_attempt < 4) {
      mDotSerial.println("AT+JOIN"); 
      delay(1000 * successful_attempt); // make the delay longer with each attempt to join network  
      while (mDotSerial.available()) { mDotString = mDotSerial.readString(); }; // get the mDot's response
      if ( Validate (mDotString) ) {  // validate the mDot's response
        successful_attempt = 10;  // set a flag for successfully joining the network
        Serial.print("."); // Print a single '.' to show that this is working   
      }
      else {
        successful_attempt++;  // if unsuccessful, increment the counter
      }        
    }
  //update user on program status
  Serial.println("");
  if (successful_attempt == 10){
    Serial.println("Successfully joined the LoRa network.");
  }
  else {
    Serial.println("  Error: Couldn't join the LoRa network. That's all we know.");
  }
  Serial.println("Arduino is beginning to read the sensors and send data...");

} // end setup()


// The loop() function is designed to read the input sensor continuously (every 100ms), 
// but only send data via LoRa every 2^x seconds, starting at 32 sec (32 sec, 64, sec, 128 sec...). 
// This is important because LoRa is built for low data rates. 
// The loop() function resets after about an hour (60 mins x 60 sec = 3,600 sec ~ 4,096 sec = 2^12 sec)
// unless there is a significant change in the input value (+/- threshold value from last_reading).
void loop() {
  // log the last reading, get a new sensor reading every 0.1 sec (100 ms) and increment the count and count_tenths
  delay(100);  
  threshold_exceeded = sensor_input_value(); // read the sensors, load the payload into LoRa_payload, and return whether threshold exceeded 
  count_tenths++;  // every loop increment count_tenths to track each 0.1 sec
  if (count_tenths > 9) {  // after 10 increments of count_tenths (1.0 sec), increment count and reset count_tenths
    count_tenths = 0;
    count++;
  }
  
  // send LoRa_payload to the internet via LoRa when the count is 2^n (32 sec, 64 sec...)
  // check 2^n from 2^5 = 32sec (count_start_exp) to 2^12 = 4,096sec (count_reset_exp)
  for (i = count_start_exp; i <= count_reset_exp; i++) {
    x = 0.5 + pow(2,i); // x = 2^i; the 0.5 fixes the problem between int and float, from: http://forum.arduino.cc/index.php?topic=2392.0
    if ( (count == x && count_tenths == 0) ) {            // look at count only at 2.0^n seconds
      if ( threshold_exceeded == false ){      // wait until 2^5 to send data if no change
        Serial.print("No change detected after ");       
        Serial.print(count); 
        Serial.println(" seconds. "); 
      } 
      if ( threshold_exceeded ){
        mDotSerial.print("AT+SEND ");    
        mDotSerial.println(LoRa_payload);  // LoRa_payload is loaded in sensor_input_value()
        stored_click_value = 0;               // reset to 0, no click yet
        stored_sound_value = 0;               // reset to 0, no sound yet
        stored_light_value = light_value;     // reset to current value 
        delay(100);
        while (mDotSerial.available()) { mDotString = mDotSerial.readString(); }; // get the mDot's response
        if ( Validate (mDotString) ) {  // validate the mDot's response
          Serial.print("SENDING sensor values: ");
          Serial.print(LoRa_payload);
          Serial.print(" after ");
          Serial.print(count); 
          Serial.println(" seconds. "); 
       } else {
        Serial.print("  Error: Couldn't send current reading via your node ");
        Serial.print(mDot_name);
        Serial.println(". That's all we know.");    
       }
     }
   }
 } // end for() to SEND LoRa data

  // reset the count to 900 seconds, if it's at the value 2^count_reset_exp. In this quickstart project that's ~1hr where count_reset_exp = 12 
  x = 0.5 + pow( 2, count_reset_exp ); // add 0.5 per: http://forum.arduino.cc/index.php?topic=2392.0
  if (count >= x) {
    count = 900;
    Serial.println("Counter timed out at 1 hour. Restarting counter.");
  }

   // reset count to 30sec if the change in reading exceeds threshold_percent AND it's been more than 65 seconds since last reset
   if ( (threshold_exceeded == true) && (count > 65) ) { // evaluated in sensor_input_value()   
   Serial.println("  Resetting timer: threshold exceed and >1min");
   count = 30;
   }
 
} // end loop()
   

// Determine whether mDot response is ok/success (TRUE) or error/unknown (FALSE)
bool Validate (String response) {
    if ( Contains(response, "OK") )  { return true; } // response =  OK 
    else if ( Contains(response, "Set Network") ) { return true; } // response = Set Network Key 
    else if ( Contains(response, "Success")) { return true; } // response = Successfully joined network       
    else {return false; };    
} // end Validate()


// Determine whether the "target" string contains the "search" string 
bool Contains(String target, String search) {
    // move through the 'target' string searching for the 'search' string. 
    int j;
    for (j = 0; j <= target.length() - search.length(); j++) {
        if (target.substring(j, j+search.length()) == search) {
          return true; // return true if the search string is in the target string 
        }; 
    }
    return false;  // otherwise return false
} // end Contains()

// This function is for reading the input value(s) from the sensor and determining significant changes. 
// Adjust this however you need to make your sensor(s) work.
// The function needs to return whether there was a signficant change (true) or not (false) in input values.
bool sensor_input_value() {
  // get new values
  click_value = digitalRead(click_sensor); 
  sound_value = digitalRead(sound_sensor);   
  light_value = analogRead(light_sensor);   

  // look for a click
  if ( click_value > 0 ) {
    Serial.println(" CLICK detected.");
    stored_click_value =  click_value;
  }
  
  // look for a sound
  if ( sound_value > 0 ) {
    Serial.println(" SOUND detected.");
    stored_sound_value =  sound_value;
  }

 // get the current light value 
 light_values_array[count_tenths] = light_value;

 // look for a light variation after 1sec (at 0.9ms)  
 if( count_tenths == 9 ){
   min_light_value = light_values_array[0]; // initialize the array min value 
   max_light_value = light_values_array[0]; // initialize the array max value
   sum_light_value = light_values_array[0]; // initialize the array sum value 
   // search for the min and max values within the array 
   // sum the light values 
   for( int i = 1; i < total_tenths_of_a_second; i++ ){  
     if ( light_values_array[i] < min_light_value){
       min_light_value = light_values_array[i];
     }
     if ( light_values_array[i] > max_light_value){
       max_light_value = light_values_array[i];
     }
     // sum the light values
     sum_light_value += light_values_array[i];
   }
  
   // calculate average of light values after 1sec
   average_light_value = sum_light_value / 10;
   
   light_variation = max_light_value - min_light_value;  // check if during the past 1sec a big light variation happenned  
   if ( light_variation > light_treshold ) {
   Serial.print( light_variation );
   Serial.println(" LIGHT change detected.");
   stored_light_value = average_light_value;      // if there was a big variation, send the latest value
   }
 }
  
  // creating a string with the data payload, per https://www.arduino.cc/en/Tutorial/StringAdditionOperator
  String stringone = "click";
  String stringtwo = ";sound";
  String stringthree = ";light";
  String stringfour = ";";
  LoRa_payload = stringone + stored_click_value + stringtwo + stored_sound_value + stringthree + stored_light_value + stringfour;

  if ( stored_click_value || stored_sound_value || light_variation > 100 ){
    return true;
  }

  return false; // if we've reached this line, then the new readings didn't exceed the threshold.  

} // end sensor_input_value()


