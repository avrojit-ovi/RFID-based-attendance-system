#include <SoftwareSerial.h>
#include <rdm6300.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

#define TFT_CLK D5  // TFT SPI clock pin (SCK)
//#define TFT_MISO D6 // TFT SPI MISO pin
#define TFT_MOSI D7 // TFT SPI MOSI pin (SDA)
#define TFT_CS D8   // TFT chip select pin
#define TFT_RST D3  // TFT reset pin
#define TFT_DC D2   // TFT data/command pin

#define RFID_RX_PIN D1
#define RFID_TX_PIN D6

#define WIFI_SSID "Web point lmt"
#define WIFI_PASSWORD "Wp333@ltd"

#define NTP_SERVER "pool.ntp.org"
#define NTP_OFFSET 0
#define NTP_UPDATE_INTERVAL 60000 // Update interval in milliseconds (1 minute)

SoftwareSerial RFID(D1, D6); // TX and RX


Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, NTP_SERVER, NTP_OFFSET, NTP_UPDATE_INTERVAL);

void setup()
{
  Serial.begin(115200);
  RFID.begin(9600);
  tft.initR(INITR_BLACKTAB);
  tft.setRotation(1);

  connectToWiFi();
  showWelcomeScreen();
  timeClient.begin();
}

void loop()
{
  timeClient.update();

  if (RFID.available())
  {
    char cardId = RFID.read(); // Read a single character
    displayCardId(String(cardId)); // Convert the character to a String and display
  }

  displayTime();
  delay(500);
}

void connectToWiFi()
{
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.print("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConnected to WiFi");
}

void showWelcomeScreen()
{
  tft.fillScreen(ST7735_BLACK);
  tft.setTextColor(ST7735_WHITE);
  tft.setTextSize(2);
  tft.setCursor(30, 50);
  tft.println("Welcome!");

  delay(3000);
}

void displayTime()
{
  tft.fillScreen(ST7735_BLACK);
  tft.setTextColor(ST7735_WHITE);
  tft.setTextSize(2);
  tft.setCursor(30, 50);
  tft.print("Time: ");
  tft.println(timeClient.getFormattedTime());
}

void displayCardId(const String &cardId)
{
  tft.setTextColor(ST7735_WHITE);
  tft.setTextSize(2);
  tft.setCursor(30, 100);
  tft.print("Card ID: ");
  tft.println(cardId);
}
