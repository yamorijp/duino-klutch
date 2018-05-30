#include "IRTransmitter.h"
#include "utils/validate.h"


bool RawParameters::validate(ESP8266WebServer* server) {
  String error;
  if (!irRawData(server->arg("data"), true, error)) {
    errors->set("data", error);
  }
  if (!integerRange(server->arg("hz"), 1, 1000, false, error)) {
    errors->set("hz", error);
  }

  if (errors->size() == 0) {
    data = argStr(server, "data");
    hz = argInt(server, "hz", 32);
    return true;
  }
  return false;
}


bool SendParameters::validate(ESP8266WebServer* server) {
  String error;
  data = server->arg("data");
  if (!hexPattern(data, true, error)) {
    errors->set("data", error);
  }
  type = server->arg("type").toInt();
  if (!integerRange(server->arg("type"), 1, 40, true, error)) {
    errors->set("type", error);
  }
  bits = server->arg("bits").toInt();
  if (!integerRange(server->arg("bits"), 1, 1000, true, error)) {
    errors->set("bits", error);
  }

  return errors->size() < 1;
}


void IRTransmitter::setup(ESP8266WebServer* server, WebSocketsServer* socket) {
  BaseHandler::setup(server, socket);
  ir.reset(new IRsend(_pin));
  ir->begin();

  server->on(_route + "/raw", [this, server]() {
    RawParameters params;
    if (params.validate(server)) {
      this->sendRaw(params.data, params.hz);
      this->success("");
    } else {
      this->fail(params.getErrors());
    }
  });

  server->on(_route + "/send", [this, server]() {
    SendParameters params;
    if (params.validate(server)) {
      this->send(params.type, params.data, params.bits);
      this->success("");
    } else {
      this->fail(params.getErrors());
    }
  });
}

void IRTransmitter::send(int16_t type, String data, int16_t bits) {
  ir->send(type, strtoul(data.c_str(), 0, 16), bits);
}

void IRTransmitter::sendRaw(String data, int16_t hz) {
  // data = "1201,1320,978,2189..."
  std::vector<uint16_t> raw;
  String buff;
  int len = sizeof(data) / sizeof(data[0]);
  for (int i=0; i<len; i++) {
    char c = data.charAt(i);
    if (c == ',') {
      if (buff != "") {
        raw.push_back(buff.toInt());
      }
      buff = "";
    } else if (c != ' ') {
      buff += c;
    }
  }
  ir->sendRaw(raw.data(), raw.size(), hz);
}
