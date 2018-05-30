#include <FS.h>
#include <ESP8266WiFi.h>
#include "Site.h"
#include "../config.h"

void Site::setup(ESP8266WebServer* server, WebSocketsServer* socket) {
  BaseHandler::setup(server, socket);

  SPIFFS.begin();
  Dir dir = SPIFFS.openDir("/");
  while (dir.next()) {
    server->serveStatic(
      String("/docs" + dir.fileName()).c_str(), SPIFFS, dir.fileName().c_str());
  };
  server->on(_route + "/", [this, server]() {
    server->sendHeader("Location", "/docs/curl/index.html", true);
    server->send(302, "text/plain", "");
  });

  server->on(_route + "/heap", [this, server]() {
    String json;
    StaticJsonBuffer<200> buff;
    JsonObject& data = buff.createObject();
    data["vcc"] = ESP.getVcc();
    data["free_heap"] = ESP.getFreeHeap();
    data["free_sketch_space"] = ESP.getFreeSketchSpace();
    data.printTo(json);
    this->success(json);
  });

  server->on(_route + "/net", [this, server]() {
    String json;
    StaticJsonBuffer<200> buff;
    JsonObject& data = buff.createObject();
    data["ip"] = String(WiFi.localIP().toString());
    data["hostname"] = String(Config::hostname) + ".local";
    data["mac"] = WiFi.macAddress();
    data.printTo(json);
    this->success(json);
  });

  server->on(_route + "/reset", [this, server]() {
    this->success("");
    delay(1000);
    ESP.reset();
  });

  server->on(_route + "/restart", [this, server]() {
    this->success("");
    delay(1000);
    ESP.restart();
  });

  server->onNotFound([this, server]() {
    this->error("404 not found", 404);
  });
}
