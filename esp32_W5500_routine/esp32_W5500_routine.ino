#include <SPI.h>
#include <Ethernet.h>
#include <EthernetUdp.h>

// 定义W5500的CS引脚
#define W5500_CS 5

// 初始化W5500
EthernetClient client;
EthernetUDP Udp;
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

// 初始化ESP32-WROOM-32U的IP和端口
IPAddress local_IP(192, 168, 4, 110);
unsigned int local_port = 8888;

// 初始化上位机的IP和端口
IPAddress remote_IP(192, 168, 4, 115);
unsigned int remote_port = 8888;

// 初始化缓冲区
char packetBuffer[255];

void setup() {
  // 初始化串口
  Serial.begin(9600);

  // 初始化W5500
  Ethernet.init(W5500_CS);
  Ethernet.begin(mac, local_IP);

  // 输出连接信息
  Serial.println("");
  Serial.println("Ethernet connected");
  Serial.println("IP address: ");
  Serial.println(Ethernet.localIP());

  // 初始化UDP
  Udp.begin(local_port);
}

void loop() {
  // 发送UDP数据包
  sprintf(packetBuffer, "你好，上位机");
  Udp.beginPacket(remote_IP, remote_port);
  Udp.write(packetBuffer);
  Udp.endPacket();

  // 接收UDP数据包
  int packetSize = Udp.parsePacket();
  if (packetSize) {
    Serial.print("Received packet of size ");
    Serial.println(packetSize);
    int len = Udp.read(packetBuffer, 255);
    if (len > 0) {
      packetBuffer[len] = 0;
    }
    Serial.println("Contents:");
    Serial.println(packetBuffer);
  }

  // 延迟1秒
  delay(1000);
}
