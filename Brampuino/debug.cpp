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


# ifdef ARDUINO
#  include <Arduino.h>
//# include <Wire.h>
# endif

#include "config.h"
#include "debug.h"

#ifdef DEBUG

/**
 * copy PROGMEM strigs to this buffer for printing
 */
char debug_string_buffer[DEBUG_STRING_BUFFER_LENGTH];

#endif
