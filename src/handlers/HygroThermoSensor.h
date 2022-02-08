#ifndef HYGROTHERMOSENSOR_H_
#define HYGROTHERMOSENSOR_H_

#include "BaseHandler.h"
#include <memory>
#include <dhtnew.h>
#include <Ticker.h>

/**
 * 湿温度センサーモジュール (DHT11)
 */
class HygroThermoSensor : public BaseHandler {
private:
  String _route;
  uint8_t _pin;
  uint16_t _interval;
  float _temperature;
  float _humidity;
  String _data;
  bool _dirty;
  Ticker _ticker;
  std::unique_ptr<DHTNEW> _sensor;

public:
  HygroThermoSensor(String route, uint8_t outPin, uint16_t interval = 10000) :
    _route(route), _pin(outPin), _interval(interval), _temperature(0.0), _humidity(0.0), _data("{}"), _dirty(false)
  {
    _sensor.reset(new DHTNEW(outPin));
  }
  void update();
  void loop();
  void setup(ESP8266WebServer* server, WebSocketsServer* socket);
  void readSensor();
  static void tick(HygroThermoSensor* obj);

  HandlerInfo getHandlerInfo() {
    HandlerInfo info;
    info.type = "HygroThermoSensor";
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
