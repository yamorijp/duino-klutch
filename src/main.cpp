/**
 * duino-klutch
 */
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>
#include <WebSocketsServer.h>
#include <ArduinoOTA.h>
#include <ezTime.h>
#include <DNSServer.h>
#include <WiFiManager.h>

#include "handlers/HandlerContainer.h"

#define PROFILE_DUINO_K

#if defined(PROFILE_DUINO_K)
  #include "profile/duino_k.h"
#elif defined(PROFILE_DUINO_H)
  #include "profile/duino_h.h"
#else
  #include "profile/template.h"
#endif

Timezone tz;
ESP8266WebServer server(config::httpPort);
WebSocketsServer socket(config::wsPort);
HandlerContainer handlers;

void setupModules() {
  Serial.println(F("initializing modules..."));
  randomSeed(analogRead(0));

  profile::setup(&handlers, &tz);
  handlers.setupAll(&server, &socket);

  Serial.println(F("done"));
  Serial.println("");
}

void setupNetwork() {
  // wifi
  Serial.println(F("trying connect access point..."));
  WiFiManager wifiManager;
  if (config::resetWifiSettings) {
    wifiManager.resetSettings();
  }

  // static ip
  if (config::enableStaticIp) {
    IPAddress ip = IPAddress();
    ip.fromString(config::ipAddr);
    IPAddress gw = IPAddress();
    gw.fromString(config::ipGateway);
    IPAddress sm = IPAddress();
    sm.fromString(config::ipSubnetMask);
    wifiManager.setSTAStaticIPConfig(ip, gw, sm);
  }

  wifiManager.autoConnect(config::hostname);
  Serial.println(F("connected"));
}

void setupOTA() {
  ArduinoOTA.setHostname(config::hostname);
  ArduinoOTA.onStart([](){
    Serial.println("OTA Start: " + ArduinoOTA.getCommand());
  });
  ArduinoOTA.onEnd([](){
    Serial.println("\nOTA End");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("OTA Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("OTA Error: %u", error);
  });
  ArduinoOTA.begin();
}

void setupMdnsResponder() {
  Serial.println(F("trying start mdsn responder..."));
  if (MDNS.begin(config::hostname, WiFi.localIP())) {
    MDNS.addService("http", "tcp", 80);
    Serial.println(F("mdns responder started"));
    Serial.printf("http://%s.local\n", config::hostname);
  } else {
    Serial.println(F("error setting up mdsn responder!"));
  }
  Serial.print("http://");
  Serial.println(WiFi.localIP());
  Serial.println("");
}

void setupTime() {
  Serial.print(F("Synchronizing time..."));
  ezt::setServer(config::ntp);
  ezt::setInterval(7200);
  ezt::waitForSync();

  Serial.print("Timestamp: ");
  Serial.println(UTC.now());
  Serial.println("UTC: " + UTC.dateTime(ISO8601));
  tz.setPosix(config::tz);
  Serial.println("Local: " + tz.dateTime(ISO8601));
  Serial.println("");
}

void onEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
  switch (type) {
    case WStype_CONNECTED:
      String json;
      DynamicJsonBuffer buff;
      JsonObject& root = buff.createObject();
      root["event"] = "init";
      root["resource"] = "*";
      JsonArray& items = root.createNestedArray("items");
      handlers.collectAllDataTo(&items);
      root.printTo(json);
      socket.sendTXT(num, json);
      break;
  }
}

void setup(void) {
  // Serial.begin(115200, SERIAL_8N1, SERIAL_TX_ONLY);
  Serial.begin(115200);
  Serial.setDebugOutput(config::enableDebug);

  // macアドレス表示
  Serial.println("");
  Serial.print(F("mac address: "));
  Serial.println(WiFi.macAddress());

  // wifi
  Serial.println(F("trying connect access point..."));
  WiFiManager wifiManager;
  if (config::resetWifiSettings) {
    wifiManager.resetSettings();
  }
  wifiManager.autoConnect(config::hostname);
  Serial.println(F("connected"));

  setupNetwork();
  setupMdnsResponder();
  setupTime();
  setupModules();

  server.begin();
  Serial.println(F("http server started"));

  socket.begin();
  socket.onEvent(onEvent);
  Serial.println(F("websocket server started"));

  Serial.println("");
  Serial.print(config::hostname);
  Serial.println(F(" is now online. Roll with it!"));
}

void loop(void) {
  ArduinoOTA.handle();
  // MDNS.update(); // ArduinoOTA.handle内で処理されるので不要
  ezt::events();
  server.handleClient();
  socket.loop();
  handlers.loopAll();
  profile::loop();
  delay(10);
}
