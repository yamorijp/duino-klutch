#ifndef HYGROTHERMOSENSOR_H_
#define HYGROTHERMOSENSOR_H_

#include "BaseHandler.h"
#include <memory>
#include <Adafruit_Sensor.h>
#include <DHTNew.h>
#include <Ticker.h>

/**
 * 湿温度センサーモジュール (DHT11)
 */
class HygroThermoSensor : public BaseHandler {
private:
  String _route;
  uint8_t _pin;
  DHTModel_t _model;
  std::unique_ptr<DHT> _sensor;
  uint16_t _interval;
  float _temperature;
  float _humidity;
  Ticker _ticker;
  String _data;
  bool _dirty;
  void update();

public:
  HygroThermoSensor(String route, uint8_t outPin, DHTModel_t model = DHT_MODEL_DHT11, uint16_t intervalMs = 30000) {
    _route = route;
    _pin = outPin;
    _model = model;
    _interval = intervalMs;
    _sensor.reset(new DHT(outPin, model));
    _sensor->begin();
  }
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
