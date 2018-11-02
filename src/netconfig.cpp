#include "netconfig.h"

String DEVICE_TYPE = "gh_5e8661030bb8";
//char* DEVICE_ID = "gh_5e8661030bb8_e282ca678250674b";
String DEVICE_ID = "4Dclass_WifiModule_";

/**
   初始化提示灯和连接上一次wifi
*/
void initNetConfig() {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(2, true);
  WiFi.begin();
  //初始化设备ID
  DEVICE_ID = DEVICE_ID + WiFi.softAPmacAddress().c_str();
  Serial.println("");
}

/**
   检查连接状态
*/
bool checkConnect () {
  for (int i = 0; i < 10; i++) {
    int state = WiFi.status();
    if (state == WL_CONNECTED) {
      Serial.println("AutoConnect Success");
      Serial.printf("SSID:%s\r\nPSW:%s\r\n", WiFi.SSID().c_str(), WiFi.psk().c_str());
      Serial.printf("now listening at ip %s, udp port %d\r\n", WiFi.localIP().toString().c_str(), DEFAULT_LAN_PORT);
      WiFi.printDiag(Serial);
      return true;
    } else {
      Serial.println("AutoConnect Waiting...");
      delay(500);
    }
  }
  Serial.println("AutoConnect Faild" );
  return false;
}

/**
   启动airkiss
*/
bool startAirkiss() {
  Serial.println("start airkiss...");
  WiFi.mode(WIFI_STA);
  WiFi.beginSmartConfig();
  bool LIGHT = HIGH;
  while (1) {
    Serial.print(".");
    digitalWrite(2, LIGHT);
    if (WiFi.smartConfigDone()) {
      Serial.println("");
      Serial.println("airkiss Success");
      Serial.printf("SSID:%s\r\nPSW:%s\r\n", WiFi.SSID().c_str(), WiFi.psk().c_str());
      WiFi.setAutoConnect(true);  // 设置自动连接
      break;
    }
    if (LIGHT == LOW) {
      LIGHT = HIGH;
    } else {
      LIGHT = LOW;
    }
    delay(500); // 这个地方一定要加延时，否则极易崩溃重启
  }
  digitalWrite(2, HIGH);
}

/**
   删除wifi连接记录
*/
bool deleteConfig() {
  ESP.eraseConfig();
  delay(100);
  ESP.reset();
  ESP.restart();
}

/**
   获取mac地址
*/
String getMacAddress() {
  return DEVICE_ID;
}

//==================================================================================================
WiFiUDP udp;

char udpPacket[255];
//用于缓存回包的数据缓冲区，也可以为局部变量
uint8_t lan_buf[255];
uint16_t lan_buf_len;

const airkiss_config_t airkissConf = {
  (airkiss_memset_fn)&memset,
  (airkiss_memcpy_fn)&memcpy,
  (airkiss_memcmp_fn)&memcmp,
  0
};

/**
   开启微信局域网发现
*/
void startDiscover() {
  udp.begin(DEFAULT_LAN_PORT);

  while (1) {
    int packetSize = udp.parsePacket();
    if (packetSize) {
      Serial.printf("Received %d bytes from %s, port %d\r\n", packetSize, udp.remoteIP().toString().c_str(), udp.remotePort());
      int len = udp.read(udpPacket, 255);
      if (len > 0) {
        udpPacket[len] = 0;
      }
      Serial.printf("UDP packet contents: %2x\r\n", udpPacket);
      if (strcmp(udpPacket, "reset") == 0) {
        deleteConfig();
      }
      int ret = airkiss_lan_recv(udpPacket, len, &airkissConf);
      int packret;
      switch (ret) {
        //接收到发现设备请求数据包
        case AIRKISS_LAN_SSDP_REQ:
          Serial.println("--->>> find device");
          Serial.println(DEVICE_TYPE);
          Serial.println(DEVICE_ID);
          lan_buf_len = sizeof(lan_buf);
          //打包数据
          packret = airkiss_lan_pack(AIRKISS_LAN_SSDP_RESP_CMD, (char *)DEVICE_TYPE.c_str(), (char *)DEVICE_ID.c_str(), 0, 0, lan_buf, &lan_buf_len, &airkissConf);
          if (packret != AIRKISS_LAN_PAKE_READY) {
            Serial.println("pack lan packet error");
            return;
          }
          Serial.printf("udp pack is %2x,length is %d\r\n", lan_buf, sizeof(lan_buf));
          udp.beginPacket(udp.remoteIP(), udp.remotePort());
          Serial.println(udp.write(lan_buf, sizeof(lan_buf)));
          udp.endPacket();
          break;
        default:
          Serial.println("pack is not ssdq req");
          break;
      }
    }
  }
}
