#include "FourDigitLed.h"
#include "utils/validate.h"


bool NumberParameters::validate(ESP8266WebServer* server) {
  String error;
  if (!integerRange(server->arg("data"), 0, 9999, true, error)) {
    errors->set("data", error);
  }
  if (!oneOrZero(server->arg("leading_zero"), false, error)) {
    errors->set("leading_zero", error);
  }
  if (!integerRange(server->arg("brightness"), 0, 7, false, error)) {
    errors->set("brightness", error);
  }

  if (errors->size() == 0) {
    data = argInt(server, "data");
    leadingZero = argBool(server, "leading_zero", false);
    brightness = argInt(server, "brightness", 4);
    return true;
  }
  return false;
}


bool SegmentsParameters::validate(ESP8266WebServer* server) {
  String error;
  if (!hexPattern(server->arg("data"), true, error)) {
    errors->set("data", error);
  }
  if (!integerRange(server->arg("brightness"), 0, 7, false, error)) {
    errors->set("brightness", error);
  }

  if (errors->size() == 0) {
    data = argStr(server, "data");
    brightness = argInt(server, "brightness", 4);
    return true;
  }
  return false;
}

bool CountingParameters::validate(ESP8266WebServer* server) {
  String error;
  if (!integerRange(server->arg("from"), 0, 9999, false, error)) {
    errors->set("from", error);
  }
  if (!integerRange(server->arg("to"), 0, 9999, false, error)) {
    errors->set("to", error);
  }
  if (!integerRange(server->arg("step"), 1, 9999, false, error)) {
    errors->set("step", error);
  }
  if (!integerRange(server->arg("brightness"), 0, 7, false, error)) {
    errors->set("brightness", error);
  }

  if (errors->size() == 0) {
    from = argInt(server, "from", 0);
    to = argInt(server, "to", 9999);
    step = argInt(server, "step", 1);
    brightness = argInt(server, "brightness", 4);
    return true;
  }
  return false;
}

bool RollParameters::validate(ESP8266WebServer* server) {
  String error;
  if (!integerRange(server->arg("min"), 0, 9999, false, error)) {
    errors->set("min", error);
  }
  if (!integerRange(server->arg("max"), 0, 9999, false, error)) {
    errors->set("max", error);
  }
  if (!integerRange(server->arg("brightness"), 0, 7, false, error)) {
    errors->set("brightness", error);
  }

  if (errors->size() == 0) {
    min = argInt(server, "min", 0);
    max = argInt(server, "max", 9999);
    brightness = argInt(server, "brightness", 4);
    return true;
  }
  return false;
}


void FourDigitLed::setup(ESP8266WebServer* server, WebSocketsServer* socket) {
  BaseHandler::setup(server, socket);
  _display.reset(new TM1637Display(_clkPin, _dioPin));
  _display->setBrightness(0, false);
  _display->showNumberDec(0);

  server->on(_route + "/clear", [this, server](){
    this->clear(true);
    this->success("");
  });

  server->on(_route + "/number", [this, server](){
    NumberParameters params;
    if (params.validate(server)) {
      this->number(params.data, params.leadingZero, params.brightness);
      this->success("");
    } else {
      this->fail(params.getErrors());
    }
  });

  server->on(_route + "/segments", [this, server](){
    SegmentsParameters params;
    if (params.validate(server)) {
      this->segments(params.data, params.brightness);
      this->success("");
    } else {
      this->fail(params.getErrors());
    }
  });

  server->on(_route + "/counting", [this, server](){
    CountingParameters params;
    if (params.validate(server)) {
      this->counting(params.from, params.to, params.step, params.brightness);
      this->success("");
    } else {
      this->fail(params.getErrors());
    }
  });

  server->on(_route + "/roll", [this, server](){
    RollParameters params;
    if (params.validate(server)) {
      uint16_t value = this->roll(params.min, params.max, params.brightness);
      String json = this->toJsonObject("roll", value);
      this->success(json);
    } else {
      this->fail(params.getErrors());
    }
  });
}

void FourDigitLed::clear(bool update) {
  _ticker.detach();
  _display->setBrightness(0, false);
  _display->showNumberDec(0);
  setMode(FOURDIGITLED_MODE_IDLE);
  if (update) {
    BaseHandler::update();
  }
}

void FourDigitLed::number(uint16_t data, bool leadingZero, uint8_t brightness) {
  clear();
  _display->setBrightness(brightness);
  _display->showNumberDec(data, leadingZero);
  setMode(FOURDIGITLED_MODE_ACTIVE);
}

void FourDigitLed::segments(String data, uint8_t brightness) {
  clear();
  _display->setBrightness(brightness);
  String value = data + "00000000";
  value = value.substring(0, 8);
  uint8_t segments[4];
  for (int i=1; i<8; i=i+2) {
    char hex[] = {data.charAt(i-1), data.charAt(i)};
    segments[i / 2] = strtoul(hex, 0, 16);
  }
  _display->setSegments(segments);
  setMode(FOURDIGITLED_MODE_ACTIVE);
}

void FourDigitLed::clock(uint8_t hour, uint8_t minute, uint8_t brightness) {
  clear();
  _display->setBrightness(brightness);
  _display->showNumberDecEx(hour * 100 + minute, 0b01000000, true);
  setMode(FOURDIGITLED_MODE_CLOCK);
}

void FourDigitLed::counting(uint16_t from, uint16_t to, uint16_t step, uint8_t brightness) {
  clear();
  _display->setBrightness(brightness);
  _counter = from;
  _countTo = to;
  _countStep = step;
  if (from <= to) {
    _ticker.attach_ms(50, countup, this);
  } else {
    _ticker.attach_ms(50, countdown, this);
  }
  setMode(FOURDIGITLED_MODE_ACTIVE);
}

void FourDigitLed::countup(FourDigitLed* obj) {
  obj->_counter += obj->_countStep;
  if (obj->_counter >= obj->_countTo) {
    obj->_counter = obj->_countTo;
    obj->_ticker.detach();
  }
  obj->_display->showNumberDec(obj->_counter);
}

void FourDigitLed::countdown(FourDigitLed* obj) {
  obj->_counter -= obj->_countStep;
  if (obj->_counter <= obj->_countTo) {
    obj->_counter = obj->_countTo;
    obj->_ticker.detach();
  }
  obj->_display->showNumberDec(obj->_counter);
}

uint16_t FourDigitLed::roll(uint16_t min, uint16_t max, uint8_t brightness) {
  clear();
  if (min > max) {
    uint16_t t = min; min = max; max = t;
  }
  _display->setBrightness(brightness);

  int rolled = random(min, max + 1);
  _display->showNumberDec(rolled);
  setMode(FOURDIGITLED_MODE_ACTIVE);

  return rolled;
}

uint8_t FourDigitLed::getMode() {
  return _mode;
}

void FourDigitLed::setMode(uint8_t mode) {
  _mode = mode;
}
