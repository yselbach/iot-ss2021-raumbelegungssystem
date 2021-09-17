#include <stdio.h>
#include <Arduino.h>
#include "credentials.h"
#include <Wire.h>                 
#include "rgb_lcd.h"
#include <WiFi.h>
#include <PubSubClient.h>
 
#define I2C_SDA 17   //GPIO 17 anstelle Standard GPIO 21 fuer SDA
#define I2C_SCL 16   //GPIO 16 anstelle Standard GPIO 22 fuer SCL
//define fuer das LCD RGB Backlight
rgb_lcd lcd;

//mqtt
WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;
int ledState = 0;

#define motion_in "esp32/motion_in"
#define motion_out "esp32/motion_out"
#define inTopic "esp32/inTopic"
#define outTopic "esp32/outTopic"

//
 
#define MOTION_AUSSEN 33 //Bewegungssensor aussen an der Tuere
#define MOTION_INNEN 26  //Bewegungssensor innen an der Tuere
int ledPinRed = 22;
int ledPinGreen = 18;
int anzahlPersonen = 0;
int InnenZaehler = 0; //Zaehler fuer den Innensensor
int AussenZaehler = 0; //Zaehler fuer den Aussensensor
//char pubPers;


//Bestimmung, wie viele Personen im Raum, welche Richtung sie gehen und ob sie sich umentscheiden
void tuerschranke(){
  //String msg="";
  //char msgPers[50];

//Aussenzaehler wird erhoeht. 3 Moeglichkeiten
//1. beide Zaehler auf Null => Aussenzaehler erhoehen
//2. Innenzaehler auf Eins => Person hat zuerst schon den Innenzaehler ausgeloest und nun aussen, daher Laufrichtung aus dem Raum 
//3. Aussenzaehler auf Eins => Person hat zweimal den Aussensensor beruehrt, hat sich wohl in der Tuere doch umentschieden und ist wieder gegangen
if((digitalRead(MOTION_AUSSEN)==HIGH) && (digitalRead(MOTION_INNEN)==LOW)){
  if((InnenZaehler==0) && (AussenZaehler==0)){
      AussenZaehler++;
      Serial.print("Aussenzaehler wurde erhoeht\n");
  } else if((InnenZaehler==1) && (AussenZaehler==0)){    //Person verlaesst den Raum
      AussenZaehler++;
      anzahlPersonen--;

      //client.publish(motion_out, "4" , true);
      Serial.print("Eine Person hat den Raum verlassen\n");
  } else if((InnenZaehler==0) && (AussenZaehler==1)){     //Person bleibt aus dem Raum
      Serial.print("Person wollte doch nicht in den Raum und ist umgedreht\n");
      AussenZaehler=0;
  }


//msg.toCharArray(msgPers,50);
//char pubPers = anzahlPersonen + '0';
//sprintf("%c", pubPers);
//client.publish(motion_out, String(anzahlPersonen).c_str(), true);
}

//Innenzaehler wird erhoeht. 3 Moeglichkeiten
//1. beide Zaehler auf Null => Innenzaehler erhoehen
//2. Aussenzaehler auf Eins => Person hat zuerst schon den Aussenzaehler ausgeloest und nun innen, daher Laufrichtung in den Raum 
//3. Innenzaehler auf Eins => Person hat zweimal den Innensensor beruehrt, hat sich wohl in der Tuere doch umentschieden und ist im Raum geblieben
if((digitalRead(MOTION_AUSSEN)==LOW) && (digitalRead(MOTION_INNEN)==HIGH)){
  
    if((InnenZaehler==0) && (AussenZaehler==0)){
      InnenZaehler++;
      Serial.print("Innenzaehler wurde erhoeht\n");
} else if((InnenZaehler==0) && (AussenZaehler==1)){     //Person geht in den Raum
      InnenZaehler++;
      anzahlPersonen++;
      Serial.print("Eine Person ist eingetreten\n");
      
} else if((InnenZaehler==1) && (AussenZaehler==0)){     //Person bleibt im Raum
      Serial.print("Die Person ist doch im Raum geblieben\n");
      InnenZaehler=0;
}

}
//wenn beide Sensoren ausgeloest wurden, Zahlen wieder auf null
if((AussenZaehler==1) && (InnenZaehler==1)){
  InnenZaehler=0;
  AussenZaehler=0;
}
//Fehlerbehebung, falls Sensoren doch einmal falsche Signale aufnehmen. Es koennen nicht weniger als Null Leute im Raum sein
if(anzahlPersonen<0){
  anzahlPersonen=0;
}
}

//Wifi Verbindung herstellen mit Anzeige auf LCD Display
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

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}


void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP32Client")) {
      Serial.println("connected");
      client.publish("esp32/test", "hello");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

//setup Methode, WifiSetup starten, Display resetten, alle Sensoren definieren auf welche Pins sie achten und Interrupts bestimmen
void setup() {
   Serial.begin(9600);
 
  Wire.begin(I2C_SDA, I2C_SCL);  
  WifiSetup();

  client.setServer(mqttserver, 1883);
  client.setCallback(callback);
   
   //client.publish("esp32/test", "hello");
  lcd.begin(16, 2);
  lcd.clear();
  lcd.setRGB(255, 255, 255);
  lcd.print("starten ...");
  delay(1000);

  pinMode(ledPinRed, OUTPUT);
  pinMode(ledPinGreen, OUTPUT);
  pinMode(MOTION_AUSSEN, INPUT);
  pinMode(MOTION_INNEN, INPUT);
    //Serial.begin(9600);

  attachInterrupt(MOTION_AUSSEN, tuerschranke, CHANGE);
  attachInterrupt(MOTION_INNEN, tuerschranke, CHANGE);
}
 
//In der Loop wird die Ausgabe des Displays definiert. Die Farbbestimmung, ob Personen im Raum sind oder nicht und die Anzeige
//fuer das WIFI Setup
void loop() {

  if(!client.connected()){
      reconnect();
  }
  client.loop();

  if(anzahlPersonen > 0) {
 
//char c = anzahlPersonen +'0';


    digitalWrite(ledPinRed, HIGH);
    digitalWrite(ledPinGreen, LOW);
    lcd.print("Raum belegt");
    lcd.setRGB(205,0,0);
    lcd.setCursor(0,2);
    if(anzahlPersonen==1){
            client.publish(motion_in , String(anzahlPersonen).c_str());
      lcd.printf("%d Person im Raum", anzahlPersonen);

      //char pubPers = anzahlPersonen + '0';
      //sprintf(pubPers, "%d");
      //client.publish(motion_in , pubPers);
    }
    if(anzahlPersonen>1){
            client.publish(motion_in , String(anzahlPersonen).c_str());
      lcd.printf("%d Pers. im Raum", anzahlPersonen);

    }
  }
  else
  {
      digitalWrite(ledPinRed, LOW);
      digitalWrite(ledPinGreen, HIGH);
      client.publish(motion_in , String(anzahlPersonen).c_str());
      lcd.print("Raum frei");
      lcd.setRGB(0, 255, 0);
      lcd.setCursor(0,2);
      lcd.print(WiFi.localIP());
  }

  //client.publish(motion_out, String(AussenZaehler).c_str() , true);
 // client.publish("esp32/test", "hello world");

  //client.publish(motion_in, String(InnenZaehler).c_str() , true);

  delay(500);
  lcd.clear();
 
}








