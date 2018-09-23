/*
 * Arduino ACM1602NI library
 * License: LGPL
 *
 * Copyright (C) 2018 Takayuki Furukawa. All rights reserved.
 *
 * This library is based on Arduino LiquidCrystal library.
 *   GitHub: https://github.com/arduino-libraries/LiquidCrystal
 *   Reference: https://www.arduino.cc/en/Reference/LiquidCrystal
 *
 * Copyright © 2006-2008 Hans-Christoph Steiner. All rights reserved.
 * Copyright (c) 2010 Arduino LLC. All right reserved.
 * This library is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation; either version 2.1 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public
 * License for more details.
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
 *
 * Differense from the LiquidCrystal library:
 *  - Changed words 'LiquidCrystal' to 'ACM1602NI'.
 *  - Changed constructor and init method parameter to I2C slave address.
 *  - Changed initialization process in begin method. Please refer to the
 *    datasheet.
 *  - Removed setRowOffsets method.
 *  - Changed send method to use I2C.
 */

#ifndef __ACM1602NI_H
#define __ACM1602NI_H

#include <inttypes.h>
#include "Print.h"

// slave address
#define ACM1602NI_SLAVE_ADDRESS 0x50

// commands
#define ACM1602NI_CLEARDISPLAY 0x01
#define ACM1602NI_RETURNHOME 0x02
#define ACM1602NI_ENTRYMODESET 0x04
#define ACM1602NI_DISPLAYCONTROL 0x08
#define ACM1602NI_CURSORSHIFT 0x10
#define ACM1602NI_FUNCTIONSET 0x20
#define ACM1602NI_SETCGRAMADDR 0x40
#define ACM1602NI_SETDDRAMADDR 0x80

// flags for display entry mode
#define ACM1602NI_ENTRYRIGHT 0x00
#define ACM1602NI_ENTRYLEFT 0x02
#define ACM1602NI_ENTRYSHIFTINCREMENT 0x01
#define ACM1602NI_ENTRYSHIFTDECREMENT 0x00

// flags for display on/off control
#define ACM1602NI_DISPLAYON 0x04
#define ACM1602NI_DISPLAYOFF 0x00
#define ACM1602NI_CURSORON 0x02
#define ACM1602NI_CURSOROFF 0x00
#define ACM1602NI_BLINKON 0x01
#define ACM1602NI_BLINKOFF 0x00

// flags for display/cursor shift
#define ACM1602NI_DISPLAYMOVE 0x08
#define ACM1602NI_CURSORMOVE 0x00
#define ACM1602NI_MOVERIGHT 0x04
#define ACM1602NI_MOVELEFT 0x00

// flags for function set
#define ACM1602NI_8BITMODE 0x10
#define ACM1602NI_4BITMODE 0x00
#define ACM1602NI_2LINE 0x08
#define ACM1602NI_1LINE 0x00
#define ACM1602NI_5x10DOTS 0x04
#define ACM1602NI_5x8DOTS 0x00

// control byte
#define ACM1602NI_COMMAND 0x00
#define ACM1602NI_DATA 0x80


class ACM1602NI : public Print {
public:
  ACM1602NI();
  ACM1602NI(uint8_t address);
  void init(uint8_t address);
  void begin(uint8_t cols=16, uint8_t rows=2);
  
  void clear();
  void home();

  void noDisplay();
  void display();
  void noBlink();
  void blink();
  void noCursor();
  void cursor();
  void scrollDisplayLeft();
  void scrollDisplayRight();
  void leftToRight();
  void rightToLeft();
  void autoscroll();
  void noAutoscroll();
  void createChar(uint8_t, uint8_t[]);

  void setCursor(uint8_t, uint8_t);
  virtual size_t write(uint8_t);
  void command(uint8_t);

  using Print::write;

private:
  void send(uint8_t value, uint8_t rs);

  uint8_t _address;

  uint8_t _displayfunction;
  uint8_t _displaycontrol;
  uint8_t _displaymode;

  uint8_t _numlines;
  uint8_t _row_offsets[2] = {0x00, 0x40};
};

#endif // __ACM1602NI_H
