/*
 * configuration of hardware and software
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

#ifndef __CONFIG_H__
# define __CONFIG_H__

/* DEBUG and HAVE_CAMERA do not work together */
//# define DEBUG
//# define VERIFY_EXPOSURE_TIME

# define BRAMPUINO_PRINT_INTERVAL_IN_MENU

//# define HAVE_RTC
# define HAVE_CAMERA
# define HAVE_ADAFRUIT_RGB_SHIELD

/*
 * use left key in menus with explicit value, else use a stack
 */
//#define MENU_USE_LEFT_KEY

#endif
