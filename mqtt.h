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

#ifndef _mqtt_h
#define _mqtt_h

void StartMQTT();
void ManageMQTT();
void StopMQTT();

void MQTTPublish(const char *key, const char *value);
void MQTTPublishInt(const char *key, const int value);

#endif

