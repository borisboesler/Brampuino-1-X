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

//# define DEBUG
//# define VERIFY_EXPOSURE_TIME

# define BRAMPUINO_PRINT_INTERVAL_IN_MENU

//# define HAVE_RTC
# define HAVE_CAMERA
# define HAVE_ADAFRUIT_RGB_SHIELD

/*
 * we use ISO values 50,, 100, 200, .. etc
 */
//
#define FULL_ISO_RANGE

#define ISO_RANGE_MAX_COUNT 20

enum _iso_code_values {
 ISO_50 =    0x00000040,
 ISO_100 =   0x00000048,
 ISO_125 =   0x0000004b,
 ISO_160 =   0x0000004d,
 ISO_200 =   0x00000050,
 ISO_250 =   0x00000053,
 ISO_320 =   0x00000055,
 ISO_400 =   0x00000058,
 ISO_500 =   0x0000005b,
 ISO_640 =   0x0000005d,
 ISO_800 =   0x00000060,
 ISO_1000 =  0x00000063,
 ISO_1250 =  0x00000065,
 ISO_1600 =  0x00000068,
 ISO_2000 =  0x0000006b, /*106, / * guess */
 ISO_2500 =  0x0000006d, /*109, / * guess */
 ISO_3200 =  0x00000070,
 ISO_4000 =  0x00000073, /* guess */
 ISO_5000 =  0x00000075, /* guess */
 ISO_6400 =  0x00000078,
 ISO_12800 = 0x00000080,
 ISO_25600 = 0x00000088
};

/*
 * use left key in menus with explicit value, else use a stack
 */
//#define MENU_USE_LEFT_KEY

#endif
