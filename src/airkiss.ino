#include "netconfig.h"

void setup() {
  Serial.begin(115200);
  initNetConfig();
  if (!checkConnect()) {
    startAirkiss();

//    startDiscover();
  }
}

void loop() {
  startDiscover();
//  for (int i = 0; i < 20; i++) {
//    Serial.println("loop");
//    delay(1000);
//  }
//  deleteConfig();
}
