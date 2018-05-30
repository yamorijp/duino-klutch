#ifndef OUTPUTSWITCH_H_
#define OUTPUTSWITCH_H_

#include <time.h>
#include "BaseHandler.h"

/**
 * スイッチ出力
 */ 
class OutputSwitch : public BaseHandler {
private:
  uint8_t _pin;
  String _route;
  bool _state;
  time_t _updated;
  String _data;
  void update();
public:
  OutputSwitch(String route, uint8_t pin) {
    _route = route;
    _pin = pin;
    _updated = time(NULL);
  };
  void setup(ESP8266WebServer* server, WebSocketsServer* socket);
  void on();
  void off();
  bool state();
  void clear() {
    off();
  }

  HandlerInfo getHandlerInfo() {
    HandlerInfo info;
    info.type = "OutputSwitch";
    info.group = "switch";
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
