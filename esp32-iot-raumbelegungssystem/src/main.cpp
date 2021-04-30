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
int touchSensor = 26; // TouchSensor, außen vor der Tür
int button= 18;       // Button, innen vor der Tür
int ledPinRed = 14;
int ledPinGreen = 33;
int anzahlPersonen = 0;
int buttonZaehler = 0;
int touchZaehler = 0;


void TouchSensorClicked() {
  buttonPressed = digitalRead(touchSensor);
}

void tuerschranke(){


if((digitalRead(touchSensor)==HIGH) && (digitalRead(button)==LOW)){
  if((buttonZaehler==0) && (touchZaehler==0)){
      touchZaehler++;
      Serial.print("touchZaehler wurde erhöht\n");
  } else if((buttonZaehler==1) && (touchZaehler==0)){    //Person verlässt den Raum
      touchZaehler++;
      anzahlPersonen--;
      Serial.print("Eine Person hat den Raum verlassen\n");
  } else if((buttonZaehler==0) && (touchZaehler==1)){
      Serial.print("Person wollte doch nicht in den Raum und ist umgedreht\n");
      touchZaehler=0;
  }
}

if((digitalRead(touchSensor)==LOW) && (digitalRead(button)==HIGH)){
    if((buttonZaehler==0) && (touchZaehler==0)){
      buttonZaehler++;
      Serial.print("buttonZaehler wurde erhöht\n");
} else if((buttonZaehler==0) && (touchZaehler==1)){
      buttonZaehler++;
      anzahlPersonen++;
      Serial.print("Eine Person ist eingetreten\n");
} else if((buttonZaehler==1) && (touchZaehler==0)){
      Serial.print("Die Person ist doch im Raum geblieben\n");
      buttonZaehler=0;
}}
if((touchZaehler==1) && (buttonZaehler==1)){
  buttonZaehler=0;
  touchZaehler=0;
}
if(anzahlPersonen<0){
  anzahlPersonen=0;
}
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
   Serial.begin(9600);
 
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

  pinMode(ledPinRed, OUTPUT);
  pinMode(ledPinGreen, OUTPUT);
  pinMode(touchSensor, INPUT);

  attachInterrupt(touchSensor, tuerschranke, CHANGE);
  attachInterrupt(button, tuerschranke, CHANGE);
}
 
void loop() {


  if(anzahlPersonen > 0) {
    digitalWrite(ledPinRed, HIGH);
    digitalWrite(ledPinGreen, LOW);
    lcd.print("Raum belegt");
    lcd.setRGB(205,0,0);
    lcd.setCursor(0,2);
    if(anzahlPersonen==1){
      lcd.printf("%d Person im Raum", anzahlPersonen);
    }
    if(anzahlPersonen>1){
      lcd.printf("%d Pers. im Raum", anzahlPersonen);
    }
  }
  else
  {
      digitalWrite(ledPinRed, LOW);
      digitalWrite(ledPinGreen, HIGH);
      lcd.print("Raum frei");
      lcd.setRGB(0, 255, 0);
      lcd.setCursor(0,2);
      lcd.print(WiFi.localIP());
  }
  delay(500);
  lcd.clear();
 
}