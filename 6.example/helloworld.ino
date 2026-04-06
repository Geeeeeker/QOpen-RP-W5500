// helloworld.ino - 通过 UART0 每秒输出 "Hello World"

void setup() {
  Serial.begin(115200);  // 使用 USB 虚拟串口或 UART0
  // 如需使用物理 UART0 (GP0, GP1)，需额外配置，此处使用默认 Serial
}

void loop() {
  Serial.println("Hello World");
  delay(1000);
}
