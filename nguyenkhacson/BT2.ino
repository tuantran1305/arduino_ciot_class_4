int green_Led = 27;
int blue_Led = 26;
int red_Led = 25;
int button = 21;

int pressCount = 0;        
bool lastButtonState = LOW;

#define onLed(ledPin)      digitalWrite(ledPin, HIGH)
#define offLed(ledPin)     digitalWrite(ledPin, LOW)
#define readButton()       digitalRead(button)

void setup() {
  pinMode(green_Led, OUTPUT);
  pinMode(blue_Led, OUTPUT);
  pinMode(red_Led, OUTPUT);
  pinMode(button, INPUT); // hoặc INPUT_PULLUP nếu nối GND
}

void loop() {
  bool currentState = readButton();
  
  // Khi nhấn button
  if (currentState == LOW && lastButtonState == HIGH) {
    pressCount++;
    if (pressCount > 3) pressCount = 0;
  }
  
  // Tắt tất cả LED
  offLed(green_Led);
  offLed(blue_Led);
  offLed(red_Led);
  
  // Sáng LED khi button đang được nhấn
  if (currentState == LOW) {
    if (pressCount == 1) onLed(green_Led);
    if (pressCount == 2) onLed(blue_Led);
    if (pressCount == 3) onLed(red_Led);
  }
  
  lastButtonState = currentState;
  delay(50);
}