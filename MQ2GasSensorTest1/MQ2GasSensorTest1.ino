const byte gasSensor = A0;    // select the input pin for the GAS SENSOR

//variables used
float gasSensorValue = 0;  // variable to store the value coming from the GAS SENSOR
void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);
  Serial.println("Initializing !!!");
  pinMode(gasSensor, INPUT);
  delay(1000);//3 mins wait for stabilizing coil
  Serial.println("Ready !!!");

}

void loop() {
  // put your main code here, to run repeatedly:
  gasSensorValue = analogRead(gasSensor);
  Serial.print("gasSensorValue:");
  Serial.println(gasSensorValue);

  delay(1000);

}
