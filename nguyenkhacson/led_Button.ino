#define led_Blink 26
#define button 4
    
void setup() {
    pinMode(led_Blink, OUTPUT);
    pinMode(button, INPUT);
    }
    
void loop() {
    bool currentState = digitalRead(button);
      if (currentState == HIGH) {
        digitalWrite(led_Blink, HIGH);
      } else {
        digitalWrite(led_Blink, LOW);
      }
    }
