#ifndef ANALOGSENSOR_H_
#define ANALOGSENSOR_H_

#include "BaseHandler.h"
#include <Ticker.h>

/**
 * アナログセンサー入力
 */
class AnalogSensor : public BaseHandler {
private:
  String _route;
  uint8_t _pin;
  uint16_t _interval;
  float _value;
  bool _dirty;
  String _data;
  Ticker _ticker;

public:
  AnalogSensor(String route, uint8_t aPin, uint16_t interval=3000) :
    _route(route), _pin(aPin), _interval(interval), _value(0.0), _dirty(false), _data("{}") {}
  void setup(ESP8266WebServer* server, WebSocketsServer* socket);
  void update();
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
