#ifndef RGBLED_H_
#define RGBLED_H_


#include "BaseHandler.h"


class OnParameters : public Parameters {
public:
  String data;
  bool validate(ESP8266WebServer* server);
};

/**
 * RGB LED
 */ 
class RGBLed : public BaseHandler {
private:
  uint8_t _redPin, _greenPin, _bluePin;
  uint8_t _r, _g, _b;
  String _route;
  void change(uint8_t r, uint8_t g, uint8_t b);
  String jsonData();
public:
  RGBLed(String route, uint8_t redPin, uint8_t greenPin, uint8_t bluePin) {
    _route = route;
    _redPin = redPin;
    _greenPin = greenPin;
    _bluePin = bluePin;
  }
  void setup(ESP8266WebServer* server, WebSocketsServer* socket);
  void on(String rgb);
  void off();
  void clear() {
    off();
  }

  HandlerInfo getHandlerInfo() {
    HandlerInfo info;
    info.type = "RGBLed";
    info.group = "led";
    info.resource = _route;
    return info;
  }
};

#endif
