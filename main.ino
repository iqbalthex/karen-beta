#include <Arduino.h>

#include <HTTPClient.h>

#include <WiFiMulti.h>

#include "RTClib.h"

WiFiMulti  wifi;
HTTPClient http;
RTC_DS3231 rtc;

int statusCode;

typedef struct {
  String datetime;
  float direction, speed;
} Current;


String getDatetime();
float  getDirection();
float  getSpeed();
void   updateAll();
//void   updateLatest();


void setup() {
  Serial.begin(115200);

  for(uint8_t t = 3; t > 0; t--) {
    Serial.printf("[SETUP] WAIT %d...\n", t);
    delay(1000);
  }

  wifi.addAP("Galaxy J3 ProE5DF", "102938475600");

  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
  }
  rtc.adjust(DateTime(__DATE__, __TIME__));
}

void loop() {
  if (wifi.run() == WL_CONNECTED) {
    Current current;
    current.datetime  = "2023-06-19_15:00:00"; // getDatetime();
    current.direction = 124; // getDirection();
    current.speed     = 0.8; // getSpeed();

//    updateAll(current);
    updateLatest(current);
  }
  delay(3000);
}


String getDatetime() {
  Datetime now;

  String year   = String(now.year());
  String month  = String(now.month());
  String day    = String(now.day());
  String hour   = String(now.hour());
  String minute = String(now.minute());
  String second = String(now.second());

  // String datetime;
  // sprintf(datetime, "%s-%s-%s_%s:%s:%s", year, minute, day, hour, minute, second);
  return year + '-' + month + '-' + day + '_' + hour + ':' + minute + ':' + second;
}


float getDirection() {
  return 0.0;
}


float getSpeed() {
  return 0.0;
}


void updateAll() {
  http.begin("https://karen-meter-beta-default-rtdb.firebaseio.com/current/all.json");
  Serial.println("Updating data...");

  String data = '{';
  data += "\"datetime\":"  + String(current.datetime)  + ',';
  data += "\"direction\":" + String(current.direction) + ',';
  data += "\"speed\":"     + String(current.speed);
  data += '}';

  statusCode = http.PATCH(data);

  if(statusCode > 0) {
    Serial.printf("Status code: %d\n", statusCode);

    if(statusCode == HTTP_CODE_OK) {
      String payload = http.getString();
    }
    Serial.println("Update data success");
  } else {
    Serial.println("Update data failed");
    Serial.printf("Error: %s\n", http.errorToString(statusCode).c_str());
  }

  http.end();
}

void updateLatest(Current current) {
  http.begin("https://karen-meter-beta-default-rtdb.firebaseio.com/current/latest.json");
  Serial.println("Updating data...");

  String data = "{\"" + current.datetime + "\":{";
  data += "\"direction\":" + String(current.direction) + ',';
  data += "\"speed\":"     + String(current.speed);
  data += "}}";

  statusCode = http.PUT(data);

  if(statusCode > 0) {
    Serial.printf("Status code: %d\n", statusCode);

    if(statusCode == HTTP_CODE_OK) {
      String payload = http.getString();
    }
    Serial.println("Update data success");
  } else {
    Serial.println("Update data failed");
    Serial.printf("Error: %s\n", http.errorToString(statusCode).c_str());
  }

  http.end();
}
