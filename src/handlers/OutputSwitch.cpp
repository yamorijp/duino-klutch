#include "OutputSwitch.h"


void OutputSwitch::setup(ESP8266WebServer* server, WebSocketsServer* socket) {
  BaseHandler::setup(server, socket);
  pinMode(_pin, OUTPUT);
  off();
  update();

  server->on(_route + "/on", [this, server](){
    this->on();
    this->success(this->_data);
  });

  server->on(_route + "/off", [this, server](){
    this->off();
    this->success(this->_data);
  });

  server->on(_route + "/toggle", [this, server](){
    this->_state ? this->off() : this->on();
    this->success(this->_data);
  });

  server->on(_route + "/state", [this, server]() {
    this->success(this->_data);
  });
}

void OutputSwitch::update() {
  String json;
  StaticJsonBuffer<200> buff;
  JsonObject& data = buff.createObject();
  data["state"] = this->_state;
  data["updated"] = this->_updated;
  data.printTo(json);
  _data = json;

  broadcast(_route, _data);
}

void OutputSwitch::on() {
  digitalWrite(_pin, LOW);
  if (!_state) {
    _state = true;
    _updated = time(NULL);
    update();
  }
}

void OutputSwitch::off() {
  digitalWrite(_pin, HIGH);
  if(_state) {
    _state = false;
    _updated = time(NULL);
    update();
  }
}
