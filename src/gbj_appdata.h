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
    String val;
    bool flAlways, flShow, flNew, flRepeat, flPub, flEvent;
    Parameter(const char *key)
      : name(key)
    {
      pubReset(true);
      flShow = true;
      flNew = false;
      flEvent = false;
      flAlways = false;
    }
    const char *getName() { return name; }
    Datatype getType() { return type; }
    void pubInit() { flPub = true; }
    void pubReset(bool force = false)
    {
      if (force)
      {
        type = Datatype::TYPE_NONE;
        val = SERIAL_F("n/a");
      }
      flPub = false;
    }
    bool isPub() { return isSet() && flShow && (flAlways || flPub); }
    bool isEvent() { return isSet() && flShow && flEvent; }
    void always() { flAlways = true; }
    void change() { flAlways = false; }
    void show() { flShow = true; }
    void hide() { flShow = false; }
    void repeat() { flRepeat = true; }
    bool isSet() { return type != Datatype::TYPE_NONE; }
    bool isNew() { return flNew; }
    void setValue(String value)
    {
      flNew = !value.equals(val);
      if (flNew || flRepeat)
      {
        flEvent = flPub = true;
      }
      val = value;
    }
    String get() { return val; }
    String publish()
    {
      flPub = false;
      return get();
    }
    String event()
    {
      flEvent = false;
      return get();
    }
    String set(bool value)
    {
      type = Datatype::TYPE_BOOL;
      String newVal = value ? SERIAL_F("true") : SERIAL_F("false");
      setValue(newVal);
      return get();
    }
    String set(byte value)
    {
      type = Datatype::TYPE_BYTE;
      setValue(String(value));
      return get();
    }
    String set(int value)
    {
      type = Datatype::TYPE_INT;
      setValue(String(value));
      return get();
    }
    String set(long value)
    {
      type = Datatype::TYPE_LONG;
      setValue(String(value));
      return get();
    }
    String set(unsigned int value)
    {
      type = Datatype::TYPE_UINT;
      setValue(String(value));
      return get();
    }
    String set(unsigned long value)
    {
      type = Datatype::TYPE_ULONG;
      setValue(String(value));
      return get();
    }
    String set(float value, unsigned int decimals = 4)
    {
      type = Datatype::TYPE_FLOAT;
      String newVal = String(value, decimals);
      setValue(newVal);
      return get();
    }
    String set(String value)
    {
      type = Datatype::TYPE_STRING;
      setValue(value);
      return get();
    }
    String set(const char *value)
    {
      type = Datatype::TYPE_CCHAR;
      setValue(String(value));
      return get();
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