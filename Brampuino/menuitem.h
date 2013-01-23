/*
 * interface to the menu system
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

#ifndef __MENUITEM_H__
# define __MENUITEM_H__

# include <stdio.h>
# include <limits.h>
# include "config.h"

// for functions
# include "lcd.h"

/*
 * maximum stack size if LEFT key is not used
 */
#define MENU_STACK_SIZE 10

#define NO_MENU ((const menu_entry_t*)NULL)
#define NO_FUNC ((const menu_function_t)NULL)

enum menu_buttons {
  /* main menu */
  MENU_BUTTON_MAIN_INCREMENT = BUTTON_UP,
  MENU_BUTTON_MAIN_DECREMENT = BUTTON_DOWN,
  MENU_BUTTON_MAIN_MENU = BUTTON_SELECT,
  MENU_BUTTON_MAIN_CHANGE_ISO = BUTTON_LEFT,
  MENU_BUTTON_MAIN_CANCEL = BUTTON_RIGHT,

  /* menu navigation - ala MX2 */
  MENU_BUTTON_MENU_PREV = BUTTON_UP,
  MENU_BUTTON_MENU_NEXT = BUTTON_DOWN,
  MENU_BUTTON_MENU_UP = BUTTON_RIGHT,
  MENU_BUTTON_MENU_ENTER = BUTTON_SELECT,

  /* buttons for value input - ala MX2 */
  MENU_BUTTON_VALUE_INCREMENT = BUTTON_UP,
  MENU_BUTTON_VALUE_DECREMENT = BUTTON_DOWN,
  MENU_BUTTON_VALUE_CANCEL = BUTTON_RIGHT,
  MENU_BUTTON_VALUE_SELECT = BUTTON_SELECT
};


/**
 * type of executed function when a button is pressed
 */
typedef void (*menu_function_t)(int);

/**
 * type of menu
 */
typedef struct _menu_entry_t {
  const char* item;            /**< menu string */
#ifdef MENU_USE_LEFT_KEY
  const struct _menu_entry_t* left;  /**< menu to select if LEFT is pressed */
#endif
  const struct _menu_entry_t* right; /**< menu to select if RIGHT is pressed */
  menu_function_t func_right;  /**< function to execute when RIGHT is pressed */
  menu_function_t func_select; /**< function to execute when SELECT is pressed */
} menu_entry_t;


/**
 * currently selectd menu
 */
extern const menu_entry_t *menu_current;


/**
 * first displayed menu on LCD
 */
extern int menu_start_line;


/**
 * selected menu in LCD with some cursor
 */
extern int menu_cursor_line;

/**
 * true iff we changed a value via the menu / did not use CANCEL
 */
extern bool menu_changed_settings;

/**
 * \{
 */

/**
 * print a string a get a signed long via the buttons
 */
extern
long menu_signed_long(const char* menu, long val,
		      long min_val = LONG_MIN, long max_val = LONG_MAX);

/**
 * print a string a get an unsigned long via the buttons
 */
extern
unsigned long menu_unsigned_long(const char* menu, unsigned long val,
				 unsigned long min_val = 0,
				 unsigned long max_val = ULONG_MAX);

/**
 * print a string a get an unsigned long from an array via the buttons
 */
extern
unsigned menu_index_unsigned_long_array(const char* menu, unsigned long val,
					unsigned long *array,
					unsigned num);

/**
 * print a string a get a float via the buttons
 */
extern
float menu_float(const char* menu, float val, float step, bool sign = false);

/**
 * print a string a get a bool via the buttons
 */
extern
bool menu_bool(const char* menu, bool val);


/**
 * \}
 */

/**
 * start delay time
 */
void menu_select_delay_time(int unused);

/**
 * \{
 */

/*
 * exposure related
 */

/**
 * exposure start time
 */
void menu_select_exp_time(int unused);

/**
 * minimal exposure time
 */
void menu_select_exp_min(int unused);

/**
 * maximal exposure time
 */
void menu_select_exp_max(int unused);

/**
 * select exposure offset
 */
void menu_select_exp_offset(int menu_num);


/**
 * minimal ISO
 */
void menu_select_iso_min(int menu_num);

/**
 * maximal ISO time
 */
void menu_select_iso_max(int menu_num);

/**
 * ISO auto ramp
 */
void menu_select_iso_ramp(int menu_num);

/**
 * \}
 */


/**
 *
 */
void menu_select_exp_increment(int menu_num);

/**
 * exponentiel change
 * \{
 */

/**
 *
 */
void menu_select_exp_fps(int menu_num);

/**
 *
 */
void menu_select_ramp_time(int menu_num);

/**
 *
 */
void menu_select_exp_ev_change(int menu_num);

/**
 * lead in
 */
void menu_select_exp_lead_in(int menu_num);

/**
 * \}
 */

/**
 * \{
 */

/*
 * interval related
 */

/**
 *
 */
void menu_select_interval_time(int unused);

/**
 * minimal interval time
 */
void menu_select_interval_min(int unused);

/**
 * maximal interval time
 */
void menu_select_interval_max(int unused);

/**
 * \}
 */


/**
 * number of maximal exposures
 */
void menu_select_max_exposures(int unused);


/**
 * number of maximal exposures
 */
void menu_select_move_focus_step(int unused);


/**
 * start was selected
 */
void menu_select_start(int color_line);


/**
 * switch to a color selected in menu
 */
void menu_lcd_select_color(int color_line);


/**
 * \{
 */

/**
 * set entry in real time clock
 */
void menu_select_clock_entry(int unused);

/**
 * \}
 */

/**
 * print version
 */
void menu_select_version(int unused);


#if 0
// unused
/**
 *
 */
void menu_select_ext_trigger(int unused);
#endif

/**
 * loop of menu
 */
extern void menu_loop();


#endif
