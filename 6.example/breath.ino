// breath.ino - PWM 实现呼吸灯
// 注意：LED_BUILTIN 通常不支持 PWM (Pico 的 25 号引脚不支持硬件 PWM)
// 此处改用 16 号引脚作为示例，你可以连接一个 LED 到 GPIO16

const int ledPin = 16;   // 支持 PWM 的引脚 (Pico 上所有 GPIO 均支持 PWM)
const int maxDuty = 65535;  // 16 位 PWM 最大值

void setup() {
  pinMode(ledPin, OUTPUT);
  analogWriteResolution(16);  // 设置 PWM 分辨率为 16 位
}

void loop() {
  // 渐亮
  for (int duty = 0; duty <= maxDuty; duty += 1000) {
    analogWrite(ledPin, duty);
    delay(10);
  }
  // 渐暗
  for (int duty = maxDuty; duty >= 0; duty -= 1000) {
    analogWrite(ledPin, duty);
    delay(10);
  }
}
