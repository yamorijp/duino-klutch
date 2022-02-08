#ifndef DISPLAY_H_
#define DISPLAY_H_

#include "BaseHandler.h"
#include <U8g2lib.h>

class BrightnessParameters : public Parameters {
public:
  uint8_t data;
  bool validate(ESP8266WebServer* server);
};


/**
 * ディスプレイ
 */
class Display : public BaseHandler {
private:
  String _route;
  uint8_t _dioPin;
  uint8_t _clkPin;
  String _data;
  uint8_t _brightness;

public:
  Display(String route, uint8_t dioPin, uint8_t clkPin) :
    _route(route), _dioPin(dioPin), _clkPin(clkPin), _data("{}"), _brightness(0) {};
  std::unique_ptr<U8G2_SSD1306_128X32_UNIVISION_F_SW_I2C> display;
  void setup(ESP8266WebServer* server, WebSocketsServer* socket);
  void brightness(uint8_t n);
  void clear();
  void update();
  HandlerInfo getHandlerInfo() {
    HandlerInfo info;
    info.type = "Display";
    info.group = "device";
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
