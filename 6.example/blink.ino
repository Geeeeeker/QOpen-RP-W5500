// blink.ino - 控制内置 LED 闪烁
// 适用于 RP2040 (Pico) 或其他 Arduino 板

const int ledPin = LED_BUILTIN;  // 内置 LED 引脚 (Pico 上为 25)

void setup() {
  pinMode(ledPin, OUTPUT);
}

void loop() {
  digitalWrite(ledPin, HIGH);
  delay(500);
  digitalWrite(ledPin, LOW);
  delay(500);
}
