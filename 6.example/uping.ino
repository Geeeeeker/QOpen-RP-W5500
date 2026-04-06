// uping.ino - 通过原始套接字实现 ICMP ping
// 注意：Arduino Ethernet 库不支持原始套接字，此示例仅演示概念，实际不可运行
// 替代方案：使用系统 ping 命令或外部库

void setup() {
  Serial.begin(115200);
  Serial.println("Ping functionality requires raw socket support, not available in Arduino Ethernet.");
  Serial.println("You can use a PC to ping the device instead.");
}

void loop() {
  // 空循环
}
