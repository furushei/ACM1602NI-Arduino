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
 */

#include "ACM1602NI.h"

#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include "Wire.h"
#include "Arduino.h"

ACM1602NI::ACM1602NI()
{
  init(ACM1602NI_SLAVE_ADDRESS);
}

ACM1602NI::ACM1602NI(uint8_t address)
{
  init(address);
}

void ACM1602NI::init(uint8_t address)
{
  _address = address;

  _displayfunction = ACM1602NI_8BITMODE | ACM1602NI_2LINE | ACM1602NI_5x8DOTS;
  _displaycontrol = ACM1602NI_DISPLAYON | ACM1602NI_CURSOROFF | ACM1602NI_BLINKOFF;
  _displaymode = ACM1602NI_ENTRYLEFT | ACM1602NI_ENTRYSHIFTDECREMENT;
}

void ACM1602NI::begin(uint8_t cols, uint8_t rows) {
  _numlines = rows;

  Wire.begin();

  // setup process; see ACM1602NI-FLW-FBW datasheet p. 11.
  delayMicroseconds(15000);  // at least 15ms

  command(0x01);
  delayMicroseconds(5000);   // at least 5ms

  command(ACM1602NI_FUNCTIONSET | _displayfunction);
  delayMicroseconds(5000);   // at least 5ms

  command(ACM1602NI_DISPLAYCONTROL | _displaycontrol);
  delayMicroseconds(5000);   // at least 5ms

  command(ACM1602NI_ENTRYMODESET | _displaymode);
  delayMicroseconds(5000);   // at least 5ms
}

/********** high level commands, for the user! */
void ACM1602NI::clear()
{
  command(ACM1602NI_CLEARDISPLAY);  // clear display, set cursor position to zero
  delayMicroseconds(5000);  // this command takes a long time!
}

void ACM1602NI::home()
{
  command(ACM1602NI_RETURNHOME);  // set cursor position to zero
  delayMicroseconds(5000);  // this command takes a long time!
}

void ACM1602NI::setCursor(uint8_t col, uint8_t row)
{
  const size_t max_lines = sizeof(_row_offsets) / sizeof(*_row_offsets);
  if ( row >= max_lines ) {
    row = max_lines - 1;    // we count rows starting w/0
  }
  if ( row >= _numlines ) {
    row = _numlines - 1;    // we count rows starting w/0
  }
  
  command(ACM1602NI_SETDDRAMADDR | (col + _row_offsets[row]));
}

// Turn the display on/off (quickly)
void ACM1602NI::noDisplay() {
  _displaycontrol &= ~ACM1602NI_DISPLAYON;
  command(ACM1602NI_DISPLAYCONTROL | _displaycontrol);
}
void ACM1602NI::display() {
  _displaycontrol |= ACM1602NI_DISPLAYON;
  command(ACM1602NI_DISPLAYCONTROL | _displaycontrol);
}

// Turns the underline cursor on/off
void ACM1602NI::noCursor() {
  _displaycontrol &= ~ACM1602NI_CURSORON;
  command(ACM1602NI_DISPLAYCONTROL | _displaycontrol);
}
void ACM1602NI::cursor() {
  _displaycontrol |= ACM1602NI_CURSORON;
  command(ACM1602NI_DISPLAYCONTROL | _displaycontrol);
}

// Turn on and off the blinking cursor
void ACM1602NI::noBlink() {
  _displaycontrol &= ~ACM1602NI_BLINKON;
  command(ACM1602NI_DISPLAYCONTROL | _displaycontrol);
}
void ACM1602NI::blink() {
  _displaycontrol |= ACM1602NI_BLINKON;
  command(ACM1602NI_DISPLAYCONTROL | _displaycontrol);
}

// These commands scroll the display without changing the RAM
void ACM1602NI::scrollDisplayLeft(void) {
  command(ACM1602NI_CURSORSHIFT | ACM1602NI_DISPLAYMOVE | ACM1602NI_MOVELEFT);
}
void ACM1602NI::scrollDisplayRight(void) {
  command(ACM1602NI_CURSORSHIFT | ACM1602NI_DISPLAYMOVE | ACM1602NI_MOVERIGHT);
}

// This is for text that flows Left to Right
void ACM1602NI::leftToRight(void) {
  _displaymode |= ACM1602NI_ENTRYLEFT;
  command(ACM1602NI_ENTRYMODESET | _displaymode);
}

// This is for text that flows Right to Left
void ACM1602NI::rightToLeft(void) {
  _displaymode &= ~ACM1602NI_ENTRYLEFT;
  command(ACM1602NI_ENTRYMODESET | _displaymode);
}

// This will 'right justify' text from the cursor
void ACM1602NI::autoscroll(void) {
  _displaymode |= ACM1602NI_ENTRYSHIFTINCREMENT;
  command(ACM1602NI_ENTRYMODESET | _displaymode);
}

// This will 'left justify' text from the cursor
void ACM1602NI::noAutoscroll(void) {
  _displaymode &= ~ACM1602NI_ENTRYSHIFTINCREMENT;
  command(ACM1602NI_ENTRYMODESET | _displaymode);
}

void ACM1602NI::createChar(uint8_t location, uint8_t charmap[]) {
  location &= 0x7; // we only have 8 locations 0-7
  command(ACM1602NI_SETCGRAMADDR | (location << 3));
  for (int i=0; i<8; i++) {
    write(charmap[i]);
  }
}

/*********** mid level commands, for sending data/cmds */

inline void ACM1602NI::command(uint8_t value) {
  send(value, ACM1602NI_COMMAND);
}

inline size_t ACM1602NI::write(uint8_t value) {
  send(value, ACM1602NI_DATA);
  return 1; // assume sucess
}

/************ low level data pushing commands **********/
void ACM1602NI::send(byte value, byte rs) {
  Wire.beginTransmission(_address);
  Wire.write(rs);
  Wire.write(value);
  Wire.endTransmission();
}

