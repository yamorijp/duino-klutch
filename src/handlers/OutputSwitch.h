#ifndef OUTPUTSWITCH_H_
#define OUTPUTSWITCH_H_

#include "BaseHandler.h"


class LockParameters : public Parameters {
public:
  bool state;
  bool validate(ESP8266WebServer* server);
};

/**
 * スイッチ出力
 */
class OutputSwitch : public BaseHandler {
private:
  String _route;
  uint8_t _pin;
  String _data;
  time_t _updated;
  bool _state;
  bool _locking;

public:
  OutputSwitch(String route, uint8_t pin) :
    _route(route), _pin(pin), _data("{}"), _updated(0), _state(false), _locking(false) {};
  void setup(ESP8266WebServer* server, WebSocketsServer* socket);
  void on(bool force=false);
  void off(bool force=false);
  void toggle(bool force=false);
  bool state();
  void lock(bool state);
  void clear();
  void update();

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
