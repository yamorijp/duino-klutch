#include "BaseHandler.h"
#include "utils/validate.h"

Parameters::Parameters() {
  errors = &buff.createObject();
}

String Parameters::getErrors() {
  String s;
  errors->printTo(s);
  return s;
}

void BaseHandler::_respond(uint16_t code, String status, const String& data, String errorMessage) {
  String json;
  DynamicJsonBuffer buff;
  JsonObject& root = buff.createObject();
  root["status"] = status;
  if (status == "success" || status == "fail") {
    if (data != "") {
      root["data"] = RawJson(data);
    } else {
      root["data"] = RawJson("null");
    }
  } else {
    root["message"] = errorMessage;
  }
  root.printTo(json);
  _server->sendHeader("Access-Control-Allow-Origin", "*");
  _server->send(code, "application/json", json);
}

void BaseHandler::success(const String& dataString) {
  _respond(200, "success", dataString);
}

void BaseHandler::fail(const String& dataString, uint16_t code) {
  _respond(code, "fail", dataString);
}

void BaseHandler::error(const String& message, uint16_t code) {
  _respond(code, "error", "", message);
}

void BaseHandler::broadcast(const String& resource, const String& data, const String& event) {
  String json;
  DynamicJsonBuffer buff;
  JsonObject& root = buff.createObject();
  root["event"] = event;
  root["resource"] = resource;
  root["data"] = RawJson(data != "" ? data : "null");
  root.printTo(json);
  _socket->broadcastTXT(json);
}
