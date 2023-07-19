#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <SoftwareSerial.h>
#include <RDM6300.h>
#include <DS3231.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>

// wifi ssid and password
const char* ssid = "Annoy";
const char* password = "Pa$$w0rd!";

// MySQL server IP Address
IPAddress server(254, 52, 254, 100);

// Initialize the OLED display
#define OLED_RESET 0
Adafruit_SSD1306 display(OLED_RESET);

// Initialize the RFID reader
#define RFID_RX D1
#define RFID_TX D2
RDM6300 rfid(RFID_RX, RFID_TX);

// Initialize the RTC module
DS3231 rtc;

// Initialize the WiFi client
WiFiClient client;

// Data logger
String dataString = "";

void setup() {
  // Initialize the OLED display
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.display();
  delay(1000);
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);

  // Initialize the RFID reader
  rfid.begin();

  // Initialize the RTC module
  rtc.begin();

  // Connect to Wi-Fi network
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
  }

  // Clear the data string
  dataString = "";
}

void loop() {
  // Get RFID tag ID
  if (rfid.available()) {
    String rfidTag = rfid.getTagString();
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("RFID Tag:");
    display.println(rfidTag);
    display.display();
    delay(1000);

    // Get the current date and time
    DateTime now = rtc.now();
    String dateTime = now.timestamp(DateTime::TIMESTAMP_FULL);

    // Append the data to the data string
    dataString += "RFID," + rfidTag + "," + dateTime + "\n";
  }

  // Send data to server every 10 seconds
  if (millis() % 10000 == 0) {
    if (client.connect(server, 80)) {
      // Make a HTTP request to the server
      client.println("POST /logdata.php HTTP/1.1");
      client.println("Host: 254.52.254.100");
      client.println("Content-Type: application/x-www-form-urlencoded");
      client.print("Content-Length: ");
      client.println(dataString.length());
      client.println();
      client.println(dataString);
      dataString = "";
    }
    client.stop();
  }
}