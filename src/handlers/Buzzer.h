#ifndef BUZZER_H_
#define BUZZER_H_

#include "BaseHandler.h"
#include <Ticker.h>

class BeepParameters : public Parameters {
public:
  uint16_t freq;
  uint16_t duration;
  uint8_t beeps;
  uint16_t cycles;
  bool validate(ESP8266WebServer* server);  
};

/**
 * ビープ出力
 */ 
class Buzzer : public BaseHandler {
private:
  String _route;
  uint8_t _pin;
  String _data;

public:
  Buzzer(String route, uint8_t pin) :
    _route(route), _pin(pin), _data("{}") {};
  void setup(ESP8266WebServer* server, WebSocketsServer* socket);
  void beep(uint16_t freq=7040, uint16_t duration=25, uint8_t beeps=1, uint16_t cycles=1);
  void loop();
  void clear();
  static void tick(Buzzer* obj);

  HandlerInfo getHandlerInfo() {
    HandlerInfo info;
    info.type = "Buzzer";
    info.group = "device";
    info.resource = _route;
    return info;
  }

  void collectDataTo(JsonArray* json) {
    JsonObject& obj = json->createNestedObject();
    obj["resource"] = _route;
    obj["data"] = RawJson(_data);
  }
};

#endif
