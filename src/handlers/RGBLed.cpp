#include "RGBLed.h"
#include "utils/validate.h"


bool OnParameters::validate(ESP8266WebServer* server) {
  String error;
  String s = argStr(server, "data");
  if (s.startsWith("#")) {
    s = s.substring(1);
  }
  s = "ffffff" + s;
  s = s.substring(s.length() - 6);
  s.toLowerCase();
  if (!hexPattern(s, false, error)) {
    errors->set("data", error);
  }

  if (errors->size() == 0) {
    data = s;
    return true;
  }
  return false;
}


void RGBLed::setup(ESP8266WebServer* server, WebSocketsServer* socket) {
  BaseHandler::setup(server, socket);
  pinMode(_redPin, OUTPUT);
  pinMode(_greenPin, OUTPUT);
  pinMode(_bluePin, OUTPUT);
  off();

  server->on(_route + "/on", [this, server](){
    OnParameters params;
    if (params.validate(server)) {
      this->on(params.data);
      this->success(this->jsonData());
    } else {
      this->fail(params.getErrors());
    }
  });

  server->on(_route + "/off", [this, server](){
    this->off();
    this->success(this->jsonData());
  });

  server->on(_route + "/state", [this, server](){
    this->success(this->jsonData());
  });
}

void RGBLed::on(String rgb) {
  String data = ("ffffff" + rgb);
  data = data.substring(data.length() - 6);
  byte r = strtoul(data.substring(0, 2).c_str(), 0, 16);
  byte g = strtoul(data.substring(2, 4).c_str(), 0, 16);
  byte b = strtoul(data.substring(4, 6).c_str(), 0, 16);
  change(r, g, b);
}

void RGBLed::off() {
  change(0, 0, 0);
}

void RGBLed::change(uint8_t r, uint8_t g, uint8_t b) {
  analogWrite(_redPin, r);
  analogWrite(_greenPin, g);
  analogWrite(_bluePin, b);
  _r = r;
  _g = g;
  _b = b;
}

String RGBLed::jsonData() {
  String json;
  StaticJsonBuffer<200> buff;
  JsonObject& data = buff.createObject();
  data["red"] = this->_r;
  data["green"] = this->_g;
  data["blue"] = this->_b;
  data.printTo(json);
  return json;
}
