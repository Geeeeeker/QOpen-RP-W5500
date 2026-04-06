// FTP.ino - 简易 FTP 服务器框架
// 完整实现非常复杂，这里给出基础命令响应（USER, PASV, LIST, RETR, STOR）

#include <SPI.h>
#include <Ethernet.h>
#include <SD.h>

// 以太网配置同前
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
IPAddress ip(192, 168, 137, 100);
IPAddress gateway(192, 168, 137, 1);
IPAddress subnet(255, 255, 255, 0);

#define FTP_CTRL_PORT 21
#define FTP_DATA_PORT 13333

EthernetServer ftpServer(FTP_CTRL_PORT);
EthernetServer dataServer(FTP_DATA_PORT);
EthernetClient dataClient;

String cwd = "/";   // current working directory on SD card

void setup() {
  Serial.begin(115200);
  initW5500();
  initSDCard();
  
  ftpServer.begin();
  dataServer.begin();
  Serial.println("FTP server started on port 21");
}

void loop() {
  EthernetClient client = ftpServer.available();
  if (client) {
    handleFTP(client);
  }
}

void handleFTP(EthernetClient &client) {
  client.println("220 Hello, this is RP2040 FTP server.");
  while (client.connected()) {
    if (client.available()) {
      String cmd = client.readStringUntil('\n');
      cmd.trim();
      String command = cmd.substring(0, cmd.indexOf(' '));
      String param = cmd.substring(cmd.indexOf(' ') + 1);
      command.toUpperCase();
      
      if (command == "USER") {
        client.println("331 OK");
      } else if (command == "PASS") {
        client.println("230 Login successful");
      } else if (command == "PASV") {
        client.println("227 Entering Passive Mode (192,168,137,100,52,21)"); // 示例
        dataClient = dataServer.available();
      } else if (command == "LIST") {
        client.println("150 Opening data connection");
        listDirectory(cwd, dataClient);
        dataClient.stop();
        client.println("226 Transfer complete");
      } else if (command == "RETR") {
        client.println("150 Opening data connection");
        sendFile(param, dataClient);
        dataClient.stop();
        client.println("226 Transfer complete");
      } else if (command == "STOR") {
        client.println("150 Ready to receive");
        receiveFile(param, dataClient);
        client.println("226 Transfer complete");
      } else if (command == "QUIT") {
        client.println("221 Goodbye");
        break;
      } else {
        client.println("502 Command not implemented");
      }
    }
  }
  client.stop();
}

void listDirectory(String path, EthernetClient &data) {
  File root = SD.open(path);
  while (true) {
    File entry = root.openNextFile();
    if (!entry) break;
    String line = (entry.isDirectory() ? "d" : "-");
    line += "rw-r--r-- 1 owner group ";
    line += String(entry.size()) + " Jan 1 12:00 " + String(entry.name()) + "\r\n";
    data.print(line);
    entry.close();
  }
  root.close();
}

void sendFile(String filename, EthernetClient &data) {
  File f = SD.open(cwd + filename);
  if (!f) return;
  while (f.available()) {
    data.write(f.read());
  }
  f.close();
}

void receiveFile(String filename, EthernetClient &data) {
  File f = SD.open(cwd + filename, FILE_WRITE);
  if (!f) return;
  while (data.connected() && data.available()) {
    f.write(data.read());
  }
  f.close();
}

void initW5500() {
  SPI.setRX(16); SPI.setTX(19); SPI.setSCK(18); SPI.setCS(17);
  SPI.begin();
  pinMode(20, OUTPUT); digitalWrite(20, LOW); delay(10); digitalWrite(20, HIGH); delay(100);
  Ethernet.init(17);
  Ethernet.begin(mac, ip, gateway, subnet);
  Serial.print("IP: "); Serial.println(Ethernet.localIP());
}

void initSDCard() {
  SPI1.setRX(12); SPI1.setTX(11); SPI1.setSCK(10); SPI1.setCS(13);
  SPI1.begin();
  if (!SD.begin(13, SPI1)) {
    Serial.println("SD init failed");
  } else {
    Serial.println("SD mounted");
  }
}
