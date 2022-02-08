/**
 * duino-k 2022 profile
 */
#ifndef PROFILE_DUINO_K_H_
#define PROFILE_DUINO_K_H_


#include <Arduino.h>
#include <ezTime.h>

#include "handlers/BaseHandler.h"
#include "handlers/HandlerContainer.h"
#include "handlers/Site.h"
#include "handlers/FourDigitLed.h"
#include "handlers/MatrixLed.h"
#include "handlers/HygroThermoSensor.h"
#include "handlers/AnalogSensor.h"
#include "handlers/Clock.h"
#include "handlers/Buzzer.h"

// #include "handlers/OutputSwitch.h"
// #include "handlers/InputSwitch.h"
// #include "handlers/MotionInputSwitch.h"
// #include "handlers/RGBLed.h"
// #include "handlers/IRTransmitter.h"
// #include "handlers/MyHomeIRTransmitter.h"
// #include "handlers/Display.h"


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
  FourDigitLed four("/led/4digit", D2, D3);
  MatrixLed matrix("/led/matrix", D8, 2);  // D5=CLK, D7=DIN, D8=CS
  HygroThermoSensor dht("/sensor/ht", D6);
  Buzzer buzzer("/device/buzzer", D1);
  AnalogSensor brightness("/sensor/brightness", A0);
  Clock cl("/sensor/clock");

  Timezone* tz;
  bool dirty = false;
  std::function<void(void)> invalidate = []{
    dirty = true;
  };

  void render() {
    int8_t m = four.getMode();
    if (m == FOURDIGITLED_MODE_IDLE || m == FOURDIGITLED_MODE_CLOCK) {
      time_t utc = cl.getTime();
      four.clock(tz->hour(utc, UTC_TIME), tz->minute(utc, UTC_TIME));
    }
  }

  void setup(HandlerContainer* handlers, Timezone* local) {
    tz = local;

    cl.onUpdate(invalidate);
    four.onUpdate(invalidate);

    handlers->add("Site", &site);
    handlers->add("Clock", &cl);
    handlers->add("Matrix", &matrix);
    handlers->add("Temperature & Humidity", &dht);
    handlers->add("Buzzer", &buzzer);
    handlers->add("Brightness", &brightness);
    handlers->add("4 Digit", &four);
  }

  void loop() {
    if (dirty) {
      dirty = false;
      render();
    }
  }
}

#endif
