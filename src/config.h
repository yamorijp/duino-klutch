#ifndef CONFIG_H_
#define CONFIG_H_

#include <Arduino.h>

struct Config {
  // timezone
  static const uint16_t timezone = 3600 * 9;  // UTC+9

  // ntp servers
  static constexpr const char* ntp1 = "ntp.nict.jp";
  static constexpr const char* ntp2 = "ntp.jst.mfeed.ad.jp";

  // server port
  static const uint8_t httpPort = 80;
  static const uint8_t wsPort = 81;

  // access point name / mDNS hostname
  static constexpr const char* hostname = "duino-k";

  // access point password
  static constexpr const char* apPasswd = "8086";

  // active handlers
  static const bool enableSite = true;
  static const bool enableRelay = true;
  static const bool enableMatrix = true;
  static const bool enableIrx = true;
  static const bool enableIr = false;        // confilict with irx
  static const bool enableMotion = true;
  static const bool enableCds = true;
  static const bool enableHt = true;
  static const bool enableFourdigit = true;
  static const bool enableRgb = false;

  static const uint8_t matrixNumDisplays = 2;

  // debug output to serial
  static const bool enableDebug = false;

  // reset saved wifi settings
  static const bool resetWifiSettings = false;
};

#endif
