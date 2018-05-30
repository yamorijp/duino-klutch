#ifndef FOURDIGITLED_H_
#define FOURDIGITLED_H_

#include "BaseHandler.h"
#include <Ticker.h>
#include <memory>
#include <TM1637Display.h>


class NumberParameters : public Parameters {
public:
  uint16_t data;
  bool leadingZero;
  uint8_t brightness;
  bool validate(ESP8266WebServer* server);
};

class SegmentsParameters : public Parameters {
public:
  String data;
  uint8_t brightness;
  bool validate(ESP8266WebServer* server);
};

class CountingParameters : public Parameters {
public:
  uint16_t from;
  uint16_t to;
  uint16_t step;
  uint8_t brightness;
  bool validate(ESP8266WebServer* server);
};

class RollParameters : public Parameters {
public:
  uint16_t min;
  uint16_t max;
  uint8_t brightness;
  bool validate(ESP8266WebServer* server);
};

/**
 * 4桁 7セグメント LED ディスプレイ (TM1637)
 */ 
class FourDigitLed : public BaseHandler {
private:
  uint8_t _dioPin;
  uint8_t _clkPin;
  Ticker _ticker;
  std::unique_ptr<TM1637Display> _display;
  uint16_t _counter;
  uint16_t _countTo;
  uint16_t _countStep;
  String _route;

public:
  FourDigitLed(String route, uint8_t clkPin, uint8_t dioPin) {
    _route = route;
    _dioPin = dioPin;
    _clkPin = clkPin;
  }
  void setup(ESP8266WebServer* server, WebSocketsServer* socket);
  void clear();
  void number(uint16_t data, bool leadingZero=false, uint8_t brightness=4);
  void segments(String data, uint8_t brightness=4);
  void counting(uint16_t from, uint16_t to, uint16_t step=1, uint8_t brightness=4);
  static void countup(FourDigitLed* obj);
  static void countdown(FourDigitLed* obj);
  uint16_t roll(uint16_t min, uint16_t max, uint8_t brightness=4);
  HandlerInfo getHandlerInfo() {
    HandlerInfo info;
    info.type = "FourDigitLed";
    info.group = "led";
    info.resource = _route;
    return info;
  }
};

#endif
