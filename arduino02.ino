#include <LiquidCrystal.h>

const int rs = 10, en = 9, d4 = 8, d5 = 7, d6 = 6, d7 = 5;

LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

int current_potentio_meter = 0;
int    brightness = 0;
int com_input = 0;

void clearSerialBuffer()
{
  while (Serial.available() > 0)
  {
    Serial.read();
  }
}

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  pinMode(A3, OUTPUT);
  analogWrite(A3, 100);
//  lcd.display();
  lcd.print("Hello Comrades");
}

void loop() {
 int    sensor_val = analogRead(A0);
 if (current_potentio_meter != sensor_val)
 {
  current_potentio_meter = sensor_val;
  brightness = current_potentio_meter/4;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Hello Comrades");
  lcd.setCursor(0, 1);
  lcd.print(String("brightness: ") + String(brightness));
 }
 else if (Serial.available() > 0)
 {
  com_input = Serial.parseInt();
  brightness = com_input;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Hello Comrades");
  lcd.setCursor(0, 1);
  lcd.print(String("brightness: ") + String(brightness));
 }
 clearSerialBuffer();

 

 float  voltage = sensor_val * (5.0 / 1023.0);


 analogWrite(11, brightness);
 Serial.println(brightness);
 delay(200);


}
