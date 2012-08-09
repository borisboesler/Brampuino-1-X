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

#include "lcd.h"
#include "debug.h"

#ifdef HAVE_ADAFRUIT_RGB_SHIELD
// The shield uses the I2C SCL and SDA pins. On classic Arduinos
// this is Analog 4 and 5 so you can't use those for analogRead() anymore
// However, you can connect other I2C sensors to the I2C bus and share
// the I2C bus.
Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();
#endif


void lcd_setup()
{
  // set up the LCD's number of rows and columns: 
  lcd.begin(LCD_COLUMNS, LCD_ROWS);
  lcd.setBacklight(BLUE);
}

#ifdef HAVE_ADAFRUIT_RGB_SHIELD
void lcd_set_color(int color)
{
  lcd.setBacklight(color);
}
#endif
