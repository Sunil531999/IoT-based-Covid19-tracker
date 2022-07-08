#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecureBearSSL.h>
#include <Adafruit_GFX.h>       
#include <Adafruit_ST7735.h> 
#include <SPI.h>
#define TFT_CS         D8     
#define TFT_RST        D3
#define TFT_DC         D4
 //below is the fingerprint of https://disease.sh/ website
const uint8_t fingerprint[20] = {0x2d, 0x90, 0xba, 0x2b, 0x42, 0x5f, 0x20, 0xb8, 0x86, 0xa8, 0x20, 0xde, 0x07, 0x9d, 0xbf, 0x0a, 0x68, 0xb8, 0xf6, 0x16};

const char* country = "";
long cases = 0;
long deaths = 0;
long recovered = 0;
long active = 0;
int critical = 0;
ESP8266WiFiMulti WiFiMulti;
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST); 
void setup() {
  
  tft.initR(INITR_GREENTAB);
  Serial.begin(115200);

  Serial.println();
  Serial.println();
  Serial.println();

  for (uint8_t t = 4; t > 0; t--) {
    Serial.printf("[SETUP] WAIT %d...\n", t);
    Serial.flush();
    delay(1000);
  }
  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP("SSID", "PASSWORD"); //replace the SSID with your wifi name and PASSWORD with your wifi password
}

void loop() {

  if ((WiFiMulti.run() == WL_CONNECTED)) {

    std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);

    client->setFingerprint(fingerprint);

    HTTPClient https;

    Serial.print("[HTTPS] begin...\n");
    //below you have to replace the country code https://disease.sh/v3/covid-19/countries/<country code or country name>
    if (https.begin(*client, "https://disease.sh/v3/covid-19/countries/in")) {  

      Serial.print("[HTTPS] GET...\n");
      int httpCode = https.GET();

      if (httpCode > 0) {

        Serial.printf("[HTTPS] GET... code: %d\n", httpCode);

        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
          String payload = https.getString();

        StaticJsonDocument<0> filter;
        filter.set(true);
        
        DynamicJsonDocument doc(2048);
        
        DeserializationError error = deserializeJson(doc, payload, DeserializationOption::Filter(filter));
        
        if (error) {
          Serial.print(F("deserializeJson() failed: "));
          Serial.println(error.f_str());
          return;
        }
        
        country = doc["country"];
        cases = doc["cases"]; 
        deaths = doc["deaths"]; 
        recovered = doc["recovered"]; 
        active = doc["active"]; 
        critical = doc["critical"]; 

Serial.println(country);
Serial.println(cases);
Serial.println(deaths);
Serial.println(recovered);
Serial.println(active);
Serial.println(critical);
display();
        }
      } else {
        Serial.printf("[HTTPS] GET... failed, error: %s\n", https.errorToString(httpCode).c_str());
      }

      https.end();
    } else {
      Serial.printf("[HTTPS] Unable to connect\n");
    }
  }

  Serial.println("Wait 10s before next round...");
  delay(10000);
}

void display()
{
 String c=String(country);
  c.toUpperCase();
  tft.fillScreen(ST77XX_WHITE);
  tft.setTextWrap(false);
  tft.setCursor(32, 5);    
  tft.setTextSize(2);
  tft.setTextColor(ST77XX_ORANGE);
  tft.print(c);

tft.fillRoundRect(0, 23, tft.width(),12, 5, ST77XX_GREEN);
  tft.setCursor(15, 26);    //Horiz/Vertic
  tft.setTextSize(1);
  tft.setTextColor(ST77XX_BLACK);
  tft.print("COVID-19 Tracker");

tft.drawRoundRect(0,36,tft.width(),35, 5, ST77XX_BLACK);
  tft.setCursor(5, 38);    //Horiz/Vertic
  tft.setTextSize(2);
  tft.setTextColor(ST77XX_YELLOW);
  tft.print("Cases");
  tft.setCursor(5, 55);
  tft.print(cases);  

tft.drawRoundRect(0,75,tft.width(),35, 5, ST77XX_BLACK);
  tft.setCursor(5, 77);    
  tft.setTextSize(2);
  tft.setTextColor(ST77XX_BLUE);
  tft.print("Recovered");
  tft.setCursor(5, 94);
  tft.print(recovered); 

tft.drawRoundRect(0,114,tft.width(),35, 5, ST77XX_BLACK);
  tft.setCursor(5, 116);   
  tft.setTextSize(2);
  tft.setTextColor(ST77XX_GREEN);
  tft.print("Deaths");
  tft.setCursor(5, 133);
  tft.print(deaths);
}
