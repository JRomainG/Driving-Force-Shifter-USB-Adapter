// Logitech Driving Force Shifter USB Adapter
// Inspired by projects from Armandoiglesias and Jason Duncan
// Copyright (C) 2020  Jean-Romain Garnier, Constant Zion
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include "Joystick.h"

Joystick_ Joystick;

// Shifter analog axis thresholds
// Change these values if the gears aren't detected correctly
#define XAXIS_LEFT_THRESH        300
#define XAXIS_RIGHT_THRESH       700
#define YAXIS_UP_THRESH          700
#define YAXIS_DOWN_THRESH        300

#define GEAR_1                   1
#define GEAR_2                   2
#define GEAR_3                   3
#define GEAR_4                   4
#define GEAR_5                   5
#define GEAR_6                   6
#define GEAR_REVERSE             7
#define GEAR_NEUTRAL             0

// Whether switching to neutral should register as a button (0 or 1)
#define REGISTER_NEUTRAL         0

// Delay after each loop reading the input (in ms)
const int ControllerUpdateRate = 50;

// Joystick button pressed during the last update (if any)
int previous_gear = -1;

void setup() {
  // Shifter analog input pins (make sure they are correctly plugged in Arduino)
  pinMode(A0, INPUT_PULLUP);   // X axis
  pinMode(A2, INPUT_PULLUP);   // Y axis
  pinMode(2, INPUT);           // Reverse button

  // Initialize Joystick Library
  Joystick.begin();
}

void loop() {
  updateControllerState();
  delay(ControllerUpdateRate);
}

void updateControllerState() {
  int x = analogRead(0);                // X axis
  int y = analogRead(2);                // Y axis
  int is_reverse = digitalRead(2);      // Reverse button

  // Find out which gear the user is now in
  int gear = GEAR_NEUTRAL;

  if (x < XAXIS_LEFT_THRESH) {
    if (y > YAXIS_UP_THRESH) {
      gear = GEAR_1;
    } else if (y < YAXIS_DOWN_THRESH) {
      gear = GEAR_2;
    }
  } else if (x > XAXIS_RIGHT_THRESH) {
    if (y > YAXIS_UP_THRESH) {
      gear = GEAR_5;
    } else if (y < YAXIS_DOWN_THRESH) {
      if (is_reverse) {
        gear = GEAR_REVERSE;
      } else {
        gear = GEAR_6;
      }
    }
  } else {
    if (y > YAXIS_UP_THRESH) {
      gear = GEAR_3;
    } else if (y < YAXIS_DOWN_THRESH) {
      gear = GEAR_4;
    }
  }

  // Don't send unnecessary button events
  if (previous_gear == gear) {
    return;
  }

  // Decide which gears register as button presses
  int min_gear = (REGISTER_NEUTRAL == 0) ? 1 : 0

  // Release the previous button if necessary...
  if (previous_gear >= min_gear) {
    Joystick.releaseButton(previous_gear);
  }

  // And press the new one
  if (gear >= min_gear) {
    Joystick.pressButton(gear);
  }

  previous_gear = gear;
}
