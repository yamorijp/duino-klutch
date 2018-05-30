#include "HandlerContainer.h"

void HandlerContainer::add(String name, BaseHandler* handler) {
  _handlers.push_back(handler);
  _names.push_back(name);
}

BaseHandler* HandlerContainer::get(String name) {
  uint8_t size = _names.size();
  for (uint8_t i=0; i<size; i++) {
    if (_names.at(i) == name) return _handlers.at(i);
  }
  return nullptr;
}

void HandlerContainer::setupAll(ESP8266WebServer* server, WebSocketsServer* socket) {
  for (BaseHandler* h : _handlers) {
    h->setup(server, socket);
  }
  setup(server, socket);
}

void HandlerContainer::clearAll() {
  for (BaseHandler* h : _handlers) {
    h->clear();
  }
}

void HandlerContainer::loopAll() {
  for (BaseHandler* h : _handlers) {
    h->loop();
  }
}

void HandlerContainer::setup(ESP8266WebServer* server, WebSocketsServer* socket) {
  BaseHandler::setup(server, socket);
  server->on("/_handlers", [this, server]() {
    if (_data == "") {
      uint8_t size = _names.size();
      String json;
      DynamicJsonBuffer buff;
      JsonObject& data = buff.createObject();
      JsonArray& items = data.createNestedArray("items");
      for (uint8_t i=0; i<size; i++) {
        HandlerInfo hi = _handlers.at(i)->getHandlerInfo();
        JsonObject& item = items.createNestedObject();
        item["name"] = _names.at(i);
        item["type"] = hi.type;
        item["group"] = hi.group;
        item["resource"] = hi.resource;
      }
      data.printTo(json);
      _data = json;
    }
    this->success(_data);
  });
}

void HandlerContainer::collectAllDataTo(JsonArray* json) {
  for (BaseHandler* h : _handlers) {
    h->collectDataTo(json);
  }
}
