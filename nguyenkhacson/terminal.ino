// Khai báo chân LED
const int BLUE_PIN  = 26;   // LED xanh nối vào GPIO26
const int GREEN_PIN = 27;   // LED xanh lá nối vào GPIO27

// Thời gian nháy LED khi ở chế độ blink (ms)
const unsigned long BLINK_INTERVAL_MS = 300;

// Các biến điều khiển
bool blinkMode = false;        // true = chế độ nháy, false = điều khiển thủ công
unsigned long lastToggle = 0;  // thời điểm lần cuối đổi trạng thái LED
bool blinkState = false;       // trạng thái hiện tại của chế độ blink

// Hàm setup() chạy 1 lần khi ESP32 khởi động
void setup() {
  // Cấu hình chân GPIO là OUTPUT
  pinMode(BLUE_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);

  // Ban đầu tắt hết LED
  digitalWrite(BLUE_PIN, LOW);
  digitalWrite(GREEN_PIN, LOW);

  // Khởi tạo UART tốc độ 115200 để nhập lệnh
  Serial.begin(115200);
  Serial.println("UART LED Controller ready.");   // Thông báo sẵn sàng
  Serial.println("Commands: onLedBlue, offLedBlue, onLedGreen, offLedGreen, blinkLed"); 
}

// Hàm loop() chạy lặp lại liên tục 
void loop() {
  // Nếu đang ở chế độ blink thì luân phiên bật/tắt LED
  if (blinkMode) {
    unsigned long now = millis();  // lấy thời gian hiện tại
    if (now - lastToggle >= BLINK_INTERVAL_MS) {  // đủ 300ms thì đổi trạng thái
      lastToggle = now;
      blinkState = !blinkState;  // đảo trạng thái

      if (blinkState) {
        // Khi blinkState = true: BẬT LED xanh lá, TẮT LED xanh
        digitalWrite(GREEN_PIN, HIGH);
        digitalWrite(BLUE_PIN, LOW);
      } else {
        // Khi blinkState = false: BẬT LED xanh, TẮT LED xanh lá
        digitalWrite(GREEN_PIN, LOW);
        digitalWrite(BLUE_PIN, HIGH);
      }
    }
  }

  // Kiểm tra có dữ liệu nhập từ UART không
  if (Serial.available()) {
    String cmd = Serial.readStringUntil('\n');  // đọc cả dòng đến khi gặp ký tự xuống dòng
    cmd.trim();  // loại bỏ khoảng trắng hoặc ký tự thừa

    if (cmd.length() == 0) return;  // nếu lệnh rỗng thì bỏ qua

    // Các lệnh điều khiển
    if (cmd == "onLedBlue") {
      blinkMode = false;  // tắt chế độ blink
      digitalWrite(BLUE_PIN, HIGH);   // bật LED xanh
      Serial.println("BLUE ON (GPIO26)");
    } 
    else if (cmd == "offLedBlue") {
      blinkMode = false;
      digitalWrite(BLUE_PIN, LOW);    // tắt LED xanh
      Serial.println("BLUE OFF (GPIO26)");
    } 
    else if (cmd == "onLedGreen") {
      blinkMode = false;
      digitalWrite(GREEN_PIN, HIGH);  // bật LED xanh lá
      Serial.println("GREEN ON (GPIO27)");
    } 
    else if (cmd == "offLedGreen") {
      blinkMode = false;
      digitalWrite(GREEN_PIN, LOW);   // tắt LED xanh lá
      Serial.println("GREEN OFF (GPIO27)");
    } 
    else if (cmd == "blinkLed") {
      // Bật chế độ nháy luân phiên
      blinkMode = true;
      lastToggle = millis();   // reset thời điểm bắt đầu
      blinkState = false;      // khởi tạo trạng thái ban đầu
      digitalWrite(BLUE_PIN, HIGH);
      digitalWrite(GREEN_PIN, LOW);
      Serial.println("BLINK MODE (alternate every 300ms)");
    } 
    else {
      // Nếu nhập lệnh sai
      Serial.print("Unknown command: ");
      Serial.println(cmd);
      Serial.println("Valid: onLedBlue, offLedBlue, onLedGreen, offLedGreen, blinkLed");
    }
  }
}
