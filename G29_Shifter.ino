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

#include "UnoJoy.h"

// Shifter analog axis thresholds
#define XAXIS_LEFT_THRESH        400
#define XAXIS_RIGHT_THRESH       500
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

void setup() {
  // Shifter analog input pins (make sure they are correctly plugged in Arduino)
  pinMode(A0, INPUT_PULLUP);   // X axis
  pinMode(A2, INPUT_PULLUP);   // Y axis
  pinMode(2, INPUT);           // Reverse button

  // Initialize Joystick Library
  setupUnoJoy();
}

void loop() {
  setControllerData(readController());
  delay(ControllerUpdateRate);
}

dataForController_t readController() {
  dataForController_t controllerData = getBlankDataForController();
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

  // Reset every button to 0
  controllerData.squareOn = 0;    // Button 1
  controllerData.crossOn = 0;     // Button 2
  controllerData.circleOn = 0;    // Button 3
  controllerData.triangleOn = 0;  // Button 4
  controllerData.l1On = 0;        // Button 5
  controllerData.r1On = 0;        // Button 6
  controllerData.l2On = 0;        // Button 7 (reverse)
  controllerData.r2On = 0;        // Button 8 (neutral)

  // Enable the right button based on gear
  switch (gear) {
    case GEAR_1: {
        controllerData.squareOn = 1;
        break;
      }
    case GEAR_2: {
        controllerData.crossOn = 1;
        break;
      }
    case GEAR_3: {
        controllerData.circleOn = 1;
        break;
      }
    case GEAR_4: {
        controllerData.triangleOn = 1;
        break;
      }
    case GEAR_5: {
        controllerData.l1On = 1;
        break;
      }
    case GEAR_6: {
        controllerData.r1On = 1;
        break;
      }
    case GEAR_REVERSE: {
        controllerData.l2On = 1;
        break;
      }
    case GEAR_NEUTRAL: {
        controllerData.r2On = REGISTER_NEUTRAL; 
        break;
      }
  }
  return controllerData;
}
