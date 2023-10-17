#include <M5Core2.h>
#include "EnvSensor.hpp"
#include "M5_ENV.h"
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <vector>

const char * ssid = "WIFI_TP_IOT";
const char * password = "TP_IOT_2022";

#define NBSENSOR_MAX 16
std::vector<EnvSensor> sensors;
TFT_eSprite tftSprite = TFT_eSprite(&M5.Lcd);
uint32_t now;
IPAddress server(192, 168, 31, 134);



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
    M5.begin();
    now = millis();
    tftSprite.setColorDepth(8);
    tftSprite.createSprite(320, 240);
    Serial.println("Fin du Setup");
}



void loop() {
  float tmp, hum, pressure;
    if(millis() - now > 1000){
        Serial.println("Loop");
        now = millis();
        printLCD();
    }
}