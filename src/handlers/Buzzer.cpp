#include <EasyBuzzer.h>
#include "Buzzer.h"
#include "utils/validate.h"


bool BeepParameters::validate(ESP8266WebServer* server) {
  String error;
  if (!integerRange(server->arg("freq"), 0, UINT16_MAX, false, error)) {
    errors->set("freq", error);
  }
  if (!integerRange(server->arg("duration"), 0, UINT16_MAX, false, error)) {
    errors->set("duration", error);
  }
  if (!integerRange(server->arg("beeps"), 0, 8, false, error)) {
    errors->set("beeps", error);
  }
  if (!integerRange(server->arg("cycles"), 0, UINT16_MAX, false, error)) {
    errors->set("cycles", error);
  }
  if (errors->size() == 0) {
    freq = argInt(server, "freq", 7040);
    duration = argInt(server, "duration", 25);
    beeps = argInt(server, "beeps", 1);
    cycles = argInt(server, "cycles", 1);
    return true;
  } else {
    return false;
  }
}

void Buzzer::setup(ESP8266WebServer* server, WebSocketsServer* socket) {
  BaseHandler::setup(server, socket);
  pinMode(_pin, OUTPUT);
  EasyBuzzer.setPin(_pin);

  server->on(_route, [this, server]() {
    this->success(this->_data);
  });

  server->on(_route + "/beep", [this, server](){
    BeepParameters params;
    if (params.validate(server)) {
      this->beep(params.freq, params.duration, params.beeps, params.cycles);
      this->success(this->_data);
    } else {
      this->fail(params.getErrors());
    }
  });

  server->on(_route + "/clear", [this, server] {
    this->clear();
    this->success(this->_data);
  });
}

void Buzzer::beep(uint16_t freq, uint16_t duration, uint8_t beeps, uint16_t cycles) {
  EasyBuzzer.beep(freq, duration, 50, beeps, 1000, cycles);
}

void Buzzer::clear() {
  EasyBuzzer.beep(3520, 0, 50, 0, 1000, 0);  // 新しいパラメーターでmSequenceを強制リセット
  EasyBuzzer.stopBeep();  // 38381 / 1976
}

void Buzzer::loop() {
  EasyBuzzer.update();
}
