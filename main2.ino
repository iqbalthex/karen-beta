#include <Arduino.h>

#include <WiFi.h>
#include <WiFiMulti.h>

#include <HTTPClient.h>

#define USE_SERIAL Serial

WiFiMulti wifiMulti;

void setup() {

    USE_SERIAL.begin(115200);

    USE_SERIAL.println();
    USE_SERIAL.println();
    USE_SERIAL.println();

    for(uint8_t t = 4; t > 0; t--) {
        USE_SERIAL.printf("[SETUP] WAIT %d...\n", t);
        USE_SERIAL.flush();
        delay(1000);
    }

    wifiMulti.addAP("Galaxy J3 ProE5DF", "102938475600");

}

void loop() {
//     wait for WiFi connection
    if((wifiMulti.run() == WL_CONNECTED)) {

        HTTPClient http;

        USE_SERIAL.print("[HTTP] begin...\n");
        // configure traged server and url
        //http.begin("https://www.howsmyssl.com/a/check", ca); //HTTPS
        http.begin("https://karen-meter-beta-default-rtdb.firebaseio.com/current.json"); //HTTP

        USE_SERIAL.print("[HTTP] GET...\n");
        // start connection and send HTTP header
        int httpCode = http.GET();

        // httpCode will be negative on error
        if(httpCode > 0) {
            // HTTP header has been send and Server response header has been handled
            USE_SERIAL.printf("[HTTP] GET... code: %d\n", httpCode);

            // file found at server
            if(httpCode == HTTP_CODE_OK) {
                String payload = http.getString();
                USE_SERIAL.println(payload);
            }
        } else {
            USE_SERIAL.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        }

        http.end();
    }

    delay(5000);
}

/*#include <SPI.h>
#include <Bridge.h>
//#include <HTTPClient.h>
#include <ArduinoHttpClient.h>
#include <WiFi101.h>
#include "arduino_secrets.h"
#include <Adafruit_GFX.h>
//#include <Adafruit_SSD1306.h>
//#include "RTClib.h"
#include <Adafruit_Sensor.h>
#include <Adafruit_HMC5883_U.h>
#include <Wire.h>

#define ESP32_LED 2
#define HIGH 1
#define LOW 0

//#define HOST "https://karen-meter-beta-default-rtdb.firebaseio.com"
#define SSID "SSID"
#define PASS "PASS"

#define TIMER_DELAY 5000

#define SCREEN_WIDTH  128
#define SCREEN_HEIGHT  64
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing reset pin)

// Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const String HOST = "https://karen-meter-beta-default-rtdb.firebaseio.com";
int statusCode;
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
float xHeading;
// DateTime now;

HttpClient http;
Adafruit_HMC5883_Unified compass = Adafruit_HMC5883_Unified(12345);
// RTC_DS3231      rtc;

typedef struct {
  String datetime;
  float  direction, speed;
} Current;


String getDatetime();
float  getDirection();
float  getSpeed();
void   updateAll(Current current);
void   updateLatest(Current current);

int n = 0;

void setup() {
  Serial.begin(115200);
  pinMode(ESP32_LED, OUTPUT);

  ledcAttachPin(2, 1);
  ledcSetup(1, 12000, 8);
//  Wire.begin();
  // rtc.begin();
  // rtc.adjust(DateTime(__DATE__, __TIME__));

  /* Initialise the sensor *
//  if(!compass.begin())
//  {
//    /* There was a problem detecting the HMC5883 ... check your connections *
//    Serial.println("Ooops, no HMC5883 detected ... Check your wiring!");
//    while(1);
//  }

//  compass.SetSamplingMode(COMPASS_CONTINUOUS);
//  compass.SetOrientation(COMPASS_HORIZONTAL_X_NORTH);
}


void loop() {
  delay(TIMER_DELAY);

  Current current;
  // current.datetime  = getDatetime();
//  current.direction = getDirection();
  // current.speed     = getSpeed();

//  updateAll(current);
  updateAll();
//  updateLatest(current);
}


// Get datetime from NTP clock.
String getDatetime() {
  // now = rtc.now();

  // String year = String(now.year());

  // display.setTextSize(1);
  // display.setCursor(0,0);
  // display.println(now.year(), DEC);

  return "";
}


// Get direction from compass.
float getDirection() {
  /* Get a new sensor event *
//  sensors_event_t event; 
//  compass.getEvent(&event);
//
//  xHeading = event.magnetic.x;
//  return xHeading;
}


// Get speed from current meter.
float getSpeed() {
  return 0.0;
}


// Update data in Firebase Realtime Database at 'current/all/{#datetime}' path.
//void updateAll(Current current) {
void updateAll() {
//  String dataStr = "{\"" + current.datetime + "\":{";
//  dataStr += "\"direction\":" + String(current.direction) + ',';
//  dataStr += "\"speed\":"     + String(current.speed);
//  dataStr += "}}";

  String endPoint = HOST + "/current.json";

  // http.begin(endPoint);

  // http.put(HOST, endPoint, dataPtr);
  String nn = String(n);
  String data = "{\"tes\":" + nn + "}";
//  statusCode = http.post(endPoint, data);
  statusCode = http.get(endPoint);
  n++;
  Serial.println(endPoint);
  Serial.println(data);

  
  if (statusCode > 0) {
    while (http.available()) {
      Serial.println("print...");
      
      String payload = http.readString();
//      Serial.print(payload);
      Serial.println(payload.length());
    }

    Serial.println("Data updated: ");

//    digitalWrite(ESP32_LED, HIGH);
//    delay(500);
//    digitalWrite(ESP32_LED, LOW);
//    digitalWrite(ESP32_LED, HIGH);
//    delay(500);
//    digitalWrite(ESP32_LED, LOW);
    ledcWrite(1, 200);
    delay(500);
    ledcWrite(1, 0);
    ledcWrite(1, 200);
    delay(500);
    ledcWrite(1, 0);
  } else {
    Serial.println("Update data failed with code: " + statusCode);
//    digitalWrite(ESP32_LED, HIGH);
//    delay(2000);
//    digitalWrite(ESP32_LED, LOW);
    ledcWrite(1, 200);
    delay(2000);
    ledcWrite(1, 0);
    delay(2000);
  }
  Serial.println("");
}


// Update data in Firebase Realtime Database at 'current/latest' path.
void updateLatest(Current current) {
  const char* endPoint = "/current/latest.json";

  String dataStr = "{";
  dataStr += "\"datetime\":"  + String(current.datetime)  + ',';
  dataStr += "\"direction\":" + String(current.direction) + ',';
  dataStr += "\"speed\":"     + String(current.speed);
  dataStr += "}";

  const char* dataPtr = dataStr.c_str();

  http.put(HOST, endPoint, dataPtr);
  statusCode = http.responseStatusCode();

  if (statusCode > 0) {
    // Serial.println("Data updated: ");
    // Serial.println(http.read());
    Serial.println("Update data success");
  } else {
    Serial.println("Update data failed with code: " + statusCode);
  }
}*/
