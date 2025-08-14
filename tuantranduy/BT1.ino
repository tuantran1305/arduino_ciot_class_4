int blue_Led = 26;
int button =   4;

int pressCount = 0;
bool lastButtonState = LOW;

void setup() {
  pinMode(blue_Led, OUTPUT);
  pinMode(button, INPUT);
}

void loop() {
  bool currentState = digitalRead(button);
  if (currentState == HIGH && lastButtonState == LOW) {
    pressCount++;
    blinkLed(blue_Led, pressCount); 
  }

  lastButtonState = currentState;
}

void blinkLed(int ledPin, int times) {
  for (int i = 0; i < times; i++) {
    digitalWrite(ledPin, HIGH);
    delay(200);
    digitalWrite(ledPin, LOW);
    delay(200);
  }
}