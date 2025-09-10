#define LED_Blink 26

void setup() {
  pinMode(LED_Blink, OUTPUT);
}

void loop() {
  digitalWrite(LED_Blink, HIGH); 
  delay(500);                  
  digitalWrite(LED_Blink, LOW); 
  delay(500);                  
}