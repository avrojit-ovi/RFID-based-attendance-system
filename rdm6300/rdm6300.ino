#include <SoftwareSerial.h>
SoftwareSerial RFID(D1, D2); // RX and TX

String text;
String CardNumber; 

void setup()
{
  Serial.begin(9600);
  RFID.begin(9600);
  Serial.println("Bring your RFID Card Closer...");
  CardNumber = "100053888B"; 
}
char c;

void loop()
{
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
  Serial.println("Card ID : " + text);
  Serial.println("Access ID : " + CardNumber);

  if (CardNumber.indexOf(text) >= 1) {
    Serial.println("Access accepted");
  }
  else {
    Serial.println("Access denied");
  }
  delay(2000);
  Serial.println(" ");
  Serial.println("Bring your RFID card closer …");
}