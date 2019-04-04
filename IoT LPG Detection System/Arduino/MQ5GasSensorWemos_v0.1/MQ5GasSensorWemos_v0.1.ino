/**
   MQ5 Gas Sensor Module used for LPG detection
   ******************** Wemos version **************
   threshold value found = 600
   +++ Author : Debajyoti Bagchi (Digital Poriborton) +++

*/

#include <ArduinoOTA.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <IFTTTMaker.h>

#define KEY "eEeU-DkoxqyUus1JuiV1i_GGmOirqoxFJyweBukh8RO"  // Get it from this page https://ifttt.com/services/maker_webhooks/settings
#define EVENT_NAME_SYSTEM_RESTART_MAIL "system_boot"
#define EVENT_NAME_KITCHEN_GAS_ABNORMAL_MAIL "GAS_ABNORMAL"
#define EVENT_NAME_KITCHEN_GAS_NORMAL_MAIL "GAS_NORMAL"

//Not required
//const String SYSTEM_NAME = "LPG GAS MONITORING SYSTEM";
//const String SENSOR_LOCATION = "KITCHEN";

const byte gasSensorPin = A0;    // select the input pin for the GAS SENSOR
const byte redLedPin = D5;       // select the output pin for the redLedPin
const byte buzzerPin = D6;       // select the output pin for the buzzerPin
const byte greenLedPin = D7;       // select the output pin for the greenLedPin
//const byte blueLedPin = D8;       // select the output pin for the blueLedPin

const int threshold = 600;    // THRESHOLD value for detection
const long coilStabilizationDelay = 180000; //3 mins wait for stabilizing coil
const long gasLeakMailSendingDelay = 180000; //(3 mins)

const char *ssid      = "debagchi81a"; // Set your WiFi SSID
const char *password  = "Sep@2017Deb"; // Set your WiFi PASSWORD
const int MAX_wifiRetryCount = 10; //Maximum retry count for Wifi connectivity try
const char *uniqueHostName = "debIoTKitchenGasSensor";

//variables used
float gasSensorPinValue = 0;  // variable to store the value coming from the GAS SENSOR
boolean gasLeakMailSendFlag = false;
boolean gasNormalMailSendToBeFlag = false;
long startGasLeakMailSend = millis();


void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200);
  Serial.println("");
  Serial.println("Initializing !!!");
  pinMode(gasSensorPin, INPUT);

  pinMode(greenLedPin, OUTPUT);
  digitalWrite(greenLedPin, HIGH);

  pinMode(redLedPin, OUTPUT);
  digitalWrite(redLedPin, LOW);

  //pinMode(blueLedPin, OUTPUT);
  //digitalWrite(blueLedPin, LOW);

  pinMode(buzzerPin, OUTPUT);
  digitalWrite(buzzerPin, LOW);

  setupOTA();
  connectWiFi();
  Serial.println("Stabilizing system. Please wait for 3 mins.");
  delay(coilStabilizationDelay);//3 mins wait for stabilizing coil
  //delay(1000);//for test--delete when tested
  sendSystemRestartMail();
  Serial.println("Ready !!!");

}

void loop() {
  ArduinoOTA.handle();
  //  if (WiFi.status() == WL_CONNECTED) {
  //    digitalWrite(blueLedPin, HIGH);
  //  } else {
  //    digitalWrite(blueLedPin, LOW);
  //  }
  digitalWrite(greenLedPin, HIGH);
  // put your main code here, to run repeatedly:
  gasSensorPinValue = analogRead(gasSensorPin);

  if (gasLeakMailSendFlag && (millis() - startGasLeakMailSend) >= gasLeakMailSendingDelay) {
    gasLeakMailSendFlag = false;//reset flag
  }
  Serial.print("gasSensorPinValue:");
  Serial.println(gasSensorPinValue);
  if (gasSensorPinValue > threshold ) {
    digitalWrite(redLedPin, HIGH);
    digitalWrite(buzzerPin, HIGH);
    if (!gasLeakMailSendFlag && connectWiFi()) {
      sendGasAbnormalMail();
      gasLeakMailSendFlag = true;
      gasNormalMailSendToBeFlag = true;
      startGasLeakMailSend = millis();
    }
  } else {
    digitalWrite(redLedPin, LOW);
    digitalWrite(buzzerPin, LOW);
    if (gasNormalMailSendToBeFlag) {
      gasNormalMailSendToBeFlag = false;
      sendGasNormalMail();
    }
  }

  delay(500);

  digitalWrite(greenLedPin, LOW);
  delay(500);
}

boolean connectWiFi() {
  //digitalWrite(blueLedPin, LOW);
  boolean isConnected = false;
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Already WiFi CONNECTED");
    //digitalWrite(blueLedPin, HIGH);
    isConnected = true;
    return isConnected;
  }
  WiFi.disconnect();
  Serial.print("Connecting to WiFi ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);

  WiFi.begin (ssid, password);
  int wifiRetryCount = 1;
  while ((WiFi.status() != WL_CONNECTED) && wifiRetryCount <= MAX_wifiRetryCount ) {
    //    digitalWrite(blueLedPin, HIGH);
    //    delay(500);
    //    digitalWrite(blueLedPin, LOW);
    //    delay(500);
    delay(500);
    Serial.print(".");
    wifiRetryCount++;
  }
  if (WiFi.status() == WL_CONNECTED) {
    isConnected = true;
    //digitalWrite(blueLedPin, HIGH);
    Serial.println(" CONNECTED");
  } else {
    isConnected = false;
    //digitalWrite(blueLedPin, LOW);
    Serial.println(" FAILED");
  }
  return isConnected;
}

void setupOTA() {
  // Port defaults to 8266
  ArduinoOTA.setPort(8266);

  // Hostname defaults to esp8266-[ChipID]
  ArduinoOTA.setHostname(uniqueHostName);

  // No authentication by default
  //ArduinoOTA.setPassword("admin");

  // Password can be set with it's md5 value as well
  // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
  //ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH)
      type = "sketch";
    else // U_SPIFFS
      type = "filesystem";

    // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
    Serial.println("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();

}

void sendSystemRestartMail() {
  WiFiClientSecure client_SYSTEM_RESTART_MAIL;
  IFTTTMaker ifttt_SYSTEM_RESTART_MAIL(KEY, client_SYSTEM_RESTART_MAIL);

  Serial.println();
  String value1 = uniqueHostName;
  String value2 = "";
  String value3 = "";

  //triggerEvent takes an Event Name and then you can optional pass in up to 3 extra Strings
  if (ifttt_SYSTEM_RESTART_MAIL.triggerEvent(EVENT_NAME_SYSTEM_RESTART_MAIL, value1, value2, value3)) {
    Serial.println("Successfully sent SYSTEM RESTART mail");
  } else
  {
    Serial.println("Failed sending SYSTEM RESTART mail!");
  }

  Serial.println();
}

void sendGasAbnormalMail() {
  WiFiClientSecure client_GAS_ABNORMAL_MAIL;
  IFTTTMaker ifttt_GAS_ABNORMAL_MAIL(KEY, client_GAS_ABNORMAL_MAIL);

  Serial.println();
  String value1 = uniqueHostName;
  String value2 = String(threshold);
  String value3 = String(gasSensorPinValue);

  //triggerEvent takes an Event Name and then you can optional pass in up to 3 extra Strings
  if (ifttt_GAS_ABNORMAL_MAIL.triggerEvent(EVENT_NAME_KITCHEN_GAS_ABNORMAL_MAIL, value1, value2, value3)) {
    Serial.println("Successfully sent gas ABNORMAL mail");
  } else
  {
    Serial.println("Failed sending gas ABNORMAL mail!");
  }

  Serial.println();
}

void sendGasNormalMail() {
  WiFiClientSecure client_GAS_NORMAL_MAIL;
  IFTTTMaker ifttt_GAS_NORMAL_MAIL(KEY, client_GAS_NORMAL_MAIL);

  Serial.println();
  String value1 = uniqueHostName;
  String value2 = String(threshold);
  String value3 = String(gasSensorPinValue);

  //triggerEvent takes an Event Name and then you can optional pass in up to 3 extra Strings
  if (ifttt_GAS_NORMAL_MAIL.triggerEvent(EVENT_NAME_KITCHEN_GAS_NORMAL_MAIL, value1, value2, value3)) {
    Serial.println("Successfully sent gas NORMAL mail");
  } else
  {
    Serial.println("Failed sending gas NORMAL mail!");
  }

  Serial.println();
}

