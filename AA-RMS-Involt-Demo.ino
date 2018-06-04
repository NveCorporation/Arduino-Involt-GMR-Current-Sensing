/*
  INVOLT BASIC ARDUINO SKETCH
  by Ernest Warzocha 2016
  ------------------------------------------------------
  This file is for serial communication between Arduino 
  Uno and Involt App. It can be used with Bluetooth 2.0
  device connected via hardware serial.
*/

/*
  involtPin array contains values received from app.
  Each UI element refers to pin number which is index of
  this array. involtString is array for values received
  with "S" pin. You can increase the length of array to
  store more values then arduino total pins. Use them 
  in sketch for not only pin-to-pin communication.
  In this case, the 15th array position is used for RMS.
*/
int    involtPin[15] = {}; //equals involt.pin.P in app
String involtString[2] ={}; //equals involt.pin.S in app

/*
  Buffer for received data. If you plan to receive more 
  at once just increase the array length.
*/
char involt[16];

/*
  String for responding to function received from app.
*/
String fname;

//Measurement variables
float Arms2 = 0.0; //RMS current squared
const int m = 250; //Averaging constant
#include <TimerOne.h> //Library routine for 10-bit PWM resolution 

void setup() {
  Serial.begin(57600);  //Connection to Involt app.
  Timer1.initialize(1000); //Set pwm timer for 1 ms period
}
void loop() {
  Arms2 = Arms2+(sq(float(analogRead(A1)-analogRead(A0)))-Arms2)/m; //Running-average RMS
  Timer1.pwm(9, sqrt(Arms2)); //Scale and send RMS voltage to pin 9
  involtSend(14, sqrt(Arms2)); //Send RMS data to Involt app.
}

/*
  INVOLT FUNCTIONS
  ------------------------------------------------------

  involtReceive
  ------------------------------------------------------ 
  read the data from app and parse the values received 
  into proper array. The read until is used due to
  issues with missing chars when reading direct strings.
  
  involtSend, involtSendString
  ------------------------------------------------------
  send int or string to app. Multiple prints are to
  reduce the sketch size (compared to sprintf()).

  involtSendFunction
  ------------------------------------------------------
  send function name to trigger it in app.
*/

void involtReceive(){
  if(Serial.available()>0) {
    Serial.readBytesUntil('\n',involt,sizeof(involt));
    int pin;
    if (involt[0] == 'P'){
      int value;
      sscanf(involt, "P%dV%d", &pin, &value);
      involtPin[pin] = value;
    }
    else if (involt[0] == 'S'){
      char value[sizeof(involt)];
      sscanf(involt, "S%dV%s", &pin, &value);
      involtString[pin] = value;
    }
    else if (involt[0] == 'F'){
      char value[sizeof(involt)];
      sscanf(involt, "F%s", &value);
      fname = value;
    };
    memset(involt,0,sizeof(involt));
  };
};

void involtSend(int pinNumber, int sendValue){
  Serial.print('A'); 
  Serial.print(pinNumber); 
  Serial.print('V'); 
  Serial.print(sendValue); 
  Serial.println('E');
  Serial.flush();
};

void involtSendString(int pinNumber, String sendString){
  Serial.print('A'); 
  Serial.print(pinNumber); 
  Serial.print('V'); 
  Serial.print(sendString); 
  Serial.println('E'); 
  Serial.flush();

};

void involtSendFunction(String functionName){
  Serial.print('F'); 
  Serial.print(functionName); 
  Serial.println('E'); 
  Serial.flush();
};

