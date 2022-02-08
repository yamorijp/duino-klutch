#include "Display.h"
#include "utils/validate.h"


bool BrightnessParameters::validate(ESP8266WebServer* server) {
  String error;
  if (!integerRange(server->arg("data"), 0, UINT8_MAX, false, error)) {
    errors->set("data", error);
  }
  if (errors->size() == 0) {
    data = argInt(server, "data", 255);
    return true;
  }
  return false;
}

void Display::setup(ESP8266WebServer* server, WebSocketsServer* socket) {
  BaseHandler::setup(server, socket);
  display.reset(new U8G2_SSD1306_128X32_UNIVISION_F_SW_I2C(U8G2_R0, _clkPin, _dioPin, U8X8_PIN_NONE));
  display->begin();
  clear();

  server->on(_route, [this, server]() {
    this->success(_data);
  });

  server->on(_route + "/brightness", [this, server]() {
    BrightnessParameters params;
    if (params.validate(server)) {
      this->brightness(params.data);
      this->success(_data);
    } else {
      this->fail(params.getErrors());
    }
  });

  server->on(_route + "/clear", [this, server]() {
    this->clear();
    this->success(_data);
  });
}

void Display::brightness(uint8_t n) {
  _brightness = n;
  display->setContrast(n);
  update();
}

void Display::clear() {
  display->clearBuffer();
  display->sendBuffer();
  brightness(255);
}

void Display::update() {
  _data = toJsonObject("brightness", _brightness);
  broadcast(_route, _data);
  BaseHandler::update();  
}