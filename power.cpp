/*

  nodePlug
  ESP8266 based remote outlet with standalone timer and MQTT integration
  
  Copyright (C) 2023  Prakash Ravichandran, PM

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

*/


// Unable to reliably read the power meter, remove it for now.
#if 0
#include <Arduino.h>
#include <Wire.h>
#include "psychoplug.h"
#include "settings.h"
#include "mqtt.h"
#include "log.h"


// Noisy current measurement, better than nothing I guess
static int lastCurrentMa = 0;

#define PIN_SDA  (12)
#define PIN_SCL  (0)


static void ReadPowerMonitor();

int GetCurrentMA()
{
  return lastCurrentMa;
}

void StartPowerMonitor()
{
  // Power Monitoring
  pinMode(PIN_SDA, INPUT_PULLUP);
  Wire.begin(PIN_SDA, PIN_SCL);
}

void ManagePowerMonitor()
{
  static unsigned long lastReadMS = 0;
  unsigned long ms = millis();
  if (ms < lastReadMS) lastReadMS = ms; // Overflow skip one reading
  unsigned long delta = ms - lastReadMS;
  if (delta > 10000) {
    lastReadMS = millis();
    ReadPowerMonitor();
    MQTTPublishInt("powerma", lastCurrentMa);
  }
}

void StopPowerMonitor()
{
  lastCurrentMa = 0;
}

static void ReadPowerMonitor()
{
  char d[12];
  uint32_t rawPwr[3];
  byte count = 0;
  int timeout = 1000;
  Wire.requestFrom(0, 12);
  while (count < 12 && timeout--) {
    if (Wire.available()) d[count++] = Wire.read();
    else delay(1);
  }
  if (timeout <= 0 ) {
    rawPwr[0] = rawPwr[1] = rawPwr[2] = 0;
  } else {
    rawPwr[0] = ((uint32_t)d[0]<<24) | ((uint32_t)d[1]<<16) | ((uint32_t)d[2]<<8) | ((uint32_t)d[3]);
    rawPwr[1] = ((uint32_t)d[4]<<24) | ((uint32_t)d[5]<<16) | ((uint32_t)d[6]<<8) | ((uint32_t)d[7]);
    rawPwr[2] = ((uint32_t)d[8]<<24) | ((uint32_t)d[9]<<16) | ((uint32_t)d[10]<<8) | ((uint32_t)d[11]);
  }

  LogPrintf("%12d(%08x), %12d(%08x), %12d(%08x)", rawPwr[0], rawPwr[0], rawPwr[1], rawPwr[1], rawPwr[2], rawPwr[2]);
  LogPrintf("  --- %12d(%08x), %12d(%08x), %12d(%08x)\n", rawPwr[0]&0x7fffffff, rawPwr[0]&0x7fffffff, rawPwr[1&0x7fffffff], rawPwr[1]&0x7fffffff, rawPwr[2]&0x7fffffff, rawPwr[2]&0x7fffffff);
  // Current measurement via curve-fit from samples.  YMMV, but fits well for me
  // Needs Vin to get power (well, and PF, but that's more than we can measure)
  double x, y;
  x = (double) rawPwr[0];
  y = 29125.43749 * x / (x + 103257.9289) - 12813.31867 * x / (x + 48884.1186); 
  if (y<0) y=0;
  lastCurrentMa = (int)y;
}

#else

int GetCurrentMA() { return 0; }
void StartPowerMonitor() {}
void ManagePowerMonitor() {}
void StopPowerMonitor() {}

#endif
