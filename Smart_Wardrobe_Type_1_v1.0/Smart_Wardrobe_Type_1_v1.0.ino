/**
   @dbagchi
   Compatable board : Nano V3.0
*/

//constant values
const boolean _DEBUG = true ; //flag to make debug statements on/off
const int _DELAY = 500; // delay after each cycle of sensor data reading
//const char MODE_AUTOMATIC = 'A'; // automatic mode
//const char MODE_MANUAL = 'M'; // manual mode



// +++++++++++++++++ PIN Setup +++++++++++++++++++++

// ************** INPUT ****************************
const byte photoSensor = A0;    // select the input pin for the common photo sensor 
const byte reedSensorL = 4;    // select the input pin for the reed sensor for LEFT wing
const byte reedSensorM = 5;    // select the input pin for the reed sensor for MIDDLE wing
const byte reedSensorRL = 6;    // select the input pin for the reed sensor for RIGHT wing Left door
const byte reedSensorRR = 7;    // select the input pin for the reed sensor for RIGHT wing Right door



// ************** OUTPUT ****************************
const byte lightL = 8;      // select the Light Output pin for the LEFT wing
const byte lightM = 9;      // select the Light Output pin for the MIDDLE wing
const byte lightRL = 10;      // select the Light Output pin for the RIGHT wing Left door
const byte lightRR = 11;      // select the Light Output pin for the RIGHT wing Right door

//variables used
int reedSensorValueL = 0;  // variable to store the value coming from the reed Sensor for LEFT wing
int reedSensorValueM = 0;  // variable to store the value coming from the reed Sensor for MIDDLE wing
int reedSensorValueRL = 0;  // variable to store the value coming from the reed Sensor for RIGHT wing Left door
int reedSensorValueRR = 0;  // variable to store the value coming from the reed Sensor for RIGHT wing Right door
int photoSensorValue = 0;  // variable to store the value coming from the photo Sensor


void setup() {
  // put your setup code here, to run once:
  if (_DEBUG) {
    Serial.begin(115200);
    Serial.println("Initializing --->");
  }

  pinMode(reedSensorL, INPUT_PULLUP);//used inversely active reed sensor,0=near,1=far.
  pinMode(reedSensorM, INPUT_PULLUP);//used inversely active reed sensor,0=near,1=far.
  pinMode(reedSensorRL, INPUT_PULLUP);//used inversely active reed sensor,0=near,1=far.
  pinMode(reedSensorRR, INPUT_PULLUP);//used inversely active reed sensor,0=near,1=far.

  pinMode(photoSensor, INPUT_PULLUP);//used inversely active photo sensor,0=near,1=far.


  pinMode(lightL, OUTPUT);
  digitalWrite(lightL, HIGH);//HIGH=OFF, LOW=ON

  pinMode(lightM, OUTPUT);
  digitalWrite(lightM, HIGH);//HIGH=OFF, LOW=ON

  pinMode(lightRL, OUTPUT);
  digitalWrite(lightRL, HIGH);//HIGH=OFF, LOW=ON

  pinMode(lightRR, OUTPUT);
  digitalWrite(lightRR, HIGH);//HIGH=OFF, LOW=ON

  if (_DEBUG) {
    Serial.println("Initializing complete.");
  }

}

void loop() {
  // put your main code here, to run repeatedly:
  if (_DEBUG) {
    Serial.println("START of new cycle of data reading--->");
  }

  // read photo sensor value
  photoSensorValue = digitalRead(photoSensor);

  if (_DEBUG) {
    Serial.print("photoSensorValue : ");
    Serial.print(photoSensorValue);
    Serial.println();
  }

  if (photoSensorValue ==  LOW) { // means photo sensor reads ample light, hence no need to bother for lighting up the wardrobe.
    // make sure all the inside lights are off

    if (_DEBUG) {
      Serial.println("****** There is ample light.No need to switch on the light. *******");
    }

    digitalWrite(lightL, HIGH);
    digitalWrite(lightM, HIGH);
    digitalWrite(lightRL, HIGH);
    digitalWrite(lightRR, HIGH);

  } else {                       // means photo sensor reads low light, hence go on reading values of reed sensors

    reedSensorValueL = digitalRead(reedSensorL);
    reedSensorValueM = digitalRead(reedSensorM);
    reedSensorValueRR = digitalRead(reedSensorRL);
    reedSensorValueRR = digitalRead(reedSensorRR);

    if (_DEBUG) {
      Serial.print("reedSensorValueL : ");
      Serial.print(reedSensorValueL);
      Serial.println();

      Serial.print("reedSensorValueM : ");
      Serial.print(reedSensorValueM);
      Serial.println();

      Serial.print("reedSensorValueRL : ");
      Serial.print(reedSensorValueRL);
      Serial.println();

      Serial.print("reedSensorValueRR : ");
      Serial.print(reedSensorValueRR);
      Serial.println();
    }

    if (reedSensorValueL == LOW) { // means door is closed hence switch off the light
      digitalWrite(lightL, HIGH);
    } else {                     // means door is open hence switch on the light
      digitalWrite(lightL, LOW);
    }

    if (reedSensorValueM == LOW) { // means door is closed hence switch off the light
      digitalWrite(lightM, HIGH);
    } else {                     // means door is open hence switch on the light
      digitalWrite(lightM, LOW);
    }

    if (reedSensorValueRL == LOW) { // means door is closed hence switch off the light
      digitalWrite(lightRL, HIGH);
    } else {                     // means door is open hence switch on the light
      digitalWrite(lightRL, LOW);
    }

    if (reedSensorValueRR == LOW) { // means door is closed hence switch off the light
      digitalWrite(lightRR, HIGH);
    } else {                     // means door is open hence switch on the light
      digitalWrite(lightRR, LOW);
    }

    if (_DEBUG) {
      Serial.println("****** Required operation is complete. *******");
    }

  }

  delay(_DELAY);

  if (_DEBUG) {
    Serial.println("END of new cycle of data reading--->");
  }

}
