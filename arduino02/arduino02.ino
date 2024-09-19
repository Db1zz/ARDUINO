#include <LiquidCrystal.h>

const int rs = 10, en = 9, d4 = 8, d5 = 7, d6 = 6, d7 = 5;
float  voltage = 0;
int   new_p_meter_val = 0;
int   old_p_meter_val = 0;
int   brightness = 0;
int   com_input = 0;

LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void clear_serial_buffer()
{
  while (Serial.available() > 0)
    Serial.read();
}

void  update_display(int brightness, int p_meter_val)
{
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("p_meter_val:" + String(p_meter_val));
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
 new_p_meter_val = analogRead(A0);
 if (is_new_signal_changed(new_p_meter_val, old_p_meter_val)) {
    old_p_meter_val = new_p_meter_val;
    brightness = old_p_meter_val / 4;
    update_display(brightness, old_p_meter_val);
  }
  if (Serial.available() > 0) {
    com_input = Serial.parseInt();
    brightness = com_input % 256;
    update_display(brightness, old_p_meter_val);
    clear_serial_buffer();
  }
  voltage = new_p_meter_val * (5.0 / 1023.0);
  analogWrite(11, brightness);
  Serial.println(brightness);
  delay(200);
}
