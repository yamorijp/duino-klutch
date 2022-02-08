#ifndef CLOCK_H_
#define CLOCK_H_

#include "BaseHandler.h"
#include <Ticker.h>
#include <U8g2lib.h>


class ClockTimerParameters : public Parameters {
public:
  uint16_t delay;
  uint8_t pre;
  uint8_t post;
  bool validate(ESP8266WebServer* server);
};

/**
 * 時計とタイマー機能
 */
class Clock : public BaseHandler {
private:
  String _route;
  uint16_t _interval;
  String _data;
  time_t _value;
  bool _dirty;
  time_t _timer_at;
  uint16_t _timer_duration;
  uint16_t _timer_action;
  uint16_t _timer_remain;
  std::function<void(int)> _action;
  Ticker _ticker;

public:
  Clock(String route, uint16_t interval=1000) :
    _route(route), _interval(interval), _data("{}"), _value(0), _dirty(false), _timer_at(0), _timer_duration(0), _timer_action(0), _timer_remain(0), _action(NULL) {};
  void setup(ESP8266WebServer* server, WebSocketsServer* socket);
  void timer(uint16_t duration, uint8_t pre, uint8_t post);
  void clear();
  void update();
  void action(uint8_t n);
  void onAction(std::function<void(int)> f);
  bool hasTimer();
  time_t getTimer();
  String getTimerFormatted();
  time_t getTime();  
  static void tick(Clock* obj);
  void loop();
  HandlerInfo getHandlerInfo() {
    HandlerInfo info;
    info.type = "Clock";
    info.group = "sensor";
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
