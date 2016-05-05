
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
 * so you need to ensure that the mDot baud rate is 9600: 
 * https://github.com/mikimer/LoRa/ 
 * To upload the firmware, you'll need the MultiTech Micro Developer Kit (MTMDK) to change the mDot firmware: 
 * http://www.multitech.com/models/94558020LF
*/

// *** Select the current mDot **
// Update the mDot data below to reflect the name, network key, and network identifier your mDot(s) 
// You need to adjust the comments in the code to select the mDot you're currently using
/* AAA with ID ending in 11:22 */       const String mDot_name = "AAA"; const String Network_key =  "11:22:33:44:55:11:22:33:44:55:11:22:33:44:11:22"; const String Network_ID = "11:22:33:44:55"; 
/* BBB with ID ending in 33:44 */    // const String mDot_name = "BBB"; const String Network_key = "11:22:33:44:55:11:22:33:44:55:11:22:33:44:33:44"; const String Network_ID = "11:22:33:44:55"; 
/* CCC with ID ending in 55:66 */    // const String mDot_name = "CCC"; const String Network_key = "11:22:33:44:55:11:22:33:44:55:11:22:33:44:55:66"; const String Network_ID = "11:22:33:44:55"; 

//create software-defined serial port on pins 10 & 11
//from: https://www.arduino.cc/en/Tutorial/SoftwareSerialExample
#include <SoftwareSerial.h>         // Include software serial to communicate with mDot 

// Declare pins
const int mDotResetPin = A0;        // Important: Ensure Arduino A0 is connected to mDot pin 5
const int click_sensor = A1;        // push-button sensor, digital (binary) values  
const int sound_sensor = A2;        // sound sensor , digital (binary) values
const int light_sensor = A3;        // light sensor, analog (0-1023) values  

// Declare variables
#define time_threshold 900     // if count = 900 seconds = 15mins, then send data
#define click_threshold 10     // if click = 10 clicks, then send data
#define lora_join_threshold 6  // max number of tries to join the lora network
int click_value = 0;           // current push button value
int sound_value = 0;           // current sound value
int light_value = 0;           // current light value
int stored_click_value = 0;    // stored push-button for sending
int stored_sound_value = 0;    // stored sound button for sending  
int stored_light_value = 0;    // stored light button for sending  
int light_values_array[10];    // array to store light values during 1s = 10 samples x 100ms/sample
int sum_light_value = 0;       // sum light values
unsigned int i;                // counter variable 
unsigned int x;                // temp variable
String mDotString;             // variable to hold mDot response
unsigned int count;            // count (in seconds) how many times we've read the input
unsigned int count_tenths = 0; // tenths of a second on the count (we need this since we sample every 0.1sec and count is an integer)
String LoRa_payload = "";      // the LoRa data payload. In the Senet portal, this appears as the PDU (Packet Data Unit). Limited to 50 ascii characters
int last_click_value = 0;      // store the last state of the clicker to ensure the button toggled on and off
int last_sound_value = 0;      // store the last state of the sound to ensure the sound toggled on and off

// AT commands to stop echo, set transmit power to 20 (maximum), and set the public network
#define GenericATcommand_count 4
const String GenericATcommands[GenericATcommand_count] = {"ATE0", "ATV0", "AT+TXP=20","AT+PN=1"};


void setup() {
  //  configure the input sensors 
  pinMode(click_sensor, INPUT);
  pinMode(sound_sensor, INPUT);
  pinMode(light_sensor, INPUT);
  
  // configure the Arduino serial port (D0,D1) to your computer (the Serial Monitor) 
  Serial.begin(115200);
  Serial.println("");
  Serial.println("  ***  This is a quickstart project to send sensor data to the internet via LoRa ***  ");
  delay(500);
  Serial.println("       Courtesy of the IoT Studio by Orange. http://orangeiotstudio.com/");
  Serial.println("");
   
  delay(500);  // allow for the rails to power up so that the mDot is running properly 
  Serial.print("This sketch is for mDot '");
  Serial.print(mDot_name);
  Serial.println("' on the Senet LoRa network. If your device or network are different, please edit the sketch");
  Serial.println("Arduino is trying to communicate with the mDot LoRa node...");
  // reset the mDot 
  pinMode(mDotResetPin, OUTPUT);
  digitalWrite(mDotResetPin, HIGH);
  delay(200); 
  digitalWrite(mDotResetPin, LOW); //send the LOW signal to reset
  delay(200); 
  digitalWrite(mDotResetPin, HIGH);
  delay(500); //delay to let mDot reset 
   
  // configure Arduino Serial1 to mDot
  Serial1.begin(115200); //initialize the serial port. 
  delay(100);
  
  //clear any existing outputs and clear out whatever might have been in the mDot receive buffer
  Serial.flush();
  Serial1.flush();
  while (Serial1.available() != 0) Serial1.read();

  // issue the generic AT commands
  int successful_attempt = 0; // set counter
  for (i=0; i<GenericATcommand_count; i++) {
    Serial1.println(GenericATcommands[i]);  // issue the AT command 
    delay(100);  // it is important to have a delay after every interaction between the Arduino & mDot
    while (Serial1.available()) { mDotString = Serial1.readString(); }; // get the mDot's response
    delay (100);
    if ( Validate (mDotString) ) {  // validate the mDot's response
      Serial.print(".");  // Print a single '.' to show that the Arduino successfully sent the AT command 
      successful_attempt++;
    }
  };// end for loop for issuing generic AT commands 

  //update user on program status
  Serial.println("");
  if (successful_attempt == 0){
    Serial.println("  Error: The Arduino couldn't send AT commands to the mDot. That's all we know.");
  }

  Serial.println("Trying to join the LoRa network...");
  // issue the mDot-specific AT commands
    Serial1.print("AT+NI=0,");   // Set Network Identifier for Senet account
    Serial1.println(Network_ID);   
    delay(100); 
    while (Serial1.available()) { mDotString = Serial1.readString(); }; // get the mDot's response
    delay (100);
   if ( Validate(mDotString) ) {  // validate the mDot's response
      Serial.print("."); } // Print a single '.' to show that this is working 
     
    Serial1.print("AT+NK=0,");   // Set Network Key for mDot
    Serial1.println(Network_key);   
    delay(100);
    while (Serial1.available()) { mDotString = Serial1.readString(); }; // get the mDot's response
    if ( Validate (mDotString) ) {  // validate the mDot's response
      Serial.print("."); // Print a single '.' to show that this is working 
    }
    
    // try to join the network three times
    successful_attempt = 1;  // use this as a counter and a flag
    while ( (successful_attempt < lora_join_threshold) && (successful_attempt != 100) ) {
      Serial1.println("AT+JOIN"); 
      delay(1000 * successful_attempt); // make the delay longer with each attempt to join network  
      while (Serial1.available()) { mDotString = Serial1.readString(); }; // get the mDot's response
      if ( Validate (mDotString) ) {  // validate the mDot's response
        successful_attempt = 100;  // set a flag for successfully joining the network
        Serial.print("."); // Print a single '.' to show that this is working   
      }
      else {
        successful_attempt++;  // if unsuccessful, increment the counter
      }        
    }// end while 
  //update user on program status
  Serial.println("");
  if (successful_attempt == 100){
    Serial.println("Successfully joined the LoRa network.");
  }
  else {
    Serial.println("  Error: Couldn't join the LoRa network. That's all we know.");
  }
  Serial.println("Arduino is beginning to read the sensors and send data...");

  count = time_threshold - 20; // set count to just before the time threshold so that Arduino quickly sends a data payload

} // end setup()


// The loop() function is designed to read the input sensor continuously (every 100ms), 
// but only send data after exceeding a critical threshold, which is important because LoRa requires low data rates. 
void loop() {
  // log the last reading, get a new sensor reading every 0.1 sec (100 ms) and increment the count and count_tenths
  delay(100);              // 0.1sec delay
  sensor_input_value();    // read the sensors & load the payload into LoRa_payload 
  count_tenths++;          // every loop increment count_tenths to track each 0.1 sec
  if (count_tenths > 9) {  // after 10 increments of count_tenths (1.0 sec), increment count and reset count_tenths
    count_tenths = 0;
    count++;
  }
  
  // send LoRa_payload to the internet via LoRa when we exceed the time_threshold (15mins=900sec) or click_threshold = 20 clicks 
  if ( (count >= time_threshold) || ( stored_click_value >= click_threshold) ) {            
    // send payload
    Serial1.print("AT+SEND ");    
    Serial1.println(LoRa_payload);  // LoRa_payload is loaded in sensor_input_value() 
    delay(100);
    while (Serial1.available()) { mDotString = Serial1.readString(); }; // get the mDot's response
    if ( Validate (mDotString) ) {  // validate the mDot's response
      Serial.print("SENDING sensor values: ");
      Serial.println(LoRa_payload);
    } 
    else {
      Serial.print("  Error: Couldn't send current reading via your node ");
      Serial.print(mDot_name);
      Serial.println(". That's all we know.");    
    }

  //reset values and counters
  count = 0;
  count_tenths = 0;
  stored_click_value = 0;                
  stored_sound_value = 0;                
  stored_light_value = 0;       
  Serial.println("reset values & counters");    
       
  } // end send payload

 
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

// This function is for reading the input values from the sensors 
// Adjust this however you need to make your sensor(s) work.
// The function needs to return whether there was a signficant change (true) or not (false) in input values.
void sensor_input_value() {
  // get new values
  click_value = digitalRead(click_sensor); 
  sound_value = digitalRead(sound_sensor);   
  light_value = analogRead(light_sensor);   

  // look for a click. We need to ensure that it clicks on and then off, otherwise holding the button breaks the algorithm
  if ( click_value == 1 && last_click_value == 0) {  // if click = 1 and last_click = 0, then count the click
    Serial.println(" CLICK detected.");
    stored_click_value++;
    last_click_value = 1;
  }
  else if ( click_value == 0) { last_click_value = 0; }; // if click is 0, then store it in last_click; otherwise leave last_click at 1
  
  // look for a sound
  if ( sound_value > 0 && last_sound_value == 0 ) { //ensure that the sound detection has settled back down to 0
    Serial.println(" SOUND detected.");
    stored_sound_value++;
    last_sound_value = 1;
  }
  else if ( sound_value == 0) { last_sound_value = 0; }; // if sound is 0, then store it in last_sound_value; otherwise leave last_sound_value at 1
  

  // get the current light value 
  light_values_array[count_tenths] = light_value;

  // store average light after 1sec  
  if( count_tenths == 9 ){
    // sum the light values 
    sum_light_value = 0;
    for( int i = 0; i < 10; i++ ){  // sum the light values     
      sum_light_value += light_values_array[i];
    }
    
   stored_light_value = sum_light_value / 10;
  } // end light value storage
  
  // creating a string with the data payload, per https://www.arduino.cc/en/Tutorial/StringAdditionOperator
  // we're wrapping the data in (), [], and {} to use the REGEXEXTRACT() function in gdocs.
  // Note that if your LoRa signal is weak, then your payload size might be limited to 11 bytes, the minimal payload.
  // http://www.multitech.net/developer/software/lora/introduction-to-lora/
  String stringone = "clicks(";
  String stringtwo = ")sounds[";
  String stringthree = "]avg_light{";
  String stringfour = "}";
  LoRa_payload = stringone + stored_click_value + stringtwo + stored_sound_value + stringthree + stored_light_value + stringfour;

} // end sensor_input_value()
