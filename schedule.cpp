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

#include <Arduino.h>
#include <TimeLib.h>
#include "psychoplug.h"
#include "schedule.h"
#include "settings.h"
#include "mqtt.h"
#include "relay.h"
#include "timezone.h"


char *GetActionString(int idx, char *str, int len) {
  switch (idx) {
    case 0 : strncpy_P(str, PSTR("None"), len); break;
    case 1 : strncpy_P(str, PSTR("On"), len); break;
    case 2 : strncpy_P(str, PSTR("Off"), len); break;
    case 3 : strncpy_P(str, PSTR("Toggle"), len); break;
    case 4 : strncpy_P(str, PSTR("Pulse Off"), len); break;
    case 5 : strncpy_P(str, PSTR("Pulse On"), len); break;
    default: strncpy_P(str, PSTR("Invalid"), len); break;
  }
  return str;
}

void PerformAction(int action)
{
  if (action != ACTION_NONE) {
    char str[16];
    MQTTPublish("event", GetActionString(action, str, sizeof(str)));
  }
  
  switch (action) {
    case ACTION_NONE: break;
    case ACTION_ON: SetRelay(true); break;
    case ACTION_OFF: SetRelay(false); break;
    case ACTION_TOGGLE: SetRelay(!GetRelay()); break;
    case ACTION_PULSEOFF: SetRelay(false); delay(2000); SetRelay(true); break;
    case ACTION_PULSEON: SetRelay(true); delay(2000); SetRelay(false); break;
  }
  
}


// Handle automated on/off simply on the assumption we don't lose any minutes
static char lastHour = 25;
static char lastMin = -1;
static char lastDOW = -1;
void ManageSchedule()
{ 
  // Can't run schedule if we don't know what the time is!
  if (timeStatus() == timeNotSet) return;

  // Sane startup time values
  if (lastHour == 25) {
    time_t t = LocalTime(now());
    lastHour = hour(t);
    lastMin = minute(t);
    lastDOW = weekday(t) - 1; // We 0-index, weekday() 1-indexes
  }

  // If this is a new h/m/dow then go through all events and see if we need to do an action.
  // Only execute after we scan everything, so only last action done once even if there are
  // multiple entries for the same time or multiple times scanned.

  time_t t = LocalTime(now());
  int newHour = hour(t);
  int newMin = minute(t);
  int newDOW = weekday(t)-1;
  if (newHour != lastHour || newMin != lastMin || newDOW != lastDOW) {
    int action = ACTION_NONE;
    // This *should* be fast unless we lose a day somewhere (NTP problems?)
    while (newHour != lastHour || newMin != lastMin || newDOW != lastDOW) {
      lastMin++;
      if (lastMin==60) { lastHour++; lastMin = 0; }
      if (lastHour==24) { delay(1); /* allow ctx switch */ lastDOW++; lastHour = 0; }
      if (lastDOW==7) lastDOW = 0; // Sat->Sun
      for (int i=0; i<MAXEVENTS; i++) {
        if ((settings.event[i].dayMask & (1<<lastDOW)) && (settings.event[i].hour == lastHour) && (settings.event[i].minute == lastMin) && (settings.event[i].action != ACTION_NONE)) {
          action = settings.event[i].action;
        }
      }
    }

    PerformAction(action);

    lastHour = hour(t);
    lastMin = minute(t);
    lastDOW = weekday(t)-1; // Sunday=1 for weekday(), we need 0-index
  }
}

void StopSchedule()
{
  // Cause new time to be retrieved.
  lastHour = -1;
  lastMin = -1;
  lastDOW = -1;
}

