#include "InputSwitch.h"
#include <ezTime.h>

void InputSwitch::setup(ESP8266WebServer* server, WebSocketsServer* socket) {
  BaseHandler::setup(server, socket);
  pinMode(_pin, INPUT_PULLUP);
  readState(true);
  _ticker.attach_ms(_interval, tick, this);

  server->on(_route, [this, server]() {
    this->success(this->_data);
  });

  server->on(_route + "/state", [this, server]() {
    this->success(this->_data);
  });
}

void InputSwitch::loop() {
  if (_dirty) {
    _dirty = false;
    readState();
  }
}

void InputSwitch::readState(bool force) {
  bool state = digitalRead(_pin);
  if (force || state != _state) {
    _state = state;
    _updated = now();
    update();
  }
}

bool InputSwitch::getState() {
  return _state;
}

void InputSwitch::tick(InputSwitch* obj) {
  obj->_dirty = true;
}

void InputSwitch::update() {
  String json;
  StaticJsonBuffer<200> buff;
  JsonObject& data = buff.createObject();
  data["state"] = _state;
  data["updated"] = _updated;
  data.printTo(json);
  _data = json;
  broadcast(_route, _data);
  BaseHandler::update();
}
