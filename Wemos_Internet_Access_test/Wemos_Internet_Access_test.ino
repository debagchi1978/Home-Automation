///////////////////////////////////////////////////////////////
//     You can find the full tutorial for this project at    //
//               https://youtu.be/etAo4Hq-76M                //
//                Copyright Nick Bartzeliotis                //
//                        Have fun!                          //
///////////////////////////////////////////////////////////////


#include <ESP8266WiFi.h>

//const char *ssid = "debagchi_home_4G_hotspot";
//const char *password = "Jan@2016";
char *ssid      = "debagchi81a";               // Set you WiFi SSID
//char *ssid      = "debagchi81aExtender";               // Set you WiFi SSID
char *password  = "Sep@2017Deb";               // Set you WiFi password

const char* host = "www.google.com";




int led = LED_BUILTIN;

void setup() {

  Serial.begin(115200);
  Serial.println();
  //delay(500);
  pinMode(led, OUTPUT);
  digitalWrite(led, HIGH);
  Serial.println("Starting wifi");
  //delay(5000);
  WiFi.disconnect();
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  //delay(100);
  //Serial.println("Ending wifi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("WiFi connected");

}


void loop() {
  //wl_status_t wifiStatus = WiFi.status();
  //Serial.print("WiFi.status():");
  //Serial.println(wifiStatus);

  WiFiClient client;
  if (client.connect(host, 80))
  {
    Serial.println("connected");
    digitalWrite(led, LOW);
    client.stop();
    //delay(100);

  }
  else {
    Serial.println("connection failed!");
    digitalWrite(led, HIGH);
    client.stop();
  }
  delay(5000);

}



