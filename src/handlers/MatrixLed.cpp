#include "MatrixLed.h"
#include "utils/validate.h"


bool MessageParameters::validate(ESP8266WebServer* server) {
  String error;
  if (!required(server->arg("data"), error)) {
    errors->set("data", error);
  }
  if (!integerRange(server->arg("n"), -1, 100, false, error)) {
    errors->set("n", error);
  }
  if (!oneOrZero(server->arg("instant"), false, error)) {
    errors->set("instant", error);
  }
  if (server->arg("instant") != "1" && _queueCount >= MESSAGE_QUEUE_SIZE) {
    error = "message queue is full";
    errors->set("data", error);
  }

  if (errors->size() == 0) {
    data = argStr(server, "data");
    n = argInt(server, "n", 1);
    instant = argInt(server, "instant", 0);
    return true;
  }
  return false;
}


bool ColumnsParameters::validate(ESP8266WebServer* server) {
  String error;
  if (!hexPattern(server->arg("data"), true, error)) {
    errors->set("data", error);
  }

  if (errors->size() == 0) {
    data = argStr(server, "data");
    return true;
  }
  return false;
}


void MatrixLed::setup(ESP8266WebServer* server, WebSocketsServer* socket) {
  BaseHandler::setup(server, socket);
  _ledMatrix.reset(new LedMatrix(_numDisplays, _csPin));
  _ledMatrix->init();
  clear();

  server->on(_route + "/clear", [this, server](){
    this->clear();
    this->success("");
  });

  server->on(_route + "/message", [this, server](){
    MessageParameters params(this->_queue.size());
    if (params.validate(server)) {
      this->message(params.data, params.n, params.instant);
      this->success("");
    } else {
      this->fail(params.getErrors());
    }
  });

  server->on(_route + "/columns", [this, server](){
    ColumnsParameters params;
    if (params.validate(server)) {
      this->columns(params.data);
      this->success("");
    } else {
      this->fail(params.getErrors());
    }
  });

  server->on(_route + "/next", [this, server]() {
    this->next();
    this->success("");
  });
}

void MatrixLed::clear() {
  _ticker.detach();
  while (!_queue.empty()) {
    _queue.pop_front();
  }
  _countdown = 0;
  _ledMatrix->clear();
  _ledMatrix->commit();
}

void MatrixLed::message(String data, int16_t n, bool instant) {
  if (instant) clear();
  MessageItem item;
  item.data = data;
  item.n = n;
  _queue.push_back(item);
  if (!_ticker.active()) next();
}

void MatrixLed::next() {
  if (_queue.empty()) {
    clear();
    return;
  }

  MessageItem item = _queue.front();
  // (1文字の幅 + 空白) x 再生回数
  _countdown = item.n == -1 ? -1 : (item.data.length() * 7 + 7 * _numDisplays + 1) * item.n;
  _ledMatrix->setText(item.data);
  _queue.pop_front();

  if (!_ticker.active()) {
    _ticker.attach_ms(REFRESH_SCROLL_MS, scrollText, this);
  }
}

void MatrixLed::columns(String data) {
  clear();
  String s;
  int size = _numDisplays * 16;
  for (int i=0; i<_numDisplays; i++) {
    s.concat("0000000000000000");
  }
  s.concat(data);
  s = s.substring(s.length() - size);

  for (int i=1; i<size; i=i+2) {
    char hex[] = {data.charAt(i-1), data.charAt(i)};
    byte b = (byte)strtoul(hex, 0, 16);
    _ledMatrix->setColumn(i / 2, b);
  }
  _ledMatrix->commit();
}

void MatrixLed::scrollText(MatrixLed* obj) {
  if (obj->_countdown == 0) {
    obj->next();
    return;
  }
  obj->_countdown--;
  obj->_ledMatrix->clear();
  obj->_ledMatrix->scrollTextLeft();
  obj->_ledMatrix->drawText();
  obj->_ledMatrix->commit();
}
