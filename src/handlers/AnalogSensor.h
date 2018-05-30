#ifndef ANALOGSENSOR_H_
#define ANALOGSENSOR_H_

#include "BaseHandler.h"
#include <Ticker.h>

/**
 * アナログセンサー入力
 */
class AnalogSensor : public BaseHandler {
private:
  uint8_t _pin;
  String _route;
  Ticker _ticker;
  uint16_t _interval;
  float _value;
  bool _dirty;
  String _data;
  void update();

public:
  AnalogSensor(String route, uint8_t aPin, uint16_t intervalMs=3000) {
    _pin = aPin;
    _route = route;
    _interval = intervalMs;
  }
  void setup(ESP8266WebServer* server, WebSocketsServer* socket);
  void loop();
  void readSensor();
  static void tick(AnalogSensor* obj);
  HandlerInfo getHandlerInfo() {
    HandlerInfo info;
    info.type = "AnalogSensor";
    info.group = "sensor";
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
