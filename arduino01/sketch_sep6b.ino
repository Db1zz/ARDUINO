int GREEN = 13;
int RED = 12;
int YELLOW = 12;
int BUTTON = 11;

int buttonState = LOW;
bool btn_switch = false;


void setup() {
  pinMode(GREEN, OUTPUT);
  pinMode(RED, OUTPUT);
  pinMode(YELLOW, OUTPUT);
  pinMode(BUTTON, INPUT);
  digitalWrite(GREEN, HIGH);
  digitalWrite(RED, LOW);
  digitalWrite(YELLOW, LOW);
  digitalWrite(BUTTON, HIGH);
  // digitalWrite(GREEN, HIGH);
}

void loop() {
  if (btn_switch == false)
  {
    digitalWrite(GREEN, HIGH);
    digitalWrite(RED, LOW);
    digitalWrite(YELLOW, LOW);
    if (digitalRead(BUTTON) == LOW)
      btn_switch = true;
  }
  while (btn_switch == true)
  {
    digitalWrite(GREEN, LOW);
    digitalWrite(RED, HIGH);
    digitalWrite(YELLOW, HIGH);
    delay(200);
    digitalWrite(RED, LOW);
    digitalWrite(YELLOW, LOW);
    delay(200);
  }
}
