#include "HygroThermoSensor.h"


void HygroThermoSensor::setup(ESP8266WebServer* server, WebSocketsServer* socket) {
  BaseHandler::setup(server, socket);
  readSensor();
  _ticker.attach_ms(_interval, tick, this);

  server->on(_route, [this, server]() {
    this->success(_data);
  });
}

void HygroThermoSensor::loop() {
  if (_dirty) {
    _dirty = false;
    readSensor();
  }
}

void HygroThermoSensor::update() {
  String json;
  StaticJsonBuffer<200> buff;
  JsonObject& data = buff.createObject();
  data["temperature"] = _temperature;
  data["humidity"] = _humidity;
  data.printTo(json);
  _data = json;

  broadcast(_route, _data);
}

void HygroThermoSensor::readSensor() {
  if (_sensor->read() && _sensor->getError() == DHT_ERROR_NONE) {
    bool changed = false;

    float temperature = _sensor->getTemperature();
    if (!isnan(temperature) && temperature != _temperature) {
      _temperature = temperature;
      changed = true;
    }
    float humidity = _sensor->getHumidity();
    if (!isnan(humidity) && humidity != _humidity) {
      _humidity = humidity;
      changed = true;
    }
    
    if (changed) {
      update();
    }
  }
}

void HygroThermoSensor::tick(HygroThermoSensor* obj) {
  obj->_dirty = true;
}
