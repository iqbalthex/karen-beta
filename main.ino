#include <SPI.h>
#include <HttpClient.h>
#include <Ethernet.h>
#include <EthernetClient.h>
#include <HMC5883L_Simple.h>
#include <Wire.h>

#define HOST "https://karen-meter-beta-default-rtdb.firebaseio.com"
#define SSID "SSID"
#define PASS "PASS"

#define TIMER_DELAY 5000

#define SCREEN_WIDTH  128
#define SCREEN_HEIGHT  64
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing reset pin)

// Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int code;
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
float compassHeading;
// DateTime now;

EthernetClient  client;
HMC5883L_Simple compass;
HttpClient      http(client);
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
  Wire.begin();
  // rtc.begin();
  // rtc.adjust(DateTime(__DATE__, __TIME__));

  compass.SetSamplingMode(COMPASS_CONTINUOUS);
  compass.SetOrientation(COMPASS_HORIZONTAL_X_NORTH);

  while (Ethernet.begin(mac) != 1) {
    Serial.println("Error getting IP address via DHCP, trying again...");
    delay(15000);
  }  
}


void loop() {
  delay(TIMER_DELAY);

  Current current;
  // current.datetime  = getDatetime();
  current.direction = getDirection();
  // current.speed     = getSpeed();

  updateAll(current);
  updateLatest(current);
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
  compassHeading = compass.GetHeadingDegrees();
  return compassHeading;
}


// Get speed from current meter.
float getSpeed() {
  return 0.0;
}


// Update data in Firebase Realtime Database at 'current/all/{#datetime}' path.
void updateAll(Current current) {
  const char* endPoint = "/current.json";

  String dataStr = "{\"" + current.datetime + "\":{";
  dataStr += "\"direction\":" + String(current.direction) + ',';
  dataStr += "\"speed\":"     + String(current.speed);
  dataStr += "}}";

  const char* dataPtr = dataStr.c_str();

  http.put(HOST, endPoint, dataPtr);
  code = http.responseStatusCode();

  if (code > 0) {
    // Serial.println("Data updated: ");
    // Serial.println(http.read());
    Serial.println("Update data success");
  } else {
    Serial.println("Update data failed with code: " + code);
  }

  http.stop();
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
  code = http.responseStatusCode();

  if (code > 0) {
    // Serial.println("Data updated: ");
    // Serial.println(http.read());
    Serial.println("Update data success");
  } else {
    Serial.println("Update data failed with code: " + code);
  }

  http.stop();
}

