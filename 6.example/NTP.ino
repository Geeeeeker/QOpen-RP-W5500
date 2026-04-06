// NTP.ino - 通过 NTP 服务器同步时间并每秒打印
// 需要安装 NTPClient 库

#include <SPI.h>
#include <Ethernet.h>
#include <EthernetUdp.h>
#include <NTPClient.h>

// W5500 配置同前
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 137, 144);
IPAddress gateway(192, 168, 137, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress dns(8, 8, 8, 8);

#define W5500_CS   17
#define W5500_RST  20

EthernetUDP udp;
NTPClient timeClient(udp, "ntp1.aliyun.com", 8 * 3600, 60000); // UTC+8，更新间隔60秒

void setup() {
  Serial.begin(115200);
  initW5500();
  
  timeClient.begin();
  // 手动设置错误的时间（模拟 RTC 未同步）
  // Arduino 没有内置 RTC，可通过设置编译时间 __TIME__ 等，但这里忽略
  Serial.println("Before NTP sync: (no RTC)");
  showTime();
  
  Serial.println("Syncing time...");
  timeClient.update();
  Serial.println("After NTP sync:");
  showTime();
}

void loop() {
  showTime();
  delay(1000);
}

void showTime() {
  timeClient.update();  // 可选，定期更新
  String formattedTime = timeClient.getFormattedTime();
  String date = timeClient.getFormattedDate();
  Serial.println(date + " " + formattedTime);
}

void initW5500() {
  SPI.setRX(16); SPI.setTX(19); SPI.setSCK(18); SPI.setCS(W5500_CS);
  SPI.begin();
  pinMode(W5500_RST, OUTPUT);
  digitalWrite(W5500_RST, LOW); delay(10); digitalWrite(W5500_RST, HIGH); delay(100);
  Ethernet.init(W5500_CS);
  Ethernet.begin(mac, ip, dns, gateway, subnet);
  while (Ethernet.linkStatus() == LinkOFF) delay(500);
  Serial.print("IP: "); Serial.println(Ethernet.localIP());
}
