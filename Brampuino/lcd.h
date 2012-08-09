/*
 * interface to the LCD
 * Copyright (C) 2012 Boris Boesler
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not sse <http://www.gnu.org/licenses/>.
 */

#ifndef __LCD_H__
# define __LCD_H__ 

# include "config.h"

/**
 * LCD colors, probably limited use
 */
enum {
  RED = 0x1,
  GREEN = 0x2,
  YELLOW = 0x3,
  BLUE = 0x4,
  VIOLET = 0x5,
  TEAL = 0x6,
  WHITE = 0x7
};


#ifdef HAVE_ADAFRUIT_RGB_SHIELD
# include <Wire.h>
# include <Adafruit_MCP23017.h>
# include <Adafruit_RGBLCDShield.h>

# define LCD_ROWS 2
# define LCD_COLUMNS 16

extern Adafruit_RGBLCDShield lcd;
#endif

# define PRINTLN(...) lcd.println(__VA_ARGS__)
# define PRINT(...) lcd.print(__VA_ARGS__)

extern void lcd_setup();
#ifdef HAVE_ADAFRUIT_RGB_SHIELD
extern void lcd_set_color(int color);
#endif

#endif
