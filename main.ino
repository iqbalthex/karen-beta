#include <WiFi.h>
#include <HTTPClient.h>

const char* HOST = "https://karen-meter-beta-default-rtdb.firebaseio.com";
const char* SSID = "SSID";
const char* PASS = "PASS";
int timerDelay = 5000;
int code;
String endPoint, response, data;

struct {
  String datetime;
  float direction, speed;
} Current;

HTTPClient http;

void setup(Current current) {
  Serial.begin(115200);
  WiFi.begin(SSID, PASS);

  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
}

void loop() {
  delay(timerDelay);

  if (WiFi.status() == WL_CONNECTED) {
    Current current;

    current.datetime  = getDatetime();
    current.direction = getDirection();
    current.speed     = getSpeed();

    updateAll(current);
    updateLatest(current);
  } else {
    Serial.println("WiFi disconnected");

    while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  }
}


/**
 * Get datetime from clock component.
 */
String getDatetime() {
  return "";
}


/**
 * Get direction from compass component.
 */
float getDirection() {
  return 0.0;
}


/**
 * Get speed from current meter component.
 */
float getSpeed() {
  return 0.0;
}


/**
 * Update data in Firebase Realtime Database at 'current/all/{#datetime}' path.
 */
void updateAll(Current current) {
  endPoint = HOST + "/current/all/" + current.datetime + ".json";

  data = "";
  data += "\"direction\":" + current.direction + ",";
  data += "\"speed\":"     + current.speed;

  http.begin(endPoint.c_str());
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");

  code = http.PUT("{" + data + "}");

  if (code > 0) {
    response = "Update data success";
  } else {
    response = "Update data failed with code: " + code;
  }

  http.end();
  return response;
}


/**
 * Update data in Firebase Realtime Database at 'current/latest' path.
 */
void updateLatest(Current current) {
  String endPoint = HOST + "/current/latest" + ".json";

  data = "";
  data += "\"datetime\":"  + current.datetime  + ",";
  data += "\"direction\":" + current.direction + ",";
  data += "\"speed\":"     + current.speed;

  http.begin(endPoint.c_str());
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");

  code = http.PUT("{" + data + "}");

  if (code > 0) {
    response = "Update data success";
  } else {
    response = "Update data failed with code: " + code;
  }

  http.end();
  return response;
}

