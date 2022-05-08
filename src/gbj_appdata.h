/*
  NAME:
  gbj_appdata

  DESCRIPTION:
  Application library for project specific data hubs definitions.

  LICENSE:
  This program is free software; you can redistribute it and/or modify
  it under the terms of the license GNU GPL v3
  http://www.gnu.org/licenses/gpl-3.0.html (related to original code) and MIT
  License (MIT) for added code.

  CREDENTIALS:
  Author: Libor Gabaj
 */
#ifndef GBJ_APPDATA_H
#define GBJ_APPDATA_H

#if defined(__AVR__)
  #include <Arduino.h>
  // #include <avr/pgmspace.h>
  #include <inttypes.h>
#elif defined(ESP8266)
  #include <Arduino.h>
  // #include <pgmspace.h>
#elif defined(ESP32)
  #include <Arduino.h>
  // #include <pgmspace.h>
#elif defined(PARTICLE)
  #include <Particle.h>
#endif
#include "gbj_serial_debug.h"

#undef SERIAL_PREFIX
#define SERIAL_PREFIX "gbj_appdata"

class gbj_appdata
{
public:
  const char *VERSION = "GBJ_APPDATA 1.0.0";

  enum Datatype
  {
    TYPE_NONE,
    TYPE_BOOL,
    TYPE_BYTE,
    TYPE_INT,
    TYPE_LONG,
    TYPE_UINT,
    TYPE_ULONG,
    TYPE_FLOAT,
    TYPE_STRING,
    TYPE_CCHAR,
  };
  struct Parameter
  {
    const char *name;
    Datatype type;
    String val, valPub;
    bool flAlways, flUse, flShow, flOnce;
    Parameter(const char *key)
      : name(key)
    {
      reset(true);
      show();
      init();
      change();
    }
    const char *getName() { return name; }
    Datatype getType() { return type; }
    void reset(bool force = false)
    {
      if (flOnce || force)
      {
        type = Datatype::TYPE_NONE;
        val = valPub = SERIAL_F("n/a");
      }
    }
    void always() { flAlways = true; }
    void change() { flAlways = false; }
    void init() { flUse = true; }
    void used() { flUse = false; }
    void show() { flShow = true; }
    void hide() { flShow = false; }
    void once() { flOnce = true; }
    void more() { flOnce = false; }
    bool isReady()
    {
      return type != Datatype::TYPE_NONE && flShow && (flAlways || flUse);
    }
    void setValue(String value)
    {
      val = value;
      if (val != valPub)
      {
        init();
      }
    }
    String get() { return val; }
    String publish()
    {
      valPub = val;
      used();
      return get();
    }
    void set(bool value)
    {
      type = Datatype::TYPE_BOOL;
      String newVal = value ? SERIAL_F("true") : SERIAL_F("false");
      setValue(newVal);
    }
    void set(byte value)
    {
      type = Datatype::TYPE_BYTE;
      setValue(String(value));
    }
    void set(int value)
    {
      type = Datatype::TYPE_INT;
      setValue(String(value));
    }
    void set(long value)
    {
      type = Datatype::TYPE_LONG;
      setValue(String(value));
    }
    void set(unsigned int value)
    {
      type = Datatype::TYPE_UINT;
      setValue(String(value));
    }
    void set(unsigned long value)
    {
      type = Datatype::TYPE_ULONG;
      setValue(String(value));
    }
    void set(float value, byte decimals = 4)
    {
      type = Datatype::TYPE_FLOAT;
      String newVal = String(value, decimals);
      setValue(newVal);
    }
    void set(String value)
    {
      type = Datatype::TYPE_STRING;
      setValue(value);
    }
    void set(const char *value)
    {
      type = Datatype::TYPE_CCHAR;
      setValue(String(value));
    }
  };

  /*
    Constructor.

    DESCRIPTION:
    Constructor creates the class instance object and sets credentials for
    IoT platform.

    PARAMETERS: None

    RETURN: object
  */
  inline gbj_appdata() {}

  // Buffer for reading values stored in flash memory.
  // Length should be adjusted for the longest parameter name.
  char progmemBuffer[20];
  inline const char *getPrmName(const char *progmemPrmName)
  {
    strcpy_P(progmemBuffer, progmemPrmName);
    return progmemBuffer;
  }
};

#endif