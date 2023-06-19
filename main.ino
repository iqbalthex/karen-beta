#include <SPI.h>
#include <Bridge.h>
#include <HTTPClient.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "RTClib.h"
#include <Adafruit_Sensor.h>
#include <Adafruit_HMC5883_U.h>
#include <Wire.h>

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

//EthernetClient  client;
//HTTPClient      http(client);

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


void setup() {
  Serial.begin(115200);
  pinMode(13, OUTPUT);
//  Wire.begin();
  // rtc.begin();
  // rtc.adjust(DateTime(__DATE__, __TIME__));

  /* Initialise the sensor */
//  if(!compass.begin())
//  {
//    /* There was a problem detecting the HMC5883 ... check your connections */
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
  /* Get a new sensor event */ 
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
  statusCode = http.get(endPoint);

  if (statusCode > 0) {
    while (http.available()) {
      Serial.print(http.read());
    }
    Serial.println("");

    Serial.println("Data updated: ");

    digitalWrite(13, 1);
    delay(500);
    digitalWrite(13, 0);
    digitalWrite(13, 1);
    delay(500);
    digitalWrite(13, 0);
  } else {
    Serial.println("Update data failed with code: " + statusCode);
    digitalWrite(13, 1);
    delay(2000);
    digitalWrite(13, 0);
  }
}


/*/ Update data in Firebase Realtime Database at 'current/latest' path.
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
