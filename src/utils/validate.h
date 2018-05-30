#ifndef VALIDATE_H_
#define VALIDATE_H_

#include <Arduino.h>

bool required(String value, String& error);
bool hexPattern(String value, bool require, String& error);
bool naturalNumber(String value, bool require, String& error);
bool oneOrZero(String value, bool require, String& error);
bool wholeNumber(String value, bool require, String& error);
bool irRawData(String value, bool require, String& error);
bool integer(String value, bool require, String& error);
bool integerRange(String value, int min, int max, bool require, String& error);
bool anyOf(String value, std::vector<String> choices, bool require, String& error);

#endif
