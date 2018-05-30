/**
 * duino-klutch
 */
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>
#include <WebSocketsServer.h>
#include <time.h>
#include <DNSServer.h>
#include <WiFiManager.h>

#include "config.h"
#include "handlers/BaseHandler.h"
#include "handlers/HandlerContainer.h"
#include "handlers/Site.h"
#include "handlers/RGBLed.h"
#include "handlers/MatrixLed.h"
#include "handlers/FourDigitLed.h"
#include "handlers/AnalogSensor.h"
#include "handlers/HygroThermoSensor.h"
#include "handlers/MyHomeIRTransmitter.h"
#include "handlers/OutputSwitch.h"
#include "handlers/InputSwitch.h"

ESP8266WebServer server(Config::httpPort);
WebSocketsServer socket(Config::wsPort);
HandlerContainer handlers;

void setupModules() {
  Serial.println(F("initializing modules..."));
  randomSeed(analogRead(0));

  if (Config::enableSite)
    handlers.add("Site", new Site(""));
  if (Config::enableRelay)
    handlers.add("Relay", new OutputSwitch("/switch/relay", D0));
  if (Config::enableMatrix)
    handlers.add("Matrix", new MatrixLed("/led/matrix", D4, Config::matrixNumDisplays));
  if (Config::enableIr)
    handlers.add("IR Remote", new IRTransmitter("/ir/transmitter", D8));
  if (Config::enableIrx)
    handlers.add("IR Remote", new MyHomeIRTransmitter("/ir/transmitter", D8));
  if (Config::enableMotion)
    handlers.add("Motion", new InputSwitch("/switch/motion", D6));
  if (Config::enableHt)
    handlers.add("Temperature & Humidity", new HygroThermoSensor("/sensor/ht", D1));
  if (Config::enableCds)
    handlers.add("Brightness", new AnalogSensor("/sensor/brightness", A0));
  if (Config::enableFourdigit)
    handlers.add("4 Digit", new FourDigitLed("/led/4digit", D2, D3));
  if (Config::enableRgb)
    handlers.add("RGB", new RGBLed("/led/rgb", D1, D2, D3));

  handlers.setupAll(&server, &socket);

  Serial.println(F("done"));
  Serial.println("");
}

void setupMdnsResponder() {
  Serial.println(F("trying start mdsn responder..."));
  if (MDNS.begin(Config::hostname, WiFi.localIP())) {
    MDNS.addService("http", "tcp", 80);
    Serial.println(F("mdns responder started"));
    Serial.printf("http://%s.local\n", Config::hostname);
  } else {
    Serial.println(F("error setting up mdsn responder!"));
  }
  Serial.print("http://");
  Serial.println(WiFi.localIP());
  Serial.println("");
}

void setupNtp() {
  Serial.println(F("trying to connect ntp server..."));
  configTime(Config::timezone, 0, Config::ntp1, Config::ntp2);
  while (time(NULL) == 0) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println(F("ntp synced"));
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
  Serial.setDebugOutput(Config::enableDebug);

  // macアドレス表示
  Serial.println("");
  Serial.print(F("mac address: "));
  Serial.println(WiFi.macAddress());

  // wifi
  Serial.println(F("trying connect access point..."));
  WiFiManager wifiManager;
  if (Config::resetWifiSettings) {
    wifiManager.resetSettings();
  }
  wifiManager.autoConnect(Config::hostname);
  Serial.println(F("connected"));

  setupMdnsResponder();
  setupNtp();
  setupModules();

  server.begin();
  Serial.println(F("http server started"));

  socket.begin();
  socket.onEvent(onEvent);
  Serial.println(F("websocket server started"));

  Serial.println("");
  Serial.print(Config::hostname);
  Serial.println(F(" is now online. Roll with it!"));
}

void loop(void) {
  server.handleClient();
  socket.loop();
  handlers.loopAll();
  delay(10);
}
