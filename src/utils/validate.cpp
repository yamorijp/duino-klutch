#include "validate.h"

bool required(String value, String& error) {
  if (value.length() != 0) {
    return true;
  } else {
    error = "is required property";
    return false;
  }
}

bool oneOrZero(String value, bool require, String& error) {
  if (require && !required(value, error)) return false;

  if (value.length() == 0 || value == "0" || value == "1") {
    return true;
  } else {
    error = "must be 0 or 1";
    return false;
  }
}

bool isInteger(String value) {
  int size = value.length();
  if (size == 0) return false;
  if (value == "0") return true;

  if (value.charAt(0) == '-') {
    value = value.substring(1);
    size--;
  }
  if (value.charAt(0) == '0') return false;
  for (int i=0; i<size; i++) {
    if (!isdigit(value.charAt(i))) {
      return false;
    }
  }
  return true;
}

/**
 * 整数 (...-2,-1,0,1,2...)
 */
bool integer(String value, bool require, String& error) {
  if (require && !required(value, error)) return false;
  if (value.length() == 0 || isInteger(value)) return true;

  error = "does not meet integer";
  return false;
}

/**
 * min <= 整数 <= max
 */
bool integerRange(String value, int min, int max, bool require, String& error) {
  if (integer(value, require, error)) {
    if (value.length() == 0) return true;

    int num = value.toInt();
    if (min <= num && num <= max) {
      return true;
    }
    error = "must have " + String(min) + " to " + String(max) + " integer";
  }
  return false;
}

/**
 * 自然数と0 (0,1,2,3...)
 */
bool wholeNumber(String value, bool require, String& error) {
  if (require && !required(value, error)) return false;
  if (value.length() == 0) return true;
  if (isInteger(value) && value.toInt() >= 0) return true;

  error = "does not meet whole number";
  return false;
}

/**
 * 自然数 (1,2,3,4...)
 */
bool naturalNumber(String value, bool require, String& error) {
  if (require && !required(value, error)) return false;
  if (value.length() == 0) return true;
  if (isInteger(value) && value.toInt() > 0) return true;

  error = "does not meet natural number";
  return false;
}

bool irRawData(String value, bool require, String& error) {
  if (require && !required(value, error)) return false;

  int size = value.length();
  if (size == 0) return true;

  for (int i=0; i<size; i++) {
    char c = value.charAt(i);
    if (!isdigit(c) && !isspace(c) && c != 44 /* comma */) {
      error = "does not conform to the raw format";
      return false;
    }
  }
  return true;
}

bool hexPattern(String value, bool require, String& error) {
  if (require && !required(value, error)) return false;

  int size = value.length();
  if (size == 0) return true;

  for (int i=0; i<size; i++) {
    char c = value.charAt(i);
    if (!isxdigit(c)) {  // 0-9A-Fa-f
      error = "does not conform to the hex format";
      return false;
    }
  }
  return true;
}

bool anyOf(String value, std::vector<String> choices, bool require, String& error) {
  if (require && !required(value, error)) return false;

  uint16_t size = choices.size();
  for (uint16_t i=0; i<size; i++) {
    if (value == choices.at(i)) {
      return true;
    }
  }
  error = "does not match any of options";
  return false;
}
