/*
  AnalogReadSerial

  Reads an analog input on pin 0, prints the result to the Serial Monitor.
  Graphical representation is available using Serial Plotter (Tools > Serial Plotter menu).
  Attach the center pin of a potentiometer to pin A0, and the outside pins to +5V and ground.

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/AnalogReadSerial
*/
#include <ESP8266WiFi.h>
const char* ssid       = "debagchi81a";
const char* password   = "Sep@2017Deb";
boolean isMovementDetected = false;
const int outPin = D4;
long stayOnForTime = 60000; //millisec
long curMillis = millis();
// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(115200);
  
  pinMode(outPin, OUTPUT);
  digitalWrite(outPin,HIGH);
  
  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  while ((WiFi.status() != WL_CONNECTED)) {
    delay(500);
    Serial.print(".");
  }
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println(" CONNECTED");
  }
}

// the loop routine runs over and over again forever:
void loop() {
  // read the input on analog pin 0:
  int sensorValue = analogRead(A0);
  // print out the value you read:
  Serial.println(sensorValue);
  if(sensorValue == 1024){
    curMillis = millis();
    if(millis() - curMillis <= stayOnForTime) {
      Serial.println("I am here......1");
      digitalWrite(outPin,LOW);
    }else{
      Serial.println("I am here......2");
      digitalWrite(outPin,HIGH);
    }
  }else{
    
  }
//  if(sensorValue == 1024){
//    isMovementDetected = true;
//  }else{
//    isMovementDetected = false;
//  }
//  if(isMovementDetected){
//    digitalWrite(outPin,LOW);
//  }else{
//    digitalWrite(outPin,HIGH);
//  }
  delay(1000);        // delay in between reads for stability
}
