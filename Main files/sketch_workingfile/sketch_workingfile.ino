#include <Wire.h>
#include <RTClib.h>
#include <Adafruit_GFX.h>      // include Adafruit graphics library
#include <Adafruit_ST7735.h> 
#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <stdlib.h>

#define TFT_RST   D4     // TFT    RST     pin is connected to  pin D4 (GPIO2)
#define TFT_CS    D3     // TFT    CS      pin is connected to  pin D3 (GPIO0)
#define TFT_DC    D0     // TFT    DC      pin is connected to  pin D0 (GPIO4)
//                       // SCK/SCL   (CLK)    pin is connected to  pin D5 (GPIO14)
//                       // SDA MOSI(DIN)  pin is connected to  pin D7 (GPIO13)


#define BUZZER_PIN D8

RTC_DS3231 rtc;
WiFiUDP udp;
NTPClient timeClient(udp, "pool.ntp.org", 6 * 3600); 

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);


SoftwareSerial RFID(D1, D6); // TX and RX

String text;
String DeviceID; 

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
       // <!...start 2 beep sound...!>
    digitalWrite(BUZZER_PIN, HIGH);
    delay(700);
    digitalWrite(BUZZER_PIN, LOW);  
    delay(100);   
    digitalWrite(BUZZER_PIN, HIGH);
    delay(700);
    digitalWrite(BUZZER_PIN, LOW);   
   // <!...end 2 beep sound...!>   
    Serial.println("Bring your RFID card closer …");     
    return true;
         
  } else {
    Serial.printf("Failed to connect to %s\n", ssidList[networkIndex]);

      // <!...start 5 beep sound...!>    
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
    // <!...end 5 beep sound...!>    
    return false;
  }
}

// set timezone for Bangladesh (GMT +6)

void setup()
{
  Serial.begin(9600);
  Wire.begin(D7, D5); // set SDA and SCL pins for I2C
  rtc.begin();
  timeClient.begin();  
  
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);  
   for (int i = 0; i < sizeof(ssidList) / sizeof(ssidList[0]); i++) {
    if (connectToWifi(i)) {
      break;
    }
  }
  DeviceID = WiFi.macAddress(); 
   
  RFID.begin(9600);
  Serial.println("Device ID :" + DeviceID);   

tft.initR(INITR_BLACKTAB);   // initialize a ST7735S chip, black tab

uint16_t time = millis();
tft.fillScreen(ST7735_BLACK);
time = millis() - time;
  
delay(500);

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
    delay(5);
    c = RFID.read();
    text += c;
  }
  if (text.length() > 20)
  check();
  text = "";
  


     
}

void check()
{
  text = text.substring(1, 11);

Serial.println("Device ID :" + DeviceID);  
Serial.println("Card ID : " + text);
// print time start

timeClient.update();

  DateTime now = rtc.now();
  DateTime ntpTime = timeClient.getEpochTime();

  if (abs(int(now.unixtime() - ntpTime.unixtime())) > 60) {
    rtc.adjust(ntpTime); // update the time on the RTC module
  }

Serial.print("Current time: ");
Serial.println(now.timestamp());  

// Serial.print(now.year(), DEC);
//   Serial.print('/');
//   Serial.print(now.month(), DEC);
//   Serial.print('/');
//   Serial.print(now.day(), DEC);
//   Serial.print(' ');
//   Serial.print(now.hour(), DEC);
//   Serial.print(':');
//   Serial.print(now.minute(), DEC);
//   Serial.print(':');
//   Serial.print(now.second(), DEC);
//   Serial.println();

//  print time end
// Serial.print("Card ID : ");
//   for (int i = 0; i < text.length(); i++) {
//     Serial.printf("%02X ", text[i]);
//   }
  // Serial.println();
digitalWrite(LED_BUILTIN, HIGH);
digitalWrite(BUZZER_PIN, HIGH);
delay(250);
digitalWrite(LED_BUILTIN, LOW);
digitalWrite(BUZZER_PIN, LOW);
Serial.println("!!!....THANK YOU SO MUCH....!!!");  
  // Serial.println("Access ID : " + CardNumber);

  // if (CardNumber.indexOf(text) >= 1) {
  //   Serial.println("Access accepted");
  // }
  // else {
  //   Serial.println("Access denied");
  // }
  delay(10);
  Serial.println(" ");
  Serial.println(" ");
  Serial.println("Bring your RFID card closer …");

tft.setTextWrap(false);
  tft.fillScreen(ST7735_BLACK);
  tft.setCursor(0, 30);
 tft.setTextColor(ST7735_RED);
  tft.setTextSize(1);
  tft.println("Card ID : " + text);  


tft.setCursor(0, 0);
tft.setTextColor(ST7735_WHITE, ST7735_BLACK);
tft.print("Current time:");
tft.setCursor(0, 10);
tft.setTextColor(ST7735_YELLOW, ST7735_BLACK);
tft.print(now.timestamp());  

}