/*
 * debugging
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

#ifndef __DEBUG_H__
# define __DEBUG_H__

# ifdef ARDUINO
#  include <Arduino.h>
//# include <Wire.h>
# endif

# ifdef DEBUG
#  define DEBUG_GLOBAL_INIT Serial.begin(115200)
#  define DEBUG_GLOBAL_FINI
#  define DEBUG_PRINTLN_PSTR(_str) Serial.println(_str)
#  define DEBUG_PRINT_PSTR(_str) Serial.print(_str)
#  define DEBUG_PRINTLN(...) Serial.println(__VA_ARGS__)
#  define DEBUG_PRINT(...) Serial.print(__VA_ARGS__)
# else
#  define DEBUG_GLOBAL_INIT Serial.begin(115200)
#  define DEBUG_GLOBAL_FINI
#  define DEBUG_PRINTLN_PSTR(...)
#  define DEBUG_PRINT_PSTR(...)
#  define DEBUG_PRINTLN(...)
#  define DEBUG_PRINT(...)
# endif


#endif
