// TCP.ino - TCP 回环服务器 (默认) 或客户端
// 使用 W5500 以太网模块，静态 IP 192.168.137.100:5050

#include <SPI.h>
#include <Ethernet.h>

// 以太网配置
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 137, 100);
IPAddress gateway(192, 168, 137, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress dns(8, 8, 8, 8);

// W5500 引脚
#define W5500_CS   17
#define W5500_RST  20

EthernetServer server(5050);  // TCP 服务器端口
EthernetClient client;

bool isServer = true;  // true=服务器模式, false=客户端模式

void setup() {
  Serial.begin(115200);
  
  // 初始化 W5500
  initW5500();
  
  if (isServer) {
    server.begin();
    Serial.println("TCP Server started on port 5050");
  } else {
    // 客户端模式：连接到目标 IP
    IPAddress dest(192, 168, 137, 114);
    if (client.connect(dest, 5050)) {
      Serial.println("Connected to server");
    } else {
      Serial.println("Connection failed");
    }
  }
}

void loop() {
  if (isServer) {
    // 服务器模式：接受客户端并回环数据
    EthernetClient newClient = server.available();
    if (newClient) {
      Serial.println("New client connected");
      while (newClient.connected()) {
        if (newClient.available()) {
          String data = newClient.readString();
          Serial.print(data);
          newClient.print(data);  // 回环发送
        }
      }
      newClient.stop();
      Serial.println("Client disconnected");
    }
  } else {
    // 客户端模式：发送并接收回环数据
    if (client.connected()) {
      if (client.available()) {
        String data = client.readString();
        Serial.print(data);
        client.print(data);
      }
    } else {
      Serial.println("Disconnected. Reconnecting...");
      client.stop();
      IPAddress dest(192, 168, 137, 114);
      client.connect(dest, 5050);
      delay(5000);
    }
  }
}

void initW5500() {
  SPI.setRX(16);   // MISO
  SPI.setTX(19);   // MOSI
  SPI.setSCK(18);  // SCK
  SPI.setCS(W5500_CS);
  SPI.begin();
  
  pinMode(W5500_RST, OUTPUT);
  digitalWrite(W5500_RST, LOW);
  delay(10);
  digitalWrite(W5500_RST, HIGH);
  delay(100);
  
  Ethernet.init(W5500_CS);
  Ethernet.begin(mac, ip, dns, gateway, subnet);
  
  Serial.print("IP address: ");
  Serial.println(Ethernet.localIP());
}
