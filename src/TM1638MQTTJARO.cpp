/*
TM1638 with MQTT 
sending buttons to topic
receiving message to display
*/
#include <Arduino.h>
#include <TM1638plus_Model2.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
// const char* ssid = "SSID";
// const char* password  = "PASSWORD";
// const char* mqtt_server = "MQTTSERVERIP";
#include "wifidata.h" 

#define  myTestDelay 5000
uint8_t  testcount = 0;
unsigned char buttonmerk;
bool jaromerk0 = false;
bool jaromerk1 = false;
bool jaromerk2 = false;
bool jaromerk3 = false;
bool jaromerk4 = false;
bool jaromerk5 = false;
bool jaromerk6 = false;
bool jaromerk7 = false;

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;


// GPIO I/O pins on the Arduino connected to strobe, clock, data, pick on any I/O pin you want.
#define  STROBE_TM D1  // strobe = GPIO connected to strobe line of module
#define  CLOCK_TM D2  // clock = GPIO connected to clock line of module
#define  DIO_TM D3 // data = GPIO connected to data line of module
bool swap_nibbles = false; //Default is false if left out, see note in readme at URL
bool high_freq = false; //default false,, If using a high freq CPU > ~100 MHZ set to true. 

// Constructor object
TM1638plus_Model2 tm(STROBE_TM, CLOCK_TM , DIO_TM, swap_nibbles, high_freq);

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  tm.DisplayStr("WIFI OK");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

// This is what happens on an incoming mqtt-message
void callback(char* topic, byte* payload, unsigned int length) {
  char message_buff[100];
  
  if (strcmp(topic,"TM1638/message")==0){
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (unsigned int i = 0; i < length; i++) {
    message_buff[i] = payload[i];
  }
  String msgString = String(message_buff);
  Serial.print(msgString);
  tm.DisplayStr(message_buff,0);
  }
  if (strcmp(topic,"TM1638/brightness")==0){
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
   for (unsigned int i = 0; i < length; i++) {
    message_buff[i] = payload[i];
  }
  String msgString = String(message_buff);
  Serial.print(msgString);
  tm.brightness(atoi(message_buff));
  }
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("TM1638/pub", "TM1638 ready.");
      // ... and resubscribe
     client.subscribe("TM1638/message");
     client.subscribe("TM1638/brightness");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

//Function to setup serial called from setup
void Serialinit()
{
  Serial.begin(115200);
  delay(100);
  Serial.println("-------------Comms UP------------");
}


void setup() {
  Serialinit();
  tm.displayBegin(); // Init the module
  delay(myTestDelay);

 // WLAN config
 setup_wifi();

 // MQTT
 client.setServer(mqtt_server, 1883);
 client.setCallback(callback);
  // reset display
 tm.reset();
}

void loop() {

if (!client.connected()) {
    reconnect();
  }
  client.loop();
  delay(200);
  unsigned char buttons = tm.ReadKey16();
  snprintf (msg, 50, "%ld", buttons);

  switch (buttons) {
  // Shutter 0
  // Button 1 & 2 = shutter 0
    case 1:
        if(jaromerk0 == true) { 
           client.publish("cmd/jarolift/shutter/0", "STOP");
           Serial.println("STOP 1");
           jaromerk0 = false;
        }
        else {
         client.publish("cmd/jarolift/shutter/0", "UP");
         Serial.println("UP 1");
         jaromerk0 = true;
        }
    break;
    case 2:
        if(jaromerk0 == true) { 
        client.publish("cmd/jarolift/shutter/0", "STOP");
        Serial.println("STOP 2");
        jaromerk0 = false;
        }
        else {
         client.publish("cmd/jarolift/shutter/0", "DOWN");
         Serial.println("DOWN 2");
         jaromerk1 = true;
        }
    break;
    // Shutter 1
    // Button 3 & 4 = shutter 1
    case 3:
        if(jaromerk1 == true) { 
           client.publish("cmd/jarolift/shutter/1", "STOP");
           Serial.println("STOP 3");
           jaromerk1 = false;
        }
        else {
         client.publish("cmd/jarolift/shutter/1", "UP");
         Serial.println("UP 3");
         jaromerk1 = true;
        }
    break;
    case 4:
        if(jaromerk1 == true) { 
        client.publish("cmd/jarolift/shutter/1", "STOP");
        Serial.println("STOP 4");
        jaromerk1 = false;
        }
        else {
         client.publish("cmd/jarolift/shutter/1", "DOWN");
         Serial.println("DOWN 4");
         jaromerk1 = true;
        }
    break;
    // Shutter 2
    // Button 5 & 6 = shutter 2
     case 5:
        if(jaromerk2 == true) { 
           client.publish("cmd/jarolift/shutter/2", "STOP");
           Serial.println("STOP 4");
           jaromerk2 = false;
        }
        else {
         client.publish("cmd/jarolift/shutter/2", "UP");
         Serial.println("UP 3");
         jaromerk2 = true;
        }
    break;
    case 6:
        if(jaromerk2 == true) { 
        client.publish("cmd/jarolift/shutter/2", "STOP");
        Serial.println("STOP 4");
        jaromerk2 = false;
        }
        else {
         client.publish("cmd/jarolift/shutter/2", "DOWN");
         Serial.println("DOWN 4");
         jaromerk2 = true;
        }
    break;
    // Shutter 3
    // Button 7 & 8 = shutter 3
     case 7:
        if(jaromerk3 == true) { 
           client.publish("cmd/jarolift/shutter/3", "STOP");
           Serial.println("STOP 7");
           jaromerk3 = false;
        }
        else {
         client.publish("cmd/jarolift/shutter/3", "UP");
         Serial.println("UP 7");
         jaromerk3 = true;
        }
    break;
    case 8:
        if(jaromerk3 == true) { 
        client.publish("cmd/jarolift/shutter/3", "STOP");
        Serial.println("STOP 8");
        jaromerk3 = false;
        }
        else {
         client.publish("cmd/jarolift/shutter/3", "DOWN");
         Serial.println("DOWN 8");
         jaromerk3 = true;
        }
    break;
    // Shutter 4
    // Button 9 & 10 = shutter 4
     case 9:
        if(jaromerk4 == true) { 
           client.publish("cmd/jarolift/shutter/4", "STOP");
           Serial.println("STOP 8");
           jaromerk4 = false;
        }
        else {
         client.publish("cmd/jarolift/shutter/4", "UP");
         Serial.println("UP 8");
         jaromerk4 = true;
        }
    break;
    case 10:
        if(jaromerk4 == true) { 
        client.publish("cmd/jarolift/shutter/4", "STOP");
        Serial.println("STOP 10");
        jaromerk4 = false;
        }
        else {
         client.publish("cmd/jarolift/shutter/4", "DOWN");
         Serial.println("DOWN 10");
         jaromerk4 = true;
        }
    break;
    // Shutter 5
    /* 
    // Button 11 & 12 = shutter 5
     case 11:
        if(jaromerk5 == true) { 
           client.publish("cmd/jarolift/shutter/5", "STOP");
           Serial.println("STOP 11");
           jaromerk5 = false;
        }
        else {
         client.publish("cmd/jarolift/shutter/5", "UP");
         Serial.println("UP 11");
         jaromerk5 = true;
        }
    break;
    case 12:
        if(jaromerk5 == true) { 
        client.publish("cmd/jarolift/shutter/5", "STOP");
        Serial.println("STOP 12");
        jaromerk5 = false;
        }
        else {
         client.publish("cmd/jarolift/shutter/5", "DOWN");
         Serial.println("DOWN 12");
         jaromerk5 = true;
        }
    break;
    */
  }
  
  if ((buttons != buttonmerk) & (buttons != 0)) {
  tm.DisplayDecNum(buttons, 0, false);
  Serial.println(msg);
  // 
  
  client.publish("TM1638/button", msg);
  }
  buttonmerk = buttons;

}
