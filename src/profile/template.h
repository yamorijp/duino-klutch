
#ifndef PROFILE_TEMPLATE_H_
#define PROFILE_TEMPLATE_H_


#include <Arduino.h>
#include <ezTime.h>

#include "handlers/HandlerContainer.h"
#include "handlers/Site.h"

namespace config {
  // local posix timezone
  const char* tz = "JST-9";

  // ntp server
  const char* ntp = "ntp.nict.jp";

  // server port
  int httpPort = 80;
  int wsPort = 81;

  // hostname / ap password
  const char* hostname = "duino-k";
  const char* apPasswd = "8086";

  // static ip address settings
  bool enableStaticIp = true;
  const char* ipAddr = "192.168.1.9";
  const char* ipGateway = "192.168.1.1";
  const char* ipSubnetMask = "255.255.255.0";

  bool enableDebug = false;
  bool resetWifiSettings = false;
}

namespace profile {
  Site site("");

  void setup(HandlerContainer* handlers, Timezone* tz) {
    handlers->add("Site", &site);
  }

  void loop() {
  }
}

#endif
