#include <SPI.h>
#include <HttpClient.h>
#include <Ethernet.h>
#include <EthernetClient.h>

const char* HOST = "https://karen-meter-beta-default-rtdb.firebaseio.com";
const char* SSID = "SSID";
const char* PASS = "PASS";
int code, timerDelay = 5000;
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

EthernetClient client;
HttpClient     http(client);

typedef struct {
  String datetime;
  float direction, speed;
} Current;

void updateAll(Current current);
void updateLatest(Current current);


void setup() {
  Serial.begin(115200);

  while (Ethernet.begin(mac) != 1) {
    Serial.println("Error getting IP address via DHCP, trying again...");
    delay(15000);
  }  
}


void loop() {
  delay(timerDelay);

  Current current;
  // current.datetime  = "2023-06-18_00:00:00";
  // current.direction = 115;
  // current.speed     = 0.4;

  updateAll(current);
  updateLatest(current);
}


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
