int blueLed = 26;
int greenLed = 27;
int buttonPin = 4;

#define onLed()      digitalWrite(greenLed, HIGH)
#define offLed()     digitalWrite(greenLed, LOW)
#define readButton() digitalRead(buttonPin)

unsigned long prevMillis = 0;
const unsigned long interval = 500; 
bool blueLedState = false;

void setup() {
  pinMode(greenLed, OUTPUT);
  pinMode(blueLed, OUTPUT);
  pinMode(buttonPin, INPUT);
}

void loop() {
  blinkLed(blueLed); 
  if (readButton() == HIGH) { 
    onLed();
  } else {
    offLed();
  }
}

void blinkLed(int ledPin) {
  unsigned long currentMillis = millis();
  if (currentMillis - prevMillis >= interval) {
    prevMillis = currentMillis;
    blueLedState = !blueLedState; 
    digitalWrite(ledPin, blueLedState);
  }
}
