#include <NTPClient.h>
#include <ESP8266WiFi.h> 
#include <ESP8266HTTPClient.h>
#include <WiFiUdp.h> 
#include <Adafruit_GFX.h>      
#include <Adafruit_ST7735.h>   
#include <SoftwareSerial.h>   
#define rx D6                               
#define tx D8
int count = 0; 
String id;
char input[13]; 
SoftwareSerial myserial(rx, tx); 

#define TFT_RST   D4    
#define TFT_CS    D3    
#define TFT_DC    D2 //A0 or DC
// SDA --> D7
// SCK --> D5
// LED --> 3.3v
// GND --> GND 
// VCC --> 5v   

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);
const char *ssid     = "muthu";
const char *password = "muthumuthu001";
int hh;
int mm;
int ss;
int day1;
String t; 
String d; 
String date;
String date_time;
String payload;
String payload1;
String arr_days[]={"SUNDAY","MONDAY","TUESDAY","WEDNESDAY","THURSDAY","FRIDAY","SATURDAY"};

void setup() {
  tft.initR(INITR_GREENTAB);
 Serial.begin(9600);
 myserial.begin(9600);
 
WiFi.begin(ssid, password);
  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    Serial.print ( "." );
  }


} 
 
void loop() {
  
   if(myserial.available())
   {
      count = 0;
       
      while(myserial.available() && count < 12)          // Read 12 characters and store them in input array
      {
     
         input[count] = myserial.read();
         count++;
         delay(5);
      }
      id = String(input);
      Serial.println(id); 
 
   HTTPClient http;  //Declare an object of class HTTPClient
  
    http.begin("http://192.168.43.204/attendance/insert.php?date="+String(date)+"&time="+String(t)+"&rf="+String(id));  //Specify request destination
    
    int httpCode = http.GET();  
       if (httpCode > 0) { //Check the returning code
 
     payload = http.getString();   //Get the request response payload
      Serial.println(payload); 
       }
       http.end();
       tft.fillScreen(ST7735_BLACK);
     tft.setTextWrap(false);
     tft.fillRect(0, 0 , 160, 24, ST7735_WHITE); 
     tft.setCursor(5, 10);    //Horiz/Vertic
     tft.setTextSize(1);
     tft.setTextColor(ST7735_MAGENTA);
     tft.print(payload);
     tft.fillRect(0, 25 , 160, 137, ST7735_BLACK); 
      tft.setCursor(40, 50);
     tft.setTextSize(4);
      tft.setTextColor(ST7735_CYAN);
        tft.print("OK");
         tft.setCursor(1, 100);
          tft.setTextSize(3);
      tft.setTextColor(ST7735_CYAN);
        tft.print("DONE..");
        
  tft.fillRect(0, 138 , 160, 20, ST7735_WHITE); 
  tft.setCursor(7, 146);    //Horiz/Vertic
  tft.setTextSize(1);
  tft.setTextColor(ST7735_BLACK);
  tft.print("ADD YOUR ATTENDANCE");
     delay(5000);
     payload="";
         }

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 19800,60000);
timeClient.begin();
timeClient.update();
//Serial.println(timeClient.getFormattedTime());
hh = timeClient.getHours();
mm = timeClient.getMinutes();
ss = timeClient.getSeconds();
 //Serial.println (hh);
  if(hh>12)
  {
    hh=hh-12;
    //Serial.print(hh);
    //Serial.print(":");
    //Serial.print(mm);
    //Serial.print(":");
    //Serial.print(ss);
    //println(" PM");
     String colon=":";
    t = ( String(hh) + colon + mm + colon + "PM");
    //Serial.println (t);
  }
  else
  {
    //Serial.print(hh);
    //Serial.print(":");
    //Serial.print(mm);
    //Serial.print(":");
    //Serial.print(ss);
    //Serial.println(" AM");  
    
   String colon=":";
    t = ( String(hh) + colon + mm + colon + "PM");
    //Serial.println (t);
  }
 int day = timeClient.getDay();
 d =(arr_days[day]);
  Serial.println (d);
  date_time = timeClient.getFormattedDate();
  int index_date = date_time.indexOf("T");
  date = date_time.substring(0, index_date);
  Serial.println(date);
 
 // --------  TEST TEXT  -------------------------------------------
   tft.fillScreen(ST7735_BLACK);
  tft.setTextWrap(false);
  tft.fillRect(0, 0 , 160, 24, ST7735_WHITE); 
  tft.setCursor(37, 5);    //Horiz/Vertic
  tft.setTextSize(2);
  tft.setTextColor(ST7735_MAGENTA);
  tft.print("SVCET");
 
  tft.setCursor(15, 28);    //Horiz/Vertic
  tft.setTextSize(1);
  tft.setTextColor(ST7735_WHITE);
  tft.print("Smart Attendance");
 
  tft.setCursor(40, 45);    //Horiz/Vertic
  tft.setTextSize(2);
  tft.setTextColor(ST7735_GREEN);
  tft.print("TIME");
  tft.setCursor(15, 68);
  tft.setTextColor(ST7735_YELLOW);
  tft.print(t);  
 
  tft.setCursor(40, 93);    //Horiz/Vertic
  tft.setTextSize(2);
  tft.setTextColor(ST7735_GREEN);
  tft.print("DATE:");
  tft.setCursor(5, 115);
  tft.setTextColor(ST7735_WHITE);
  tft.print(date); 
 
 
  tft.fillRect(0, 138 , 160, 20, ST7735_WHITE); 
  tft.setCursor(7, 146);    //Horiz/Vertic
  tft.setTextSize(1);
  tft.setTextColor(ST7735_BLACK);
  tft.print("PLESE ENTER YOUR ID");
             


delay(1000);

    
}
