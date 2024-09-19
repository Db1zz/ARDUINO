#include <LiquidCrystal.h>

const int rs = 10, en = 9, d4 = 8, d5 = 7, d6 = 6, d7 = 5;

LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

int   old_sensor_val = 0;
int   brightness = 0;
int   com_input = 0;

void clearSerialBuffer()
{
  while (Serial.available() > 0)
  {
    Serial.read();
  }
}

void  update_display(int brightness)
{
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Hello Comrades");
    lcd.setCursor(0, 1);
    lcd.print("brightness: " + String(brightness));
}

bool  is_new_signal_changed(int new_signal, int old_signal)
{
    return (abs(new_signal - old_signal) > 1);
}

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  pinMode(A3, OUTPUT);
  analogWrite(A3, 100);
  lcd.print("Hello Comrades");
}

void loop() {
 int    new_sensor_val = analogRead(A0) / 4;
 if (is_new_signal_changed(new_sensor_val, old_sensor_val)) {
    old_sensor_val = new_sensor_val;
    brightness = old_sensor_val;
    update_display(brightness);
  }

  if (Serial.available() > 0) {
    com_input = Serial.parseInt();
    brightness = com_input % 256;
    update_display(brightness);
    clearSerialBuffer();
  }
  
 float  voltage = new_sensor_val * (5.0 / 1023.0);


 analogWrite(11, brightness);
 Serial.println(brightness);
 delay(200);


}
