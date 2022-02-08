#ifndef INPUTSWITCH_H_
#define INPUTSWITCH_H_

#include "BaseHandler.h"
#include <Ticker.h>

/**
 * スイッチ入力
 * HIGH/LOW入力のあるデジタルスイッチ。PIRモジュールなども含む。
 */
class InputSwitch : public BaseHandler {

protected:
  String _route;

private:
  uint8_t _pin;
  uint16_t _interval;
  String _data;
  time_t _updated;
  bool _state;
  bool _dirty;
  Ticker _ticker;

public:
  InputSwitch(String route, uint8_t pin, uint16_t interval=1000) :
    _route(route), _pin(pin), _interval(interval), _data("{}"), _updated(0), _state(false), _dirty(false) {}
  void setup(ESP8266WebServer* server, WebSocketsServer* socket);
  void loop();
  void readState(bool force=false);
  static void tick(InputSwitch* obj);
  bool getState();
  void update();

  virtual HandlerInfo getHandlerInfo() {
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
