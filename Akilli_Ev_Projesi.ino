#include <SoftwareSerial.h>

#include <SimpleTimer.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

#define RED_LED 3
#define GREEN_LED 5
#define BLUE_LED 6


LiquidCrystal_I2C lcd(0x27, 16, 2);

SoftwareSerial ss(0, 1);

Servo myservo;


const int lm35 = A0;
const int buzzerPin = 8;

//float lm35_volt_carpan = 5000 / 1024;
float lm35_volt_carpan = 4.8828;

void setup() {
  //analog girişleri tanımlamasak da olur normalde.

  //SICAKLIK INPUT OLARAK AYARLANIR
  pinMode(lm35, INPUT);

  //RGB OUTPUT OLARAK AYARLANIR
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);

  pinMode(buzzerPin, OUTPUT);

  Serial.begin(9600);

  ss.begin(9600);

  lcd.begin();
  lcd.backlight();
  lcd.print("Hosgeldiniz!");

  myservo.attach(10);
}

float derece = 0;
float lm35_prev = 0;

int servo_derece = 0;

void loop() {

  if (ss.available()) {
    char komut = ss.read();

    if (komut == '1') {
      Serial.println("Birinci komut geldi!");
      lcdSicaklikGoster();
    } else if (komut == '2') {
      Serial.println("2. komut geldi!");
      isiklariAcKapat();
    } else if (komut == '3') {
      Serial.println("3. komut geldi!");
      perdeleriAcKapat();
    } else if (komut == '4') {
      Serial.println("4. komut geldi!");
      alarmCal();
    }

  }
  else {

    float lm35 = analogRead(lm35);
    float mv = lm35 * lm35_volt_carpan;
    float derece = mv / 10;

    if (derece > 32.5 && derece < 40) {//ilk derece çok büyük bir değer olduğundan 40 ile sınırladık..
      if (servo_derece != 180) {// perde hali hazırda 180 derece açık değilse..
        //servo_derece = 180;
        //Serial.println("Sicakkkk..");
        //        lcd.clear();
        //        lcd.print("Cok sicakkkk..");
        //        myservo.write(servo_derece);
        //        delay(100);
      }
    }
  }


}

void lcdSicaklikGoster() {
  //TODO - sicakliği gerçek zamanlı ekranda göster..
  float lm35 = analogRead(lm35);
  float mv = lm35 * lm35_volt_carpan;

  float derece = mv / 10;

  String str = "Sicaklik : ";
  String sicaklikStr = str + derece;

  lcd.clear();
  lcd.print(sicaklikStr);
}

int isikDurum = 0; //0 -kapalı , 1 - kırmızı, 2 - mavi

void isiklariAcKapat() {
  if (isikDurum == 0) { // 0 ise kapalıyıd, kırmızıya dönüştür..
    analogWrite(RED_LED, 255);
    analogWrite(GREEN_LED, 0);
    analogWrite(BLUE_LED, 0);
    isikDurum = 1;

    Serial.print("LED : MAVI");
    lcd.clear();
    lcd.print("LED : MAVI");

  } else if (isikDurum == 1) {
    analogWrite(RED_LED, 0);
    analogWrite(GREEN_LED, 0);
    analogWrite(BLUE_LED, 255);
    isikDurum = 2;

    Serial.print("LED : KIRMIZI");
    lcd.clear();
    lcd.print("LED : KIRMIZI");

  } else if (isikDurum == 2) {
    analogWrite(RED_LED, 0);
    analogWrite(GREEN_LED, 0);
    analogWrite(BLUE_LED, 0);
    isikDurum = 0;


    Serial.print("LED : KAPALI");
    lcd.clear();
    lcd.print("LED : KAPALI");
  }
}



void perdeleriAcKapat() {

  if (servo_derece == 0) {
    servo_derece = 180;
    
    Serial.println("Havalandirma aciliyor...");
    lcd.clear();
    lcd.print("Havalandirma");
    lcd.setCursor(2, 6);
    lcd.print(" - ACIK -");
  } else {
    servo_derece = 0;
    
    Serial.println("Havalandirma durduruluyor...");
    lcd.clear();
    lcd.print("Havalandirma");
    lcd.setCursor(2, 6);
    lcd.print(" - KAPALI -");
  }


  myservo.write(servo_derece);
  delay(200);

}

void alarmCal() {

  Serial.print("Polis aranıyor...");
  for (int i = 0; i < 3; i++) {
    digitalWrite(buzzerPin, HIGH);
    delay(800);
    digitalWrite(buzzerPin, LOW);
    delay(200);
  }

  digitalWrite(buzzerPin, LOW);
  lcd.clear();
  lcd.print("Polis araniyor...");

}


void sicaklikGonder(long lm35) {
  if (lm35 == lm35_prev)
    return;

  if (lm35 + 1 == lm35_prev || lm35 - 1 == lm35_prev)
    return;
  if (lm35 + 2 == lm35_prev || lm35 - 2 == lm35_prev)
    return;
  if (lm35 + 3 == lm35_prev || lm35 - 3 == lm35_prev)
    return;

  float mv = lm35 * lm35_volt_carpan;

  float derece = mv / 10;

  Serial.print(lm35);
  Serial.print(" derece : ");
  Serial.println(derece);

  lm35_prev = lm35;
}
//0x3F
