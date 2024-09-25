#include <Servo.h>

Servo servo_motor;
int eat = 2;
int code = 1;
int sleep = 0;
int servo = 3;

constexpr int eat_degrees = 0;
constexpr int code_degrees = 90;
constexpr int sleep_degrees = 180;

#define INITIAL_POS 0

void setup()
{
	Serial.begin(9600);
  servo_motor.attach(servo);
  servo_motor.write(INITIAL_POS);
}

int to_percent(int val)
{
  return (map(val, 0, 1023, 0, 100));
}

void  print(char *str, int sensor_val)
{
  Serial.print(str);
  Serial.println(sensor_val);
}

int read_and_compare()
{
    int eat_output = analogRead(eat);
    int code_output = analogRead(code);
    int sleep_output = analogRead(sleep);
    print("eat: ", to_percent(eat_output));
    print("code: ", to_percent(code_output));
    print("sleep: ", to_percent(sleep_output));
    if (eat_output > code_output && eat_output > sleep_output)
      return (eat_degrees);
    else if (code_output > sleep_output)
      return (code_degrees);
    else
      return (sleep_degrees);
}

void loop()
{
  int degree = read_and_compare();
  servo_motor.write(degree);
  delay(1000);
}
