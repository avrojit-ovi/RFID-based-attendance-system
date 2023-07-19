#include <Wire.h>
// #include <RTClib.h>
#include <TimeLib.h>
#include <Adafruit_GFX.h>          // include Adafruit graphics library
#include <Adafruit_ST7735.h>
#include <Fonts/FreeMono12pt7b.h>
#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <NTPClient.h>

#define TFT_RST   D3       // TFT    RST     pin is connected to  pin D4 (GPIO2)
#define TFT_CS    D8       // TFT    CS      pin is connected to  pin D3 (GPIO0)
#define TFT_DC    D2       // TFT    DC      pin is connected to  pin D0 (GPIO4)
//                         // SCK/SCL   (CLK)    pin is connected to  pin D5 (GPIO14)
//                         // SDA MOSI(DIN)  pin is connected to  pin D7 (GPIO13)

#define BUZZER_PIN D4

// RTC_DS3231 rtc;
// WiFiUDP udp;
// NTPClient timeClient(udp, "pool.ntp.org", 6 * 3600); //ntp Bangladesh time Zone

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

SoftwareSerial RFID(D1, D6);   // TX and RX

String text;
String DeviceID;
String cardData;


const char* ssidList[] = {"stellar", "Web point lmt", "Web Point Warrior"};
const char* passwordList[] = {"12345678", "Wp333@ltd", "Pa$$w0rd!"};

bool connectToWifi(int networkIndex) {
  Serial.printf("Connecting to %s\n", ssidList[networkIndex]);
  WiFi.begin(ssidList[networkIndex], passwordList[networkIndex]);
  int tries = 0;
  while (WiFi.status() != WL_CONNECTED && tries < 10) {
    delay(1000);
    Serial.print(".");
    tries++;
  }
  if (WiFi.status() == WL_CONNECTED) {
    Serial.printf("Connected to %s\n", ssidList[networkIndex]);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    //Wifi connected burzzer code here

    digitalWrite(BUZZER_PIN, HIGH);
    delay(700);
    digitalWrite(BUZZER_PIN, LOW);
    delay(100);
    digitalWrite(BUZZER_PIN, HIGH);
    delay(700);
    digitalWrite(BUZZER_PIN, LOW);
    Serial.println("Bring your RFID card closer …");

    
    return true;
   
  } else {
    
    //Wifi not connected burzzer code here

    Serial.printf("Failed to connect to %s\n", ssidList[networkIndex]);
    digitalWrite(BUZZER_PIN, HIGH);
    delay(300);
    digitalWrite(BUZZER_PIN, LOW);
    delay(100);
    digitalWrite(BUZZER_PIN, HIGH);
    delay(150);
    digitalWrite(BUZZER_PIN, LOW);
    delay(100);
    digitalWrite(BUZZER_PIN, HIGH);
    delay(300);
    digitalWrite(BUZZER_PIN, LOW);
    delay(100);
    digitalWrite(BUZZER_PIN, HIGH);
    delay(150);
    digitalWrite(BUZZER_PIN, LOW);
    delay(100);
    digitalWrite(BUZZER_PIN, HIGH);
    delay(300);
    digitalWrite(BUZZER_PIN, LOW);
    return false;
  }
}

void setup() {
  Serial.begin(9600);
  Wire.begin(D7, D5);
  // rtc.begin();
  // timeClient.begin();

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  
  for (int i = 0; i < sizeof(ssidList) / sizeof(ssidList[0]); i++) {
    if (connectToWifi(i)) {
      break;
    }
  }
  
  //device id from mac address
  DeviceID = WiFi.macAddress();
  // Reserve space for the card data string
  cardData.reserve(20); 
  
  RFID.begin(9600);
  Serial.println("Device ID: " + DeviceID);

  // Welcome Screen 
  tft.initR(INITR_BLACKTAB);
  tft.fillScreen(ST7735_YELLOW);
  tft.setTextColor(ST7735_BLACK);
  tft.setTextSize(2);
  tft.setCursor(20, 50);
  tft.println("Welcome!");

  delay(2000);

  // show device name

  tft.fillScreen(ST7735_BLUE);
  tft.setTextColor(ST7735_WHITE);
  tft.setTextSize(2);
  tft.setCursor(20, 50);
  tft.println("MegaTek!");
  delay(4000);

  // ready for use 

  tft.fillScreen(ST7735_BLACK);
  
  tft.setTextSize(2);
  tft.setTextColor(ST7735_WHITE);
  tft.setCursor(35, 45);
  tft.println("Ready");
  tft.setCursor(48, 65);
  tft.println("For");
  tft.setCursor(45, 85);
  tft.println("Use");

  // Display Device ID
  tft.setTextColor(ST7735_GREEN);
  tft.setTextSize(1);
  tft.setCursor(0, 150);
  tft.println("4D65A8A6");
  tft.setTextColor(ST7735_YELLOW);
  tft.setTextSize(1);
  tft.setCursor(100, 150);
  tft.println("WiFi");
  // Institute or Company Name here
  tft.setCursor(15, 0);
  tft.setTextColor(ST7735_YELLOW);
  tft.println("Web Point Limited");
  
}

char c;

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Disconnected from Wi-Fi network");
    for (int i = 0; i < sizeof(ssidList) / sizeof(ssidList[0]); i++) {
      if (connectToWifi(i)) {
        break;
      }
    }
  }

  while (RFID.available() > 0) {
    delay(2);
    c = RFID.read();
    text += c;
  }

  if (text.length() > 20) {
    check();
    text = "";
  }
}

void check() {
  // text = text.substring(1, 11);

  // Assign card ID to the cardData variable
  cardData = text.substring(1, 11); 

  Serial.println("Device ID: " + DeviceID); //Device ID
  Serial.println("Card ID: " + cardData);       //Card ID

   // Get current time from NTP server
  WiFiUDP udp;
  NTPClient timeClient(udp, "pool.ntp.org", 6 * 3600); // NTP server and time zone offset for Bangladesh
  timeClient.begin();
  timeClient.update();
  unsigned long epochTime = timeClient.getEpochTime();

  // Convert epoch time to Bangladesh's time zone
  epochTime += 3600 * 6;

  // Convert epoch time to a formatted time string in 12-hour format
  char formattedTime[12];
  sprintf(formattedTime, "%02d:%02d %s", hour(epochTime) % 12, minute(epochTime), hour(epochTime) < 12 ? "AM" : "PM");

  // Print the formatted time
  Serial.println(formattedTime);
  
 //Beep when card is punched
  digitalWrite(LED_BUILTIN, HIGH);
  digitalWrite(BUZZER_PIN, HIGH);
  delay(250);
  digitalWrite(LED_BUILTIN, LOW);
  digitalWrite(BUZZER_PIN, LOW);
  
  Serial.println("!!!....THANK YOU SO MUCH....!!!");


  
  //display card id 

  tft.setTextWrap(false);
  tft.fillScreen(ST7735_BLACK);
  // tft.setCursor(0, 30);
  // tft.setTextColor(ST7735_RED);
  // tft.setTextSize(1);
  // tft.println("Card ID: " + text);

  // Display Card Id Under

  tft.setCursor(5, 75);
  tft.setTextColor(ST7735_GREEN);
  tft.setTextSize(2.5);
  tft.println(cardData);
  
 //Display Timer

  tft.setCursor(0, 20);
  tft.setTextSize(1);
  tft.setTextColor(ST7735_WHITE, ST7735_BLACK);
  tft.print("Current time:");
  tft.setCursor(0, 20);
  tft.setTextColor(ST7735_YELLOW, ST7735_BLACK);
  // tft.print(now.timestamp());

  // Display Device ID
  tft.setTextColor(ST7735_GREEN);
  tft.setTextSize(1);
  tft.setCursor(0, 0);
  tft.println("4D65A8A6");
  tft.setTextColor(ST7735_YELLOW);
  tft.setTextSize(1);
  tft.setCursor(100, 0);
  tft.println("WiFi");

  delay(3000);

  //Ready Display code Here

  tft.fillScreen(ST7735_BLACK);
  tft.setTextColor(ST7735_GREEN);
  tft.setTextSize(2);
  tft.setCursor(30, 55);
  tft.println("Ready!");
  tft.setCursor(15, 80);
  tft.setTextColor(ST7735_YELLOW, ST7735_BLACK);
  tft.println(formattedTime);

  // Print Device ID

  tft.setTextColor(ST7735_GREEN);
  tft.setTextSize(1);
  tft.setCursor(0, 0);
  tft.println("4D65A8A6");
  tft.setTextColor(ST7735_YELLOW);
  tft.setTextSize(1);
  tft.setCursor(105, 0);
  tft.println("WiFi");
  tft.setCursor(15, 150);
  tft.setTextColor(ST7735_YELLOW);
  tft.println("Web Point Limited");
}
