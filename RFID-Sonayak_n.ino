
#include <SPI.h>
#include <RFID.h>
#include <Wire.h>
#include <AvrI2c_Greiman.h>
#include <LiquidCrystal_I2C_AvrI2C.h>
#include <Servo.h>

LiquidCrystal_I2C_AvrI2C lcd(0x27 , 16 , 2);

const byte kirmizi_led = 8;
const byte yesil_led = 7;
const byte buzzer = 5;
int bakiye = 50;
RFID rfid(10, 9);

int kart[5] = {171,201,234,13,133};

int kart1[5] = {251,178,78,210,213};
int kart2[5] = {22,70,53,44,73};



boolean pass = true;

Servo SG90;

void ID() {
  lcd.setCursor(0, 1);

  lcd.setCursor(0, 1);
  lcd.print(rfid.serNum[0]);
  lcd.print(".");
  lcd.print(rfid.serNum[1]);
  lcd.print(".");
  lcd.print(rfid.serNum[2]);
  lcd.print(".");
  lcd.print(rfid.serNum[3]);
  lcd.print(".");
  lcd.print(rfid.serNum[4]);
  
}

void ID_RST() {
  lcd.setCursor(0, 1);
  lcd.print("Kart ID: ");
  lcd.setCursor(0, 2);
  lcd.print("-");
}

void beepT() {
  for (int i = 0; i < 4; i++) {
    digitalWrite(buzzer, HIGH);
    delay(100);
    digitalWrite(buzzer, LOW);
    delay(25);
  }
}

void beepF(){
  for(int i = 0; i < 2; i++){
    digitalWrite(buzzer, HIGH);
    delay(225);
    digitalWrite(buzzer, LOW);
    delay(25);
  }
}

void setup()
{
  Serial.begin(9600);

  SPI.begin();
  lcd.begin();
  rfid.init();

  pinMode(kirmizi_led, OUTPUT);
  pinMode(yesil_led, OUTPUT);
  pinMode(buzzer, OUTPUT);
  
  SG90.attach(6);
}

void loop() {
  lcd.setCursor(0, 0);
  lcd.print("Kartinizi Okutunuz");
  ID_RST();
  SG90.write(180);
  pass = true;

  if (rfid.isCard()) {
    if (rfid.readCardSerial()) {
      Serial.print("Kart ID: ");
      Serial.print(rfid.serNum[0]);
      Serial.print(",");
      Serial.print(rfid.serNum[1]);
      Serial.print(",");
      Serial.print(rfid.serNum[2]);
      Serial.print(",");
      Serial.print(rfid.serNum[3]);
      Serial.print(",");
      Serial.println(rfid.serNum[4]);
       
    }

    for (int i = 0; i < 5; i++) {
      if ((rfid.serNum[i] != kart[i])&&(rfid.serNum[i] != kart1[i])&&(rfid.serNum[i] != kart2[i])) {
        pass = false;
      }
      else {
        pass = true;
      }
      }
     //if(rfid.serNum[0]==171&&rfid.serNum[1]==201&&rfid.serNum[2]==234&&rfid.serNum[3]==13&&rfid.serNum[4]==133){
     //bakiye=bakiye-5;   
    //}

    if (pass == false) {
      lcd.clear();
      ID();
      lcd.setCursor(0, 0);
      lcd.print("YANLIS KART!");
      Serial.println("izinsiz Giris");
      digitalWrite(kirmizi_led, HIGH);
      beepF();
      delay(1000);
      digitalWrite(kirmizi_led, LOW);
      lcd.clear();
      rfid.halt();
    }

    else {      
      lcd.clear();
      ID();
      SG90.write(13);
      lcd.setCursor(0, 0);
      lcd.print("IZIN VERILDI");
      Serial.println("izin Verildi");
      lcd.setCursor(1, 2);
      lcd.print("BAKİYE: ");
      digitalWrite(yesil_led, HIGH);
      beepT();
      delay(1000);
      digitalWrite(yesil_led, LOW);
      lcd.clear();
    
    }
    rfid.halt();
    
  }
}
