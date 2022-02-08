#ifndef BASEMODULE_H_
#define BASEMODULE_H_

#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESP8266WebServer.h>
#include <WebSocketsServer.h>
#include <vector>


/**
 * ハンドラー情報
 */
struct HandlerInfo {
  String type;
  String group;
  String resource;
};


/**
 * 入力パラメータのコンテナ
 */
class Parameters {
public:
  DynamicJsonBuffer buff;
  JsonObject* errors;
  Parameters();
  String getErrors();

  /** バリデート処理を実装する */
  virtual bool validate(ESP8266WebServer* server) {
    return true;
  }
  int argInt(ESP8266WebServer* server, String name, int fallback=0) {
    return server->hasArg(name) ? server->arg(name).toInt() : fallback;
  }
  String argStr(ESP8266WebServer* server, String name, String fallback="") {
    return server->hasArg(name) && server->arg(name) ?
      server->arg(name) : fallback;
  }
  bool argBool(ESP8266WebServer* server, String name, bool fallback=false) {
    return argInt(server, name, fallback ? 1 : 0);
  }
};

/**
 * ハンドラー
 */
class BaseHandler {
private:
  void _respond(uint16_t code, String status, const String& data, String errorMessage="");
  ESP8266WebServer* _server;
  WebSocketsServer* _socket;
  std::function<void(void)> _update;

protected:
  /** 成功レスポンスを送出する */
  void success(const String& dataString);

  /** 失敗レスポンスを送出する */
  void fail(const String& dataString, uint16_t code=400);

  /** エラーレスポンスを送出する */
  void error(const String& message, uint16_t code=500);

  /** ソケットに更新をブロードキャストする */
  void broadcast(const String& resource, const String& data, const String& event="update");

public:
  /**
   * モジュールの初期化、ハンドラー登録を実装
   */
  virtual void setup(ESP8266WebServer* server, WebSocketsServer* socket) {
    _server = server;
    _socket = socket;
  }

  /** LEDの消灯などモジュールのリセット処理を実装 */
  virtual void clear() {
  }

  /** 状態の更新 */
  virtual void update() {
    if (_update) _update();
  }

  /** updateイベント */
  void onUpdate(std::function<void(void)> f) {
    _update = f;
  }

  /** メインループで実行する処理を実装 */
  virtual void loop() {
  }

  /** ハンドラー情報を実装 */
  virtual HandlerInfo getHandlerInfo() {
    HandlerInfo info;
    info.type = "BaseHandler";
    info.group = "";
    info.resource = "";
    return info;
  }

  virtual void collectDataTo(JsonArray* json) {
  }

  template <typename T>
  String toJsonObject(String name, T value) {
    String json;
    DynamicJsonBuffer buff;
    JsonObject& data = buff.createObject();
    data[name] = value;
    data.printTo(json);
    return json;
  }

  template <typename T>
  String toJsonObjectArray(String name, std::vector<T> values) {
    String json;
    DynamicJsonBuffer buff;
    JsonObject& data = buff.createObject();
    JsonArray& array = data.createNestedArray(name);
    for (uint16_t i=0; i<values.size(); i++) {
      array.add(values.at(i));
    }
    data.printTo(json);
    return json;
  }
};

#endif  // BASEMODULE_H_
