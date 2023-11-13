#include <M5Core2.h>
#include "EnvSensor.hpp"
#include "M5_ENV.h"
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <vector>
#include <WiFi.h>
#include <PubSubClient.h>
#include <string.h>
#define NBSENSOR_MAX 16



const char * ssid = "WIFI_TP_IOT";
const char * password = "TP_IOT_2022";
String mac;

WiFiClient wifiClient;
PubSubClient client(wifiClient);

std::vector<EnvSensor> sensors;
TFT_eSprite tftSprite = TFT_eSprite(&M5.Lcd);
uint32_t now;
IPAddress server(192, 168, 31, 134);

SHT3X sht30;
QMP6988 qmp6988;

float tmp      = 0.0;
float hum      = 0.0;
float pressure = 0.0;


const char* host = "api.thingspeak.com"; // This should not be changed
const int httpPort = 80; // This should not be changed

// The default example accepts one data filed named "field1"
// For your own server you can ofcourse create more of them.
int field1 = 0;

int numberOfResults = 3; // Number of results to be read
int fieldNumber = 1; // Field number which will be read out

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i=0;i<length;i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void printLCD(){
  
  char data[50];
  tftSprite.fillSprite(BLACK);
  tftSprite.drawLine(0, 0, 0, 239, TFT_RED);
  tftSprite.drawLine(80, 0, 80, 239, TFT_RED);
  tftSprite.drawLine(160, 0, 160, 239, TFT_RED);
  tftSprite.drawLine(240, 0, 240, 239, TFT_RED);
  tftSprite.drawLine(319, 0, 319, 239, TFT_RED);
  tftSprite.drawLine(0, 0, 319, 0, TFT_RED);
  tftSprite.drawLine(0, 60, 319, 60, TFT_RED);
  tftSprite.drawLine(0, 120, 319, 120, TFT_RED);
  tftSprite.drawLine(0, 180, 319, 180, TFT_RED);
  tftSprite.drawLine(0, 239, 319, 239, TFT_RED);
  for (int y = 0, i = 0  ; y < 4 && i < sensors.size() && i  < NBSENSOR_MAX ; y++){
    for (int x = 0; x < 4 && i < sensors.size() && i  < NBSENSOR_MAX ; x++, i++){ 
      tftSprite.setTextDatum(MC_DATUM);
      tftSprite.setTextColor(WHITE);
      tftSprite.setTextSize(2);
      sprintf(data, "%2.1f", sensors[i].getTemp());
      tftSprite.drawString(data, 40 + 80 * x, 15 + 60 * y, 1);
      tftSprite.setTextSize(1);
      sprintf(data, "%2.1f", sensors[i].getHum());
      tftSprite.drawString(data, 40 + 80 * x, 30 + 60 * y, 1);
      sprintf(data, "%2.1f", sensors[i].getPressure());
      tftSprite.drawString(data, 40 + 80 * x, 40 + 60 * y, 1);
      sprintf(data, sensors[i].getMac().c_str() + 9);
      tftSprite.drawString(data, 40 + 80 * x, 50 + 60 * y, 1);
     }
  }
  tftSprite.pushSprite(0, 0);
}



void setup() {

    M5.begin();             // Init M5Core2.  初始化M5Core2
    M5.lcd.setTextSize(2);  // Set the text size to 2.  设置文字大小为2
    Wire.begin();  // Wire init, adding the I2C bus.  Wire初始化, 加入i2c总线
    sht30.init();
    qmp6988.init();
    M5.lcd.println(F("ENVIII Unit(SHT30 and QMP6988) test"));

    Serial.begin(115200);
    while(!Serial){delay(100);}

    // We start by connecting to a WiFi network

    Serial.println();
    Serial.println("******************************************************");
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    Serial.println("MAC Address: ");
    Serial.println(WiFi.macAddress());
    mac = WiFi.macAddress();
    delay(10000);

    client.setServer(server, 1883);
    client.setCallback(callback);

}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("mac")) {
      Serial.println("connected");
      client.publish("outTopic","hello world");
      // ... and resubscribe
      client.subscribe("inTopic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  delay(10000);

  pressure = qmp6988.calcPressure();
  if (sht30.get() == 0) {  // Obtain the data of shT30.  获取sht30的数据
      tmp = sht30.cTemp;   // Store the temperature obtained from shT30.
      hum = sht30.humidity;  // Store the humidity obtained from the SHT30.
  } else {
      tmp = 0, hum = 0;
  }
  M5.lcd.fillRect(0, 20, 100, 60, BLACK);  // Fill the screen with black 
  M5.lcd.setCursor(0, 20);
  M5.Lcd.printf("Temp: %2.1f  \r\nHumi: %2.0f%%  \r\nPressure:%2.0fPa\r\n",
                tmp, hum, pressure);

  DynamicJsonDocument doc(1024);
  String output;
  doc["humidity"] = hum;
  doc["temperature"]   = tmp;
  doc["pressure"]= pressure;
  doc["mac"] = mac;

  serializeJson(doc, output);

  Serial.println(output);
  client.publish("Env/3C:61:05:0D:CA:38", output.c_str());
  client.subscribe("Env/#");

  delay(2000);
}