/******************************************************************************************
Reads an AA-Series analog magnetometer sensor, amplifies and corrects offset using an 
Arduino Uno. Sensor OUT+ on IO0 and OUT- on IO1. Amplified PWM output on IO3.
*******************************************************************************************/
int sensor = 0; //The sensor output

void setup() {
}

void loop() { 
  sensor = analogRead(0)-analogRead(1); //Read the sensor differential output voltage 
  //(OUT+ on IO0 and OUT- on IO1) 
  sensor = map(sensor, -2, 40, 0, 255); //Correct offset and scale to saturation level
  analogWrite(3, sensor); //Output the corrected sensor output to IO3
  delay(50);
}
