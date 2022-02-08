#ifndef IRTRANSMITTER_H_
#define IRTRANSMITTER_H_

#include "BaseHandler.h"
#include <memory>
#include <vector>
#include <IRremoteESP8266.h>
#include <IRsend.h>


class RawParameters : public Parameters {
public:
  String data;
  int16_t hz;
  bool validate(ESP8266WebServer* server);
};


class SendParameters : public Parameters {
public:
  int16_t type;
  String data;
  int16_t bits;
  bool validate(ESP8266WebServer* server);
};


/**
 * 赤外線リモコン
 */
class IRTransmitter : public BaseHandler {
private:
  int8_t _pin;

protected:
  String _route;
  std::unique_ptr<IRsend> ir;

public:
  IRTransmitter(String route, int8_t dataPin) : _route(route), _pin(dataPin) {}
  virtual void setup(ESP8266WebServer* server, WebSocketsServer* socket);
  void send(int16_t type, String data, int16_t bits);
  void sendRaw(String data, int16_t hz=32);

  HandlerInfo getHandlerInfo() {
    HandlerInfo info;
    info.type = "IRTransmitter";
    info.group = "ir";
    info.resource = _route;
    return info;
  }
};

#endif
