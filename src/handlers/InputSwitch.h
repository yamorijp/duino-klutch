#ifndef INPUTSWITCH_H_
#define INPUTSWITCH_H_


#include "BaseHandler.h"
#include <Ticker.h>
#include <time.h>

/**
 * スイッチ入力
 */ 
class InputSwitch : public BaseHandler {

private:
  uint8_t _pin;
  String _route;
  Ticker _ticker;
  bool _state;
  time_t _updated;
  uint16_t _interval;
  bool _dirty;
  String _data;
  void update();

public:
  InputSwitch(String route, uint8_t pin, uint16_t intervalMs=3000) {
    _pin = pin;
    _route = route;
    _interval = intervalMs;
    _updated = time(NULL);
  }
  void setup(ESP8266WebServer* server, WebSocketsServer* socket);
  void loop();
  void readState();
  static void tick(InputSwitch* obj);

  HandlerInfo getHandlerInfo() {
    HandlerInfo info;
    info.type = "InputSwitch";
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
