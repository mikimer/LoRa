/* This sketch allows an Arduino to send data on a LoRa network 
 *   
 * Developed by Mike & Anna
 * .
*/
//create software-defined serial port on pins 10 & 11
//from: https://www.arduino.cc/en/Tutorial/SoftwareSerialExample
#include <SoftwareSerial.h>
SoftwareSerial mDotSerial(10, 11); // RX, TX
int blinkrate = 1000; // time between AT commands
int wait_time_to_join_network = 5000; // set delay to join the network
int mDotResetPin = A0;  // super important! Make sure A0 is connected to mDot pin 5!
String incomingString; // string coming from Serial
String mDotString; // string coming from mDot
void setup() {
  
  // allow for the power rails to power up 
  delay(2000);
  pinMode(10, INPUT);
  pinMode(11, OUTPUT);
  
  // reset the mDot with the reset pin (Arduino pin A0 to mDot pin 5)
  pinMode(mDotResetPin, OUTPUT);
  digitalWrite(mDotResetPin, HIGH);
  delay(500); 
  digitalWrite(mDotResetPin, LOW);
  delay(500); //send the reset signal for 500ms
  digitalWrite(mDotResetPin, HIGH);
  //have 2sec delay to let mDot completely reset
  delay(2000);  
  
  //configure the USB to the computer
  Serial.begin(9600); //initialize the serial port. Set baud rate to 115200, same as mDot firmware
  Serial.println("configured serial to home computer");
  //configure USB to the xbee/mDot
  mDotSerial.begin(115200);
  Serial.println("configured serial to mDot");
  //clear any existing outputs
  mDotSerial.flush();
  Serial.flush();
  //clear out whatever might have been in the receive buffer
  while (mDotSerial.available() != 0) mDotSerial.read();
  
  // get on the Senet network
  get_on_the_network();
   
} // end of setup
void loop() {
  // Communication : serial receive buffer to mDot
  while (Serial.available()) {
    delay(100);   
    incomingString = Serial.readString();
    Serial.print("Home sent: ");
    Serial.println(incomingString); 
    mDotSerial.println(incomingString);
 } // end while get home data
 // Communication : check mDot data 
 while (mDotSerial.available()) {
    mDotString = mDotSerial.readString();
    Serial.print("mDot response: ");
    Serial.println(mDotString);
    Serial.print("mDot check for ok: ");
    Serial.println(check_for_ok(),DEC);
    Serial.print("mDot check for ok 2: ");
    Serial.println(check_for_ok(),DEC);
 } // end while get mDot data
 delay(2000);
  
} // end of loop
// receive data from the mDot and see if there's an OK returned
int check_for_ok (){
  // Length (with one extra character for the null terminator)
  int str_len = mDotString.length() + 1;
  // Prepare the character array (the buffer) 
  char char_array[str_len];
  // Copy it over 
  mDotString.toCharArray(char_array, str_len);
  for (int i = 0; i < str_len + 1; i++){
    if(char_array[i] == 'O'){
      for(int j; j < str_len + 1 - i; j++){
        if(char_array[j] == 'K'){ return 1; }
      }
    // if(char_array[i+1] == 'O'){ return 1; } not working this way, I have no clue about what is happening
    }
  }
  return 0;
} // end check_for_ok
void get_on_the_network() {
  // disable the echo; it causes problems: http://www.activexperts.com/sms-component/at/commands/?at=E0
  mDotSerial.write("ATE0\r\n");
  delay(blinkrate);
  Serial.println("Sent ATEO");
  mDotString = mDotSerial.readString(); // read string from mDot
  Serial.print("mDot response: ");
  Serial.println(mDotString); // print mDot data
  Serial.print("checking for ok: ");
  Serial.println(check_for_ok(),DEC);
  Serial.println("");
  mDotSerial.write("ATV0\r\n");
  delay(blinkrate);
  Serial.println("Sent ATVO");
  mDotString = mDotSerial.readString(); // read string from mDot
  Serial.print("mDot response: ");
  Serial.println(mDotString); // print mDot data
  Serial.print("checking for ok: ");
  Serial.println(check_for_ok(),DEC);
  Serial.println("");
  
  // send the basic AT command "AT"
  mDotSerial.println("AT");
  Serial.println("Sent AT");
  mDotString = mDotSerial.readString(); // read string from mDot
  Serial.print("mDot response: ");
  Serial.println(mDotString); // print mDot data
  delay(blinkrate);
  Serial.print("checking for ok: ");
  Serial.println(check_for_ok(),DEC);
  Serial.println("");
  // set power to high = 20  
  mDotSerial.println("AT+TXP=20");
  delay(blinkrate);
  Serial.println("Sent AT+TXP");
  mDotString = mDotSerial.readString(); // read string from mDot
  Serial.print("mDot response: ");
  Serial.println(mDotString); // print mDot data
  Serial.print("checking for ok: ");
  Serial.println(check_for_ok(),DEC);
  Serial.println("");
  
  // get the device ID 
  mDotSerial.println("AT+DI");
  delay(blinkrate);
  Serial.println("Sent AT+DI");
  mDotString = mDotSerial.readString(); // read string from mDot
  Serial.print("mDot response: ");
  Serial.println(mDotString); // print mDot data
  Serial.println("");
  // Set a public network
  mDotSerial.println("AT+PN=1");
  delay(blinkrate);
  Serial.println("Sent AT+PN");
  mDotString = mDotSerial.readString(); // read string from mDot
  Serial.print("mDot response: ");
  Serial.println(mDotString); // print mDot data
  Serial.print("checking for ok: ");
  Serial.println(check_for_ok(),DEC);
  Serial.println("");
  //send the Senet network identifier. This is your unique APP EUI from the Senet dashboard  
  // Mike's orange account: 00:25:0C:00:00:01:00:01
  // Andrew's sutro account:   
  mDotSerial.println("AT+NI=0,00:25:0C:00:00:01:00:01");
  delay(blinkrate);
  Serial.println("Sent AT+NI");
  mDotString = mDotSerial.readString(); // read string from mDot
  Serial.print("mDot response: ");
  Serial.println(mDotString); // print mDot data
  Serial.print("checking for ok: ");
  Serial.println(check_for_ok(),DEC);
  Serial.println("");
  //send the app key that's unique to this mDot provided by Senet
  //this is the unique APP KEY from your Senet dashboard (click on pen & paper)  
  // mint on Anna's orange account    CB:2C:63:CB:C8:AC:F2:31:FB:E8:BF:55:35:27:89:CC
  // walnut on Mike's orange account  7E:74:B1:46:8D:5E:5E:0D:03:35:70:82:AC:E4:5B:04
  // grape on Mike's orange account   9B:77:EE:F7:F7:A8:51:91:8C:00:A1:40:62:C6:52:A5
  mDotSerial.println("AT+NK=0,CB:2C:63:CB:C8:AC:F2:31:FB:E8:BF:55:35:27:89:CC");
  delay(blinkrate);
  Serial.println("Sent AT+NK");
  mDotString = mDotSerial.readString(); // read string from mDot
  Serial.print("mDot response: ");
  Serial.println(mDotString); // print mDot data
  Serial.print("checking for ok: ");
  Serial.println(check_for_ok(),DEC);
  Serial.println("");
  //Join the network
  mDotSerial.println("AT+JOIN");
  delay(blinkrate);
  Serial.println("Sent AT+Join");
  delay(wait_time_to_join_network); //wait to join the network
  mDotString = mDotSerial.readString(); // read string from mDot
  Serial.print("mDot response: ");
  Serial.println(mDotString); // print mDot data
  Serial.print("checking for ok: ");
  Serial.println(check_for_ok(),DEC);
  Serial.print("checking for ok 2: ");
  Serial.println(check_for_ok(),DEC);
  Serial.println("");
  delay(5000);
  Serial.println("Start sending data:");
  
} // end of get_on_the_network
