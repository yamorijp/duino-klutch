/**
 * duino-h 2022 profile
 */
#ifndef PROFILE_DUINO_H_H_
#define PROFILE_DUINO_H_H_


#include <Arduino.h>
#include <ezTime.h>

#include "handlers/BaseHandler.h"
#include "handlers/HandlerContainer.h"
#include "handlers/Site.h"
#include "handlers/OutputSwitch.h"
#include "handlers/Display.h"
#include "handlers/MotionInputSwitch.h"
#include "handlers/InputSwitch.h"
#include "handlers/Buzzer.h"
#include "handlers/Clock.h"


namespace config {
  // local posix timezone
  const char* tz = "JST-9";

  // ntp server
  const char* ntp = "ntp.nict.jp";

  // server port
  int httpPort = 80;
  int wsPort = 81;

  // hostname / ap password
  const char* hostname = "duino-h";
  const char* apPasswd = "8086";

  // static ip address settings
  bool enableStaticIp = true;
  const char* ipAddr = "192.168.1.10";
  const char* ipGateway = "192.168.1.1";
  const char* ipSubnetMask = "255.255.255.0";

  bool enableDebug = false;
  bool resetWifiSettings = false;
}

namespace profile {
  enum ACT {
    A_NOTHING = 0,
    A_SW1_OFF = 10,
    A_SW1_ON = 11,
    A_SW2_OFF = 20,
    A_SW2_ON = 21,
    A_BUZ_CLEAR = 30,
    A_BUZ_BEEP = 31,
    A_BUZ_BEEP2 = 32,
    A_BUZ_REPEAT = 33
  };

  Site site("");
  OutputSwitch pw1("/switch/pw1", D1);
  OutputSwitch pw2("/switch/pw2", D0);
  Display disp("/device/display", D3, D2);  // dio, clk
  MotionInputSwitch motion("/switch/motion", D5);
  InputSwitch sw("/switch/sw", D6);
  Buzzer buzzer("/device/buzzer", D7);
  Clock cl("/sensor/clock");

  Timezone* tz;
  bool dirty = false;
  std::function<void(void)> invalidate = []{
    dirty = true;
  };

  String formatTimer() {
    time_t remain = cl.getTimer();
    if (remain <= 0) return "T=--:--";
    if (remain >= 6000) return "T=99:59+";
    char buff[8];
    sprintf(buff, "T=%02d:%02d", remain / 60, remain % 60);
    return buff;
  }

  void render() {
    time_t utc = cl.getTime();
    time_t t = tz->tzTime(utc, UTC_TIME);
    String week = tz->dateTime(t, "D");
    week.toUpperCase();

    disp.display->clearBuffer();

    // hh:mm
    disp.display->setFont(u8g2_font_tenstamps_mn);
    disp.display->drawStr(2, 16, tz->dateTime(t, "H:i").c_str());

    disp.display->setFont(u8g2_font_chikita_tr);
    if (cl.hasTimer()) {
      // timer
      disp.display->drawStr(2, 30, formatTimer().c_str());
    } else {
      // week
      disp.display->drawStr(2, 30, week.c_str());
    }
    disp.display->drawStr(56, 30, tz->dateTime(t, "m.d").c_str());

    // indicators P1=pw1, P2=pw2
    if (pw1.state())
      disp.display->drawStr(104, 10, "P1");
    if (pw2.state())
      disp.display->drawStr(104, 28, "P2");

    // indicators T=timer, L=lock switch, P=pir motion
    if (cl.hasTimer())
      disp.display->drawStr(120, 10, "T");
    if (sw.getState())
      disp.display->drawStr(120, 19, "L");
    if (motion.getState())
      disp.display->drawStr(120, 28, "P");

    disp.display->sendBuffer();
  }

  void setup(HandlerContainer* handlers, Timezone* local) {
    tz = local;
    sw.onUpdate([]() {
      if (sw.getState()) {
        buzzer.beep(7040, 25, 2);
        pw1.lock(true);
      } else {
        pw1.clear();
      }
      dirty = true;
    });

    motion.onUpdate([]() {
      if (motion.getState()) {
        buzzer.clear();
        pw1.on();
        buzzer.beep(7040, 25, 1);
      } else {
        pw1.off();
      }
      dirty = true;
    });

    pw1.onUpdate(invalidate);
    pw2.onUpdate(invalidate);
    buzzer.onUpdate(invalidate);
    disp.onUpdate(invalidate);
    cl.onUpdate(invalidate);

    cl.onAction([](int action) {
      switch (action) {
        case A_NOTHING:
          break;
        case A_SW1_OFF:
          pw1.off();
          break;
        case A_SW1_ON:
          pw1.on();
          break;
        case A_SW2_OFF:
          pw2.off();
          break;
        case A_SW2_ON:
          pw2.on();
          break;
        case A_BUZ_CLEAR:
          buzzer.clear();
          break;
        case A_BUZ_BEEP:
          buzzer.beep(7040, 25, 1);
          break;
        case A_BUZ_BEEP2:
          buzzer.beep(7040, 25, 4);
          break;
        case A_BUZ_REPEAT:
          buzzer.beep(7040, 25, 4, 60);
          break;
      }
    });

    handlers->add("Display", &disp);
    handlers->add("Site", &site);
    handlers->add("Power1", &pw1);
    handlers->add("Power2", &pw2);
    handlers->add("Buzzer", &buzzer);
    handlers->add("Switch", &sw);
    handlers->add("Motion", &motion);
    handlers->add("Clock", &cl);
  }

  void loop() {
    if (dirty) {
      dirty = false;
      render();
    }
  }
}

#endif
