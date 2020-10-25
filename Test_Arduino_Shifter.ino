// Logitech Driving Force Shifter USB Tester
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

// Delay after each loop reading the input (in ms)
const int ControllerUpdateRate = 500;

void setup() {
  // Shifter analog input pins (make sure they are correctly plugged in Arduino)
  pinMode(A0, INPUT_PULLUP);   // X axis
  pinMode(A2, INPUT_PULLUP);   // Y axis
  pinMode(2, INPUT);           // Reverse button

  // Serial port to output values
  Serial.begin(9600);
}

void loop() {
  int x = analogRead(0);                // X axis
  int y = analogRead(2);                // Y axis
  int is_reverse = digitalRead(2);      // Reverse button

  char buffer[64];
  snprintf(buffer, sizeof(buffer), "x: %d, y: %d, is_reverse: %d", x, y, is_reverse);
  Serial.println(buffer);

  delay(ControllerUpdateRate);
}
