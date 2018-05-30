#ifndef MYHOMEIRTRANSMITTER_H_
#define MYHOMEIRTRANSMITTER_H_

#include "IRTransmitter.h"


class CommandParameters : public Parameters {
public:
  String data;
  bool validate(ESP8266WebServer* server);
};

/**
 * 赤外線リモコン (コマンド追加)
 */ 
class MyHomeIRTransmitter : public IRTransmitter {
public:
  MyHomeIRTransmitter(String route, uint8_t dataPin) : IRTransmitter(route, dataPin) {}
  void setup(ESP8266WebServer* server, WebSocketsServer* socket);
  bool sendCommand(String command);
  void sendRawData_P(const uint16_t buff_P[], uint16_t len, uint16_t hz);

  HandlerInfo getHandlerInfo() {
    HandlerInfo info;
    info.type = "MyHomeIRTransmitter";
    info.group = "ir";
    info.resource = _route;
    return info;
  }
};

#endif
