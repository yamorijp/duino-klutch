#ifndef SITEMODULE_H_
#define SITEMODULE_H_

#include "BaseHandler.h"

/**
 * サイト
 */ 
class Site : public BaseHandler {
private:
  String _route;

public:
  Site(String route) {
    _route = route;
  }
  void setup(ESP8266WebServer* server, WebSocketsServer* socket);

  HandlerInfo getHandlerInfo() {
    HandlerInfo info;
    info.type = "Site";
    info.group = "site";
    info.resource = _route;
    return info;
  }
};

#endif
