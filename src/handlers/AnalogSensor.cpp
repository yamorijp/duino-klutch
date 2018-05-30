#include "AnalogSensor.h"


void AnalogSensor::setup(ESP8266WebServer* server, WebSocketsServer* socket) {
  BaseHandler::setup(server, socket);
  pinMode(_pin, INPUT);
  readSensor();
  _ticker.attach_ms(_interval, tick, this);

  server->on(_route, [this, server]() {
    this->readSensor();
    this->success(_data);
  });
}

void AnalogSensor::loop() {
  if (_dirty) {
    _dirty = false;
    readSensor();
  }
}

void AnalogSensor::update() {
  _data = toJsonObject("value", _value);
  broadcast(_route, _data);
}

void AnalogSensor::readSensor() {
  float value = analogRead(_pin);
  if (value != _value) {
    _value = value;
    update();
  }
}

void AnalogSensor::tick(AnalogSensor* obj) {
  obj->readSensor();
}
