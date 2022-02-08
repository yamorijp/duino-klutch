#include <ESP8266WiFi.h>
#include <LittleFS.h>
#include "Site.h"
#include "../config.h"

void walkDir(String root, std::function<void(String)> func) {
  Dir dir = LittleFS.openDir(root);
  while (dir.next()) {
    String path = root + "/" + dir.fileName();
    if (dir.isDirectory()) {
      walkDir(path, func);
    } else {
      func(path);
    }
  }
}

void Site::setup(ESP8266WebServer* server, WebSocketsServer* socket) {
  BaseHandler::setup(server, socket);

  LittleFS.begin();
  
  walkDir("", [server](String path) {
    server->serveStatic(path.c_str(), LittleFS, path.c_str());
  });
  server->serveStatic("/", LittleFS, "/index.html");
  
  server->on(_route + "/docs", [this, server]() {
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
