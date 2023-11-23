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

#ifndef _relay_h
#define _relay_h

// Initializes relay control pins (relay state undefined)
void StartRelay(bool state);

// Sets the relay on or off and handles any logging required
void SetRelay(bool on);

// Returns current state of relay
bool GetRelay();

#endif

