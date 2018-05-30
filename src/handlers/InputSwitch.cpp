#include "InputSwitch.h"


void InputSwitch::setup(ESP8266WebServer* server, WebSocketsServer* socket) {
  BaseHandler::setup(server, socket);
  pinMode(_pin, INPUT);
  readState();
  _ticker.attach_ms(_interval, tick, this);

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

void InputSwitch::update() {
  String json;
  StaticJsonBuffer<200> buff;
  JsonObject& data = buff.createObject();
  data["state"] = this->_state;
  data["updated"] = this->_updated;
  data.printTo(json);
  _data = json;

  broadcast(_route, _data);
}

void InputSwitch::readState() {
  bool state = digitalRead(_pin);
  if (state != _state) {
    _state = state;
    _updated = time(NULL);
    update();
  }
}

void InputSwitch::tick(InputSwitch* obj) {
  obj->_dirty = true;
}
