#include <Arduino.h>
#include "credentials.h"
#include <Wire.h>                 
#include "rgb_lcd.h"
#include <WiFi.h>
 
#define I2C_SDA 17   //GPIO 17 anstelle Standard GPIO 21 für SDA
#define I2C_SCL 16   //GPIO 16 anstelle Standard GPIO 22 für SCL
 
 
rgb_lcd lcd;

 
//long lastMsg = 0;
 
//char buf[50];
 
int buttonPressed = 0; // Finger auf TouchSensor
int buttonPin = 26; // TouchSensor
int ledPinRed = 14;
int ledPinGreen = 33;

void TouchSensorClicked() {
  buttonPressed = digitalRead(buttonPin);
}

void WifiSetup() {
  lcd.print("Connecting...");
  Serial.print("\nConnecting to");
  Serial.println(ssid);

    WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED)
  {
    lcd.setCursor(0, 2);
    lcd.print("Status ");
    lcd.print(WiFi.status());
    Serial.println(WiFi.status());
  }

  Serial.print("\nConnected to");
  Serial.println(ssid);

}

void setup() {
   Serial.begin(115200);
 
  //Optional, da Standard
  //Wire.begin(SDA, SCL);     //SDA default is GPIO 21, SCL default is GPIO 22
 
  //==> 2. I2C Bus des ESP32 verwenden. Auf diesem Steckplatz (D2V5) liegen zusätzlich auch direkt die für das Display benötigten 5V! 
  Wire.begin(I2C_SDA, I2C_SCL);  
  WifiSetup();
   
  lcd.begin(16, 2);
  lcd.clear();
  lcd.setRGB(255, 255, 255);
  lcd.print("starten ...");
  delay(1000);
  
  
  //lcd.setCursor(0, 1);
  // print into line 2:
  //sprintf(buf, "Akt.Millis:%04d\0", lastMsg);
  //lcd.print(buf);

  pinMode(ledPinRed, OUTPUT);
  pinMode(ledPinGreen, OUTPUT);
  pinMode(buttonPin, INPUT);

  attachInterrupt(buttonPin, TouchSensorClicked, CHANGE);
}
 
void loop() {
  /*
 //Alle 5 Sekunden Farbe ändern
  long now = millis();
  if (now - lastMsg > 5000) {
    lastMsg = now;
     //Farbe LCD Display ändern:
      if(colorR > 100){
          colorR = 5;
          colorG = 255;
          colorB = 5;
      }
      else if(colorG > 100){
          colorR = 5;
          colorG = 5;
          colorB = 255;
      }
      else if(colorB > 100){
          colorR = 255;
          colorG = 5;
          colorB = 5;
      }
       
       lcd.clear();
       lcd.setRGB(colorR, colorG, colorB);
       lcd.setCursor(0, 0); 
       lcd.print("moxd lab IoTree");
       lcd.setCursor(0, 1);
 
       int x = (int) lastMsg % 10000;
        
       sprintf(buf, "Akt.Millis:%4d\0", x);
       lcd.print(buf);
       
  } */

  if(buttonPressed > 0) {
    digitalWrite(ledPinRed, HIGH);
    digitalWrite(ledPinGreen, LOW);
    lcd.print("Raum belegt");
    lcd.setRGB(205,0,0);
  }
  else
  {
      digitalWrite(ledPinRed, LOW);
      digitalWrite(ledPinGreen, HIGH);
      lcd.print("Raum frei");
      lcd.setRGB(0, 255, 0);
      //lcd.setCursor(0,2);
      //lcd.print(WiFi.localIP());
  }
  delay(500);
  lcd.clear();
 
}