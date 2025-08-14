// int blueLed = 26;
// int greenLed = 27;
// int button = 4;
// int count = 0;


// void setup() {
//   pinMode(blueLed, OUTPUT);
//   pinMode(greenLed, OUTPUT);
//   pinMode(button, INPUT);  

// }

// void brightLed(int countBlink){
//   for(int i = 0; i < countBlink; i++){
//     digitalWrite(blueLed, HIGH);
//     delay(200);
//     digitalWrite(blueLed, LOW);
//     delay(200);
//   }
   
// }


// void loop() {
//   bool currentBtnState = digitalRead(button) == LOW;
//   if (currentBtnState && !lastBtnState) {
//     count++;
//     brightLed(count);
//   }
//   lastBtnState = currentBtnState;
// }

// // thay đổi trạng thái
// // void loop() {
// //   if(digitalRead(button) == HIGH){
// //     count++;
// //     brightLed(count);
// //   }

// // }

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

  //  Chỉ tăng khi chuyển từ LOW -> HIGH 
  if (currentState == HIGH && lastButtonState == LOW) {
    pressCount++;
    blinkLed(blue_Led, pressCount); 
  }

  lastButtonState = currentState;
}

void blinkLed(int ledPin, int times) {
  for (int i = 0; i < times; i++) {
    digitalWrite(ledPin, HIGH);
    delay(500);
    digitalWrite(ledPin, LOW);
    delay(500);
  }
}