
String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete

#include <SoftwareSerial.h>         // Include software serial to communicate with mDot
SoftwareSerial mDotSerial(10, 11);  // The Arduino receive (Rx) is on pin 10; transmit (tx) is on pin 11. 

// Declare pins
const int mDotResetPin = A0;        // Important: Ensure Arduino A0 is connected to mDot pin 5
const int click_sensor = A1;        // push-button sensor, digital (binary) values  
const int sound_sensor = A2;        // sound sensor , digital (binary) values
const int light_sensor = A3;        // light sensor, analog (0-1023) values  

// Declare variables
#define time_threshold 5     // if count = 900 seconds = 15mins, then send data
#define click_threshold 2     // if click = 10 clicks, then send data
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


//--------------------------------------------------------------------------------------

void setup() {

  //  configure the input sensors 
  pinMode(click_sensor, INPUT);
  pinMode(sound_sensor, INPUT);
  pinMode(light_sensor, INPUT);
  
}

//--------------------------------------------------------------------------------------

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

    Serial.println("sending stuff.. but not really");
    Serial.println(LoRa_payload);

    //reset values and counters
    count = 0;
    count_tenths = 0;
    stored_click_value = 0;                
    stored_sound_value = 0;                
    stored_light_value = 0;       
    Serial.println("reset values & counters");    
       
  } // end send payload



}



//--------------------------------------------------------------------------------------

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

