#ifndef HANDLERCONTAINER_H_
#define HANDLERCONTAINER_H_

#include "BaseHandler.h"
#include <vector>

/**
 * ハンドラーのコンテナ
 */ 
class HandlerContainer : public BaseHandler {
private:
  std::vector<BaseHandler*> _handlers;
  std::vector<String> _names;
  String _data;

public:
  void add(String name, BaseHandler* handler);
  void setup(ESP8266WebServer* server, WebSocketsServer* socket);
  void setupAll(ESP8266WebServer* server, WebSocketsServer* socket);
  void loopAll();
  void clearAll();

  BaseHandler* get(String name);
  HandlerInfo getHandlerInfo() {
    HandlerInfo info;
    info.type = "HandlerContainer";
    info.group = "";
    info.resource = "";
    return info;
  }
  std::vector<HandlerInfo> getHandlerInfoArray();
  void collectAllDataTo(JsonArray* json);
};

#endif  // HANDLERCONTAINER_H_
