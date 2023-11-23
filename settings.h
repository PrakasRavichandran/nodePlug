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

#ifndef _settings_h
#define _settings_h

#include <Arduino.h>
#include "password.h"
#include "schedule.h"

#define SETTINGSVERSION (2)

typedef struct {
  byte version;
  char ssid[32];
  char psk[32];
  char hostname[32];
  bool useDHCP;
  byte ip[4];
  byte dns[4];
  byte gateway[4];
  byte netmask[4];
  byte logsvr[4];
  char ntp[48];
  bool use12hr;
  bool usedmy;
  char timezone[32];
  
  bool onAfterPFail;
//  byte voltage;
  
  // MQTT Configuration
  bool mqttEnable;
  char mqttHost[48];
  int mqttPort;
  bool mqttSSL;
  char mqttClientID[32];
  char mqttTopic[32]; 
  char mqttUser[32];
  char mqttPass[32];

  // Web Interface
  char uiUser[32];
  char uiPassEnc[PASSENCLEN];
  char uiSalt[SALTLEN];

  // Events to process
  Event event[MAXEVENTS];
} Settings;
extern Settings settings;

void StartSettings();
bool LoadSettings(bool reset);
void SaveSettings();
void StopSettings();

#endif

