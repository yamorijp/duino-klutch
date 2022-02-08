#include "OutputSwitch.h"
#include "utils/validate.h"
#include <ezTime.h>


bool LockParameters::validate(ESP8266WebServer* server) {
  String error;

  if (!oneOrZero(server->arg("state"), false, error)) {
    errors->set("state", error);
  }
  if (errors->size() == 0) {
    state = argInt(server, "state", 1);
    return true;
  }
  return false;
}


void OutputSwitch::setup(ESP8266WebServer* server, WebSocketsServer* socket) {
  BaseHandler::setup(server, socket);
  pinMode(_pin, OUTPUT);
  off(true);

  server->on(_route, [this, server]() {
    this->success(this->_data);
  });

  server->on(_route + "/on", [this, server](){
    this->on();
    this->success(this->_data);
  });

  server->on(_route + "/off", [this, server](){
    this->off();
    this->success(this->_data);
  });

  server->on(_route + "/toggle", [this, server](){
    this->toggle();
    this->success(this->_data);
  });

  server->on(_route + "/state", [this, server]() {
    this->success(this->_data);
  });

  server->on(_route + "/lock", [this, server]() {
    LockParameters params;
    if (params.validate(server)) {
      this->lock(params.state);
      this->success(this->_data);
    } else {
      this->fail(params.getErrors());
    }
  });

  server->on(_route + "/release", [this, server]() {
    this->success(this->_data);
  });
}

void OutputSwitch::on(bool force) {
  if (!force && _locking) return;
  digitalWrite(_pin, LOW);
  if (force || !_state) {
    _state = true;
    _updated = now();
    update();
  }
}

void OutputSwitch::off(bool force) {
  if (!force && _locking) return;
  digitalWrite(_pin, HIGH);
  if(force || _state) {
    _state = false;
    _updated = now();
    update();
  }
}

void OutputSwitch::toggle(bool force) {
  if (_locking) return;
  _state ? off(force) : on(force);
}

void OutputSwitch::lock(bool state) {
  if (_locking) return;
  state ? on(true) : off(true);
  _locking = true;
}

void OutputSwitch::clear() {
  _locking = false;
  toggle();
}

bool OutputSwitch::state() {
  return _state;
}

void OutputSwitch::update() {
  String json;
  StaticJsonBuffer<200> buff;
  JsonObject& data = buff.createObject();
  data["locking"] = _locking;
  data["state"] = _state;
  data["updated"] = _updated;
  data.printTo(json);
  _data = json;

  broadcast(_route, _data);
  BaseHandler::update();
}