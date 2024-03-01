#define BLYNK_PRINT Serial

/* Fill in information from Blynk Device Info here */
//#define BLYNK_TEMPLATE_ID           "TMPxxxxxx"
//#define BLYNK_TEMPLATE_NAME         "Device"
//#define BLYNK_AUTH_TOKEN            "YourAuthToken"
#define BLYNK_TEMPLATE_ID "TMPL6eEhuW5QF"
#define BLYNK_TEMPLATE_NAME "SmartHome"
#define BLYNK_AUTH_TOKEN "msrU2miWth64Y8AZllCxoWqvKzKbISsI"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <stdlib.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
//https://sgp1.blynk.cloud/external/api/update?token=msrU2miWth64Y8AZllCxoWqvKzKbISsI&v0=0

int buttonBlynk1;
int buttonBlynk2;

const int relay1 = 13;
const int relay2 = 12;
// Save the light state to sync to blynk
int rl1State = 0;
int rl2State = 0;
// button pins
const int button1 = 32;
const int button2 = 33;
// process debounce for 2 button
long unsigned int nowTime = 0;
long unsigned int preTime = 0;
unsigned int pressTime = 150;
long unsigned int nowTime2 = 0;
long unsigned int preTime2 = 0;
unsigned int pressTime2 = 150;
// sensor pins
const int airSensorPin = 35;
const int buzzer =23;
//digital pin to control on off power relay module
const int pinRELAY = 14;

char ssid[] = "abc";
char pass[] = "11111111";
LiquidCrystal_I2C lcd(0x27, 16, 2);
BlynkTimer timer;
void setup() {
  Serial.begin(9600);
  // pimode
  pinMode(relay2, OUTPUT);
  pinMode(relay1, OUTPUT);
  pinMode(button1, INPUT_PULLDOWN);
  pinMode(button2, INPUT_PULLDOWN);
  pinMode(airSensorPin, INPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(pinRELAY, OUTPUT);
  // lcd setup
  lcd.init();
  lcd.backlight();
  lcd.setCursor(3, 1);
  lcd.print("Starting...");
  // prepare for connect wifi
  digitalWrite(relay1, HIGH);  //off
  digitalWrite(relay2, HIGH);  //off
  //digitalWrite(pỉnRELAY, LOW); // turn off relay module
  // begin blynk
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  timer.setInterval(500L, updateSensorValue);
}


void loop() {
  Blynk.run();
  timer.run();
  buttonControlRelay();
}

void updateSensorValue() {
  // air quality
  int airQuality = analogRead(airSensorPin);
  airQuality = map(airQuality, 0, 4100, 0, 100);

  lcd.setCursor(0, 0);
  String a = "";
  if (airQuality < 70) {
    a = "   Air: Good   ";
  } else {
    a = "   Air: Bad   ";
    beepWarning() ;
  }
  lcd.print(a);
  Blynk.virtualWrite(V2, airQuality);

}
BLYNK_WRITE(V0) {
  buttonBlynk1 = param.asInt();
  if (buttonBlynk1 == 1) {
    digitalWrite(relay1, LOW);
    rl1State = 1;
  } else {
    digitalWrite(relay1, HIGH);
    rl1State = 0;
  }
  Blynk.virtualWrite(V0, rl1State);
}

BLYNK_WRITE(V1) {
  buttonBlynk2 = param.asInt();
  if (buttonBlynk2 == 1) {
    digitalWrite(relay2, LOW);
    rl2State = 1;
  } else {
    digitalWrite(relay2, HIGH);
    rl2State = 0;
  }
  Blynk.virtualWrite(V1, rl2State);
}



void buttonControlRelay() {
  int stateButton1 = digitalRead(button1);
  int stateButton2 = digitalRead(button2);
  nowTime = millis();
  if (nowTime - preTime > pressTime) {
    if (stateButton1 == 1 && rl1State == 1) {  // while led1 wass turning on
      digitalWrite(relay1, HIGH);              //turn off
      rl1State = 0;
      Blynk.virtualWrite(V0, rl1State);
    } else if (stateButton1 == 1 && rl1State == 0) {
      digitalWrite(relay1, LOW);
      rl1State = 1;
      Blynk.virtualWrite(V0, rl1State);
    }

    preTime = nowTime;
  }

  nowTime2 = millis();
  if (nowTime2 - preTime2 > pressTime2) {
    if (stateButton2 == 1 && rl2State == 1) {  // while led2 wass turning on
      digitalWrite(relay2, HIGH);              //turn off
      rl2State = 0;
      Blynk.virtualWrite(V1, rl2State);
    } else if (stateButton2 == 1 && rl2State == 0) {
      digitalWrite(relay2, LOW);
      rl2State = 1;
      Blynk.virtualWrite(V1, rl2State);
    }
    preTime2 = nowTime2;
  }
}

BLYNK_CONNECTED() {
  digitalWrite(pinRELAY, HIGH);
   Blynk.virtualWrite(V0, rl1State);
   Blynk.virtualWrite(V1, rl2State);

  lcd.setCursor(1, 1);
  lcd.print(WiFi.localIP());

  lcd.setCursor(0, 0);
  lcd.print("   Connected    ");
}

BLYNK_DISCONNECTED() {
  digitalWrite(pinRELAY, LOW);
  lcd.setCursor(3, 1);
  lcd.print("Disconnected");
}
void beepWarning() {
  tone(buzzer, 3000);
  delay(2000);
  noTone(buzzer);
}
