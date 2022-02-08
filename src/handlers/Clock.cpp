#include "Clock.h"
#include "utils/validate.h"
#include <ezTime.h>


bool ClockTimerParameters::validate(ESP8266WebServer* server) {
  String error;
  if (!integerRange(server->arg("delay"), 0, UINT16_MAX, true, error)) {
    errors->set("delay", error);
  }
  if (!integerRange(server->arg("pre"), 0, 99, false, error)) {
    errors->set("pre", error);
  }
  if (!integerRange(server->arg("post"), 0, 99, false, error)) {
    errors->set("post", error);
  }
  if (errors->size() == 0) {
    delay = argInt(server, "delay");
    pre = argInt(server, "pre", 0);
    post = argInt(server, "post", 0);
    return true;
  }
  return false;
}

void Clock::setup(ESP8266WebServer* server, WebSocketsServer* socket) {
  BaseHandler::setup(server, socket);
  _ticker.attach_ms(_interval, tick, this);
  _value = now();
  update();

  server->on(_route, [this, server]() {
    this->success(_data);
  });

  server->on(_route + "/timer", [this, server]() {
    ClockTimerParameters params;
    if (params.validate(server)) {
      this->timer(params.delay, params.pre, params.post);
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

void Clock::timer(uint16_t delay, uint8_t pre, uint8_t post) {
  _timer_duration = delay;
  _timer_action = post;
  _timer_at = now();
  _timer_remain = _timer_duration - (_value - _timer_at);
  action(pre);
  update();
}

void Clock::clear() {
  _timer_duration = 0;
  _timer_action = 0;
  _timer_at = 0;
  _timer_remain = 0;
  update();
}

void Clock::loop() {
  if (_dirty) {
    _dirty = false;
    _value = now();

    // タイマー処理
    if (hasTimer()) {
      _timer_remain = _timer_duration - (_value - _timer_at);
      if (_timer_remain <= 0) {
        action(_timer_action);
        clear();
      }
    }
    update();
  }
}

void Clock::onAction(std::function<void(int)> f) {
  _action = f;
}

void Clock::action(uint8_t n) {
  if (_action) _action(n);
}

void Clock::update() {
  String json;
  StaticJsonBuffer<200> buff;
  JsonObject& data = buff.createObject();
  data["value"] = _value;
  data["timer"] = _timer_remain;
  data.printTo(json);
  _data = json;
  broadcast(_route, _data);
  BaseHandler::update();
}

void Clock::tick(Clock* obj) {
  obj->_dirty = true;
}

bool Clock::hasTimer() {
  return _timer_at > 0;
}

time_t Clock::getTime() {
  return _value;
}

time_t Clock::getTimer() {
  return hasTimer() ? _timer_remain : 0;
}

String Clock::getTimerFormatted() {
  if (!hasTimer()) return "--.--";
  if (_timer_remain >= 6000) return "99.59+";
  char buff[8];
  sprintf(buff, "%02d.%02d", _timer_remain / 60, _timer_remain % 60);
  return buff;
}
