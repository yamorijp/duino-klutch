#ifndef MATRIXLED_H_
#define MATRIXLED_H_


#include "BaseHandler.h"
#include <Ticker.h>
#include <memory>
#include <deque>
#include <SPI.h>
#include <LedMatrix.h>

const uint8_t MESSAGE_QUEUE_SIZE = 10;
const uint16_t REFRESH_SCROLL_MS = 80;

typedef struct {
  String data;
  int16_t n;
} MessageItem;

class MessageParameters : public Parameters {
public:
  MessageParameters(int8_t queueCount) {
    _queueCount = queueCount;
  };
  int8_t _queueCount;
  String data;
  int16_t n;
  bool instant;
  bool validate(ESP8266WebServer* server);
};

class ColumnsParameters : public Parameters {
public:
  String data;
  bool validate(ESP8266WebServer* server);
};

/**
 * 8x8マトリクス LED (MAX7219)
 */
class MatrixLed : public BaseHandler {

private:
  std::unique_ptr<LedMatrix> _ledMatrix;
  std::deque<MessageItem> _queue;
  Ticker _ticker;
  uint8_t _csPin;
  uint8_t _numDisplays;
  uint16_t _countdown;
  String _route;

public:
  MatrixLed(String route, uint8_t csPin, uint8_t numDisplays=1) {
    _route = route;
    _csPin = csPin;
    _numDisplays = numDisplays;
  };
  void setup(ESP8266WebServer* server, WebSocketsServer* socket);
  void clear();
  void message(String data, int16_t n, bool instant);
  void next();
  void columns(String data);
  static void scrollText(MatrixLed* obj);
  
  HandlerInfo getHandlerInfo() {
    HandlerInfo info;
    info.type = "MatrixLed";
    info.group = "led";
    info.resource = _route;
    return info;
  }
};

#endif
