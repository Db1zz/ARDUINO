int GREEN = 13;
int RED = 12;
int YELLOW = 10;
int BUTTON = 11;

int buttonState = LOW;
bool btn_switch = false;
bool was_green = true;
int green = 45;
int rellow = 46;
int current_color = green;

unsigned long previousMillisRed = 0;
unsigned long previousMillisYellow = 0;
const long intervalRed = 500;
const long intervalYellow = 2000;

void setup() {
  pinMode(GREEN, OUTPUT);
  pinMode(RED, OUTPUT);
  pinMode(YELLOW, OUTPUT);
  pinMode(BUTTON, INPUT_PULLUP);
  digitalWrite(GREEN, HIGH);
  digitalWrite(RED, LOW);
  digitalWrite(YELLOW, LOW);
  digitalWrite(BUTTON, HIGH);
}

void play_colors(int current_color) {
  if (current_color == green) {
    digitalWrite(RED, LOW);
    digitalWrite(YELLOW, LOW);
    digitalWrite(GREEN, HIGH);
    was_green = true;
  } 
  else if (current_color == rellow) {
    digitalWrite(GREEN, LOW);
    was_green = false;
    
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillisRed >= intervalRed) {
      previousMillisRed = currentMillis;
      if (digitalRead(RED) == LOW) {
        digitalWrite(RED, HIGH);
      } else {
        digitalWrite(RED, LOW);
      }
    }
    if (currentMillis - previousMillisYellow >= intervalYellow) {
      previousMillisYellow = currentMillis;
      if (digitalRead(YELLOW) == LOW) {
        digitalWrite(YELLOW, HIGH);
      } else {
        digitalWrite(YELLOW, LOW);
      }
    }
  }
}

void change_colors() {
  if (was_green) {
    current_color = rellow;
  } else {
    current_color = green;
  }
}

void loop() {
  if (digitalRead(BUTTON) == LOW)
  {
    delay(50);
    while (digitalRead(BUTTON) == LOW);
    change_colors();
  }
  play_colors(current_color);
}
