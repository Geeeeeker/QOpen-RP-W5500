// motor.ino - 通过 PWM 控制电机转速，并控制两个方向引脚
// 假设：方向引脚8、9，PWM引脚7

const int dirPin1 = 8;
const int dirPin2 = 9;
const int pwmPin = 7;

void setup() {
  pinMode(dirPin1, OUTPUT);
  pinMode(dirPin2, OUTPUT);
  pinMode(pwmPin, OUTPUT);
  
  analogWriteResolution(16);  // 16-bit PWM
}

void loop() {
  // 正转：50% 占空比，频率 1kHz
  digitalWrite(dirPin1, HIGH);
  digitalWrite(dirPin2, LOW);
  analogWriteFreq(1000);      // 设置 PWM 频率 (仅适用于 ESP32，RP2040 需要底层)
  analogWrite(pwmPin, 32768);  // 50% duty (65535*0.5)
  delay(5000);
  
  // 反转：25% 占空比，频率 500Hz
  digitalWrite(dirPin1, LOW);
  digitalWrite(dirPin2, HIGH);
  analogWriteFreq(500);
  analogWrite(pwmPin, 16384);  // 25% duty
  delay(5000);
}
