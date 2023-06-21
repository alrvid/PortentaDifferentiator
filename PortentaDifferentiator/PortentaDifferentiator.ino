/*
*********************************************************************************************************
*                                  Portenta Machine Control Detector
*
*
*                           Copyright 2023 Arduino SA. http://arduino.cc
*
*                                 Original Author: A. Vidstrom
*
*
*                            SPDX-License-Identifier: LGPL-2.1-or-later
*
*   Licensed under the GNU Lesser General Public License 2.1. See LICENSE file in the repository root
*   (https://github.com/alrvid/PortentaDifferentiator) for full license information.
*
*********************************************************************************************************
*/

#include <DigitalIn.h>

enum BoardTypes : uint8_t
{
  BOARD_ERROR,
  BOARD_MACHINE_CONTROL,
  BOARD_PORTENTA
};

enum BoardTypes getBoardType()
{
  int inAfterUp, inAfterDown;
  mbed::DigitalIn vbusPin(PB_14);
  // Inject high level into input, wait 1 ms to settle
  vbusPin.mode(PullUp);
  delay(1);
  // Shortly float the input and read the state
  vbusPin.mode(PullNone);           
  inAfterUp = vbusPin.read();
  // Inject low level into input, wait 1 ms to settle
  vbusPin.mode(PullDown);
  delay(1);
  // Shortly float the input and read the state
  vbusPin.mode(PullNone);
  inAfterDown = vbusPin.read();
  // End floating state and leave the input in pull-up state
  vbusPin.mode(PullUp);
  // The Machine Control has an external pull-up resistor
  // and will never float, the input is always 1.
  // A plain board floats, and the input follows the
  // previously injected level, first 1 and then 0.
  if ((1 == inAfterUp) && (0 == inAfterDown))
  {
    return BOARD_PORTENTA;
  }
  if ((1 == inAfterUp) && (1 == inAfterDown))
  {
    return BOARD_MACHINE_CONTROL;
  }
  else
  {
    return BOARD_ERROR;   // Illegal state, detection failed
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  while (!Serial) ;

  enum BoardTypes boardType = getBoardType();
  switch (boardType)
  {
    case BOARD_PORTENTA:
      Serial.println("The board is a plain Portenta");
      break;
    case BOARD_MACHINE_CONTROL:
      Serial.println("The board is a Portenta Machine Control");
      break;
    case BOARD_ERROR:
      Serial.println("The board couldn't be reliably detected");
      break;
  }
}

void loop() {
  // put your main code here, to run repeatedly:
}