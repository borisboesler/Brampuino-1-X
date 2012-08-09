/*
 * handling buttons
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

#ifndef __BUTTON_H__
# define __BUTTON_H__

# include "config.h"

# ifdef HAVE_ADAFRUIT_RGB_SHIELD
// reuse buttons on the RGB LCD shield
#  include "lcd.h"
#  define READ_BUTTONS() lcd.readButtons()
# else
enum {
  BUTTON_UP = 'u',
  BUTTON_DOWN = 'd',
  BUTTON_LEFT = 'l',
  BUTTON_RIGHT = 'r',
  BUTTON_SELECT = 's',
  BUTTON_QUIT = 'q',
};

# endif

/**
 * setup the buttons
 */
extern void button_setup(void);

#endif
