#ifndef MOTIONINPUTSWITCH_H_
#define MOTIONINPUTSWITCH_H_


#include "BaseHandler.h"
#include "InputSwitch.h"
#include <Ticker.h>

/**
 * スイッチ入力
 * HIGH/LOW入力のあるデジタルスイッチ。PIRモジュールなども含む。
 */ 
class MotionInputSwitch : public InputSwitch {

public:
  using InputSwitch::InputSwitch;
  HandlerInfo getHandlerInfo() {
    HandlerInfo info;
    info.type = "MotionInputSwitch";
    info.group = "switch";
    info.resource = _route;
    return info;
  }
};

#endif
