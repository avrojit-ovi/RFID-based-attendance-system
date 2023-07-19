#include <Wire.h>
#include <RTClib.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <stdlib.h>

RTC_DS3231 rtc;
WiFiUDP udp;
NTPClient timeClient(udp, "pool.ntp.org", 6 * 3600); // set timezone for Bangladesh (GMT +6)

const char* ssid = "Web Point Warrior";
const char* password = "Pa$$w0rd!";

void setup() {
  Serial.begin(115200);
  Wire.begin(D7, D5); // set SDA and SCL pins for I2C
  rtc.begin();
  timeClient.begin();

  // Connect to Wi-Fi network
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
}

void loop() {
  timeClient.update();

  DateTime now = rtc.now();
  DateTime ntpTime = timeClient.getEpochTime();

  if (abs(int(now.unixtime() - ntpTime.unixtime())) > 60) {
    rtc.adjust(ntpTime); // update the time on the RTC module
  }

  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.print(' ');
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.println();
  
  delay(1000);
}
