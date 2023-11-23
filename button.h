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

#ifndef _button_h
#define _button_h

#include <Arduino.h>

// Button event types
#define BUTTON_NONE    (0)
#define BUTTON_PRESS   (1)
#define BUTTON_RELEASE (2)

void StartButton();

// Check button state and act appropriately
void ManageButton();

// Used during setup to see raw state of button, not to be used elsewhere
bool RawButton();

#endif

