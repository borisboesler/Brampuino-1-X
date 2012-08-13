/*
 * running the menu system
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

#include "config.h"
#include "debug.h"

#include <stdlib.h>
#include <stdio.h>

#include "brampuino.h"
#include "menuitem.h"
#include "lcd.h"
#include "button.h"

#ifdef HAVE_RTC
# include "RTClib.h"
#endif

/* forward declarations */
extern const menu_entry_t menu_top[];
extern const menu_entry_t menu_settings[];
extern const menu_entry_t menu_lcd[];
extern const menu_entry_t menu_clock[];
extern const menu_entry_t menu_exposure[];
extern const menu_entry_t menu_iso[];
extern const menu_entry_t menu_interval[];

#define DEBOUNCE_TIME 40
#define DEBOUNCE_COUNT 10
#define DEBOUNCE_MULTIPLIED 2

/**
 * for reading from program memory
 */
static char buffer[LCD_COLUMNS + 1];

/* ******************** */
/* menu data structure */

/*
 * generate a stack to return to previous menu
 */
#ifndef MENU_USE_LEFT_KEY
static const menu_entry_t* menu_stack[MENU_STACK_SIZE];
static int menu_sp = -1;
#endif

static void menu_push(const menu_entry_t* entry)
{
  if(menu_sp < MENU_STACK_SIZE - 1) {
    menu_stack[++menu_sp] = entry;
  }
  else {
    /* Error, stack too small */
    DEBUG_PRINTLN_PSTR("Error, stack too small");
  }
}

static const menu_entry_t* menu_toppop()
{
  const menu_entry_t* res = NO_MENU;
  if(0 <= menu_sp) {
    res = menu_stack[menu_sp--];
  }
  else {
    DEBUG_PRINTLN_PSTR("Error, stack empty");
  }
  return(res);
}

/**
 * currently selectd menu
 */
const menu_entry_t *menu_current = menu_top;


/**
 * first displayed menu on LCD
 */
int menu_start_line = 0;


/**
 * selected menu in LCD with some cursor
 */
int menu_cursor_line = 0;


/**
 * true iff we changed a value via the menu / did not use CANCEL
 */
bool menu_changed_settings = false;


/*
 * read buttons and return a value
 */

/*
 * print a string a get a signed long via the buttons
 */
long menu_signed_long(const char* menu, long val, long min_val, long max_val)
{
  long old_val = val;
  lcd.clear();
  lcd.setCursor(0, 0);
  PRINT(menu);

  // read buttons
  uint8_t c1, c2;
  int pressed = 0;
  long adding = 1;
  int multiplied = 0;
  do {
    lcd.setCursor(0, 1);
    PRINT(val); PRINT(" ");
    c1 = READ_BUTTONS();
    delay(DEBOUNCE_TIME);
    c2 = READ_BUTTONS();
    if(c1 && (c1 == c2)) {
      ++pressed;
      if((DEBOUNCE_MULTIPLIED > multiplied) && (DEBOUNCE_COUNT < pressed)) {
	pressed = 1;
	adding *= 10;
	++multiplied;
      }
      switch(c1) {
      case MENU_BUTTON_DECREMENT:
	// check wrap around
	if(val - adding < val) {
	  val -= adding;
	}
	break;
      case MENU_BUTTON_INCREMENT:
	// check wrap around
	if(val + adding > val) {
	  val += adding;
	}
	break;
      case MENU_BUTTON_CANCEL:
	/* cancel */
	return(old_val);
	break;
      default:
	// skip
	break;
      }
    }
    else {
      pressed = 0;
      adding = 1;
      multiplied = 0;
    }
  } while(MENU_BUTTON_SELECT != c1);

  menu_changed_settings = true;
  return(val);
}


/*
 * print a string a get a signed long via the buttons
 */
unsigned long menu_unsigned_long(const char* menu, unsigned long val,
				 unsigned long min_val, unsigned long max_val)
{
  unsigned long old_val = val;
  lcd.clear();
  lcd.setCursor(0, 0);
  PRINT(menu);

  // read buttons
  uint8_t c1, c2;
  int pressed = 0;
  unsigned long adding = 1;
  int multiplied = 0;
  do {
    lcd.setCursor(0, 1);
    PRINT(val); PRINT(" ");
    c1 = READ_BUTTONS();
    delay(DEBOUNCE_TIME);
    c2 = READ_BUTTONS();
    if(c1 && (c1 == c2)) {
      ++pressed;
      if((DEBOUNCE_MULTIPLIED > multiplied) && (DEBOUNCE_COUNT < pressed)) {
	pressed = 1;
	adding *= 10;
	++multiplied;
      }
      switch(c1) {
      case MENU_BUTTON_DECREMENT:
	// check wrap around
	if(val - adding < val) {
	  val -= adding;
	}
	val = MAX(min_val, val);
	break;
      case MENU_BUTTON_INCREMENT:
	// check wrap around
	if(val + adding > val) {
	  val += adding;
	}
	val = MIN(max_val, val);
	break;
      case MENU_BUTTON_CANCEL:
	/* cancel */
	return(old_val);
	break;
      default:
	// skip
	break;
      }
    }
    else {
      pressed = 0;
      adding = 1;
      multiplied = 0;
    }
  } while(MENU_BUTTON_SELECT != c1);

  menu_changed_settings = true;
  return(val);
}


/*
 * print a string a get an unsigned long from an array via the buttons
 */
unsigned menu_index_unsigned_long_array(const char* menu, unsigned long val,
					unsigned long *array,
					unsigned num)
{
  unsigned long old_val = val;
  lcd.clear();
  lcd.setCursor(0, 0);
  PRINT(menu);

  // read buttons
  uint8_t c1, c2;
  unsigned index = val;
  do {
    lcd.setCursor(0, 1);
    PRINT(array[index]); PRINT(" ");
    c1 = READ_BUTTONS();
    delay(DEBOUNCE_TIME);
    c2 = READ_BUTTONS();
    if(c1 && (c1 == c2)) {
      switch(c1) {
      case MENU_BUTTON_DECREMENT:
	if(0 < index) {
	  --index;
	}
	break;
      case MENU_BUTTON_INCREMENT:
	if(index + 1 < num) {
	  ++index;
	}
	break;
      case MENU_BUTTON_CANCEL:
	/* cancel */
	return(old_val);
	break;
      default:
	// skip
	break;
      }
    }
  } while(MENU_BUTTON_SELECT != c1);

  menu_changed_settings = true;
  val = index;
  return(val);
}


/*
 * print a string a get a float via the buttons
 */
float menu_float(const char* menu, float val, float step, bool sign)
{
  float old_val = val;
  lcd.clear();
  lcd.setCursor(0, 0);
  PRINT(menu);

  // read buttons
  uint8_t c1, c2;
  int pressed = 0;
  float adding = step;
  int multiplied = 0;

  do {
    lcd.setCursor(0, 1);
    PRINT(val); PRINT(" ");
    c1 = READ_BUTTONS();
    delay(DEBOUNCE_TIME);
    c2 = READ_BUTTONS();
    if(c1 && (c1 == c2)) {
      ++pressed;
      if((DEBOUNCE_MULTIPLIED > multiplied) && (DEBOUNCE_COUNT < pressed)) {
	pressed = 1;
	adding *= 10;
	++multiplied;
      }
      switch(c1) {
      case MENU_BUTTON_DECREMENT:
	val -= adding;
	if((0 > val) && !sign) {
	  val = 0;
	}
	break;
      case MENU_BUTTON_INCREMENT:
	val += adding;
	break;
      case MENU_BUTTON_CANCEL:
	/* cancel */
	return(old_val);
	break;
      default:
	// skip
	break;
      }
    }
    else {
      pressed = 0;
      adding = step;
      multiplied = 0;
    }
  } while(MENU_BUTTON_SELECT != c1);

  menu_changed_settings = true;
  return(val);
}


/*
 * print a string a get a bool via the buttons
 */
extern
bool menu_bool(const char* menu, bool val)
{
  bool old_val = val;
  lcd.clear();
  lcd.setCursor(0, 0);
  PRINT(menu);

  // read buttons
  uint8_t c1, c2;
  do {
    lcd.setCursor(0, 1);
    if(val) {
      PRINT("On ");
    }
    else {
       PRINT("Off");
    }

    c1 = READ_BUTTONS();
    delay(DEBOUNCE_TIME);
    c2 = READ_BUTTONS();
    if(c1 == c2) {
      switch(c1) {
      case MENU_BUTTON_DECREMENT:
	val = false;
	break;
      case MENU_BUTTON_INCREMENT:
	val = true;
	break;
      case MENU_BUTTON_CANCEL:
	/* cancel */
	return(old_val);
	break;
      default:
	// skip
	break;
      }
    }
  } while(MENU_BUTTON_SELECT != c1);

  menu_changed_settings = true;
  return(val);
}


/* ************************************************************ */

/*
 * print double test for minimum and maximum
 */
void menu_minmax_int(const char *text, long unsigned* val1, long unsigned* val2,
		     unsigned long min_val, unsigned long max_val)
{
  const int lines = 2;
  // save old values
  volatile unsigned long values[lines] = { *val1, *val2 };
  const char* prefixes[lines] = {
    "Min.",
    "Max."
  };

  int row = strlen(text) + 5 /* strlen("Min.")+strlen(":")*/;

  // print fixed menu
  lcd.clear();
  for(int y = 0; y < lines; y++) {
    lcd.setCursor(0, y);
    PRINT(prefixes[y]); PRINT(text); PRINT(":");
  }

  // turn cursor on
  lcd.cursor();
  lcd.blink();

  // read buttons
  uint8_t c1, c2;
  int line = 0;
  bool redraw = true;
  int pressed = 0;
  unsigned long adding = 1;
  int multiplied = 0;

  do {
    // print current values
    if(redraw) {
      for(int y = 0; y < lines; y++) {
	lcd.setCursor(row, y);
	PRINT(values[y]); PRINT("  ");
      }
      redraw = false;
    }
    lcd.setCursor(row, line);

    c1 = READ_BUTTONS();
    delay(DEBOUNCE_TIME);
    c2 = READ_BUTTONS();
    if(c1 && (c1 == c2)) {
      ++pressed;
      if((DEBOUNCE_MULTIPLIED > multiplied) && (DEBOUNCE_COUNT < pressed)) {
	pressed = 1;
	adding *= 10;
	++multiplied;
      }
      switch(c1) {
	// menu
      case MENU_BUTTON_NEXT:
	line = MIN(lines - 1, line + 1);
	break;
      case MENU_BUTTON_PREVIOUS:
	line = MAX(0, line - 1);
	break;
	// value
      case MENU_BUTTON_DECREMENT:
	values[line] = MAX(min_val, values[line] - adding);
	redraw = true;
	break;
      case MENU_BUTTON_INCREMENT:
	values[line] = MIN(max_val, values[line] + adding);
	redraw = true;
	break;
      default:
	// skip
	break;
      }
    }
    else {
      pressed = 0;
      adding = 1;
      multiplied = 0;
    }
  } while(MENU_BUTTON_SELECT != c1);

  // turn cursor off
  lcd.noCursor();
  lcd.noBlink();


  // store values
  *val1 = values[0];
  *val2 = values[1];
  menu_changed_settings = true;
}

/* ************************************************************ */
/*
 * menu functions
 */

/*
 * delay
 */

/**
 * start delay time
 */
void menu_select_delay_time(int menu_num)
{
  strcpy_P(buffer, menu_settings[menu_num].item);
  settings.start_delay = menu_unsigned_long(buffer, settings.start_delay);
  DEBUG_PRINT_PSTR("set delay time to:");
  DEBUG_PRINTLN(settings.start_delay);
}


/*
 * exposure
 */

/**
 * exposure start time
 */
void menu_select_exp_time(int menu_num)
{
  strcpy_P(buffer, menu_exposure[menu_num].item);
  settings.exposure.start_time = menu_unsigned_long(buffer,
						    settings.exposure.start_time,
						    BRAMPUINO_MIN_EXPOSURE_TIME,
						    BRAMPUINO_MAX_EXPOSURE_TIME);
  DEBUG_PRINT_PSTR("set exp. time to:");
  DEBUG_PRINTLN(settings.exposure.start_time);
}


/*
 * minimal exposure time
 */
void menu_select_exp_min(int menu_num)
{
  strcpy_P(buffer, menu_exposure[menu_num].item);
  settings.exposure.min = menu_unsigned_long(buffer, settings.exposure.min,
					     BRAMPUINO_MIN_EXPOSURE_TIME,
					     BRAMPUINO_MAX_EXPOSURE_TIME);
  DEBUG_PRINT_PSTR("set exp min to:");
  DEBUG_PRINTLN(settings.exposure.min);
}


/*
 * maximal exposure time
 */
void menu_select_exp_max(int menu_num)
{
  strcpy_P(buffer, menu_exposure[menu_num].item);
  settings.exposure.max = menu_unsigned_long(buffer, settings.exposure.max,
					     BRAMPUINO_MIN_EXPOSURE_TIME,
					     BRAMPUINO_MAX_EXPOSURE_TIME);
  DEBUG_PRINT_PSTR("set exp max to:");
  DEBUG_PRINTLN(settings.exposure.max);
}


/*
 * select exposure offset
 */
void menu_select_exp_offset(int menu_num)
{
  strcpy_P(buffer, menu_exposure[menu_num].item);
  settings.exposure.offset = menu_unsigned_long(buffer, settings.exposure.offset,
						BRAMPUINO_MIN_OFFSET,
						BRAMPUINO_MAX_OFFSET);
  DEBUG_PRINT_PSTR("set exp offset to:");
  DEBUG_PRINTLN(settings.exposure.offset);
}


/*
 * minimal ISO
 */
void menu_select_iso_min(int menu_num)
{
  strcpy_P(buffer, menu_iso[menu_num].item);
  settings.iso.min_index
    = menu_index_unsigned_long_array(buffer,
				     settings.iso.min_index,
				     iso_values,
				     sizeof(iso_values) / sizeof(unsigned long));
  DEBUG_PRINT_PSTR("set ISO min index to:");
  DEBUG_PRINTLN(settings.iso.min_index);
}


/*
 * maximal ISO
 */
void menu_select_iso_max(int menu_num)
{
  strcpy_P(buffer, menu_iso[menu_num].item);
  settings.iso.max_index
    = menu_index_unsigned_long_array(buffer,
				     settings.iso.max_index,
				     iso_values,
				     sizeof(iso_values) / sizeof(unsigned long));
  DEBUG_PRINT_PSTR("set ISO max index to:");
  DEBUG_PRINTLN(settings.iso.max_index);
}


/*
 * ISO Ramp
 */
void menu_select_iso_ramp(int menu_num)
{
  strcpy_P(buffer, menu_iso[menu_num].item);
  settings.iso.auto_ramp = menu_bool(buffer, settings.iso.auto_ramp);
  DEBUG_PRINT_PSTR("set ISO auto ramp to:");
  DEBUG_PRINTLN(settings.iso.auto_ramp);
}



/*
 * exposure increment
 */
void menu_select_exp_increment(int menu_num)
{
  strcpy_P(buffer, menu_exposure[menu_num].item);
  settings.exposure.u.linear.increment
    = menu_signed_long(buffer, settings.exposure.u.linear.increment);
  DEBUG_PRINT_PSTR("set increment time to:");
  DEBUG_PRINTLN(settings.exposure.u.linear.increment);
}

/**
 *
 */
void menu_select_exp_fps(int menu_num)
{
  strcpy_P(buffer, menu_exposure[menu_num].item);
  settings.fps = menu_unsigned_long(buffer, settings.fps, 1, 1000);
  DEBUG_PRINT_PSTR("set FPS to:");
  DEBUG_PRINTLN(settings.fps);
}

/**
 *
 */
void menu_select_exp_ev_change(int menu_num)
{
  strcpy_P(buffer, menu_exposure[menu_num].item);
  settings.exposure.u.exponential.ev_change
    = menu_float(buffer, settings.exposure.u.exponential.ev_change, 0.1, true);
  DEBUG_PRINT_PSTR("set exposure EV change to:");
  DEBUG_PRINTLN(settings.exposure.u.exponential.ev_change);
}



/*
 * interval
 */

/**
 * start interval time
 */
void menu_select_interval_time(int menu_num)
{
  strcpy_P(buffer, menu_interval[menu_num].item);
  settings.interval.time = menu_unsigned_long(buffer, settings.interval.time,
					      BRAMPUINO_MIN_INTERVAL,
					      BRAMPUINO_MAX_INTERVAL);
  DEBUG_PRINT_PSTR("set interval time to:");
  DEBUG_PRINTLN(settings.interval.time);
}

/*
 * minimal interval time
 */
void menu_select_interval_min(int menu_num)
{
  strcpy_P(buffer, menu_interval[menu_num].item);
  settings.interval.min = menu_unsigned_long(buffer, settings.interval.min,
					     BRAMPUINO_MIN_INTERVAL,
					     BRAMPUINO_MAX_INTERVAL);
  DEBUG_PRINT_PSTR("set exp min to:");
  DEBUG_PRINTLN(settings.interval.min);
}


/*
 * maximal interval time
 */
void menu_select_interval_max(int menu_num)
{
  strcpy_P(buffer, menu_interval[menu_num].item);
  settings.interval.max = menu_unsigned_long(buffer, settings.interval.max,
					     BRAMPUINO_MIN_INTERVAL,
					     BRAMPUINO_MAX_INTERVAL);
  DEBUG_PRINT_PSTR("set exp max to:");
  DEBUG_PRINTLN(settings.interval.max);
}


/*
 * #exposures
 */

/**
 *
 */
void menu_select_max_exposures(int menu_num)
{
  strcpy_P(buffer, menu_settings[menu_num].item);
  settings.max_exposures = menu_unsigned_long(buffer, settings.max_exposures);
  DEBUG_PRINT_PSTR("set max # exposures to ");
  DEBUG_PRINTLN(settings.max_exposures);
}


/*
 * number of maximal exposures
 */
void menu_select_move_focus_step(int menu_num)
{
  strcpy_P(buffer, menu_settings[menu_num].item);
  settings.move_focus = menu_signed_long(buffer, settings.move_focus);
  DEBUG_PRINT_PSTR("set move focus step to ");
  DEBUG_PRINTLN(settings.move_focus);
}


/*
 * start
 */

/*
 * start was selected
 */
void menu_select_start(int color_line)
{
  DEBUG_PRINTLN_PSTR("running intervalometer");
  lcd.clear();
  lcd.setCursor(0, 0);
  start_interval_delay();
}


/*
 * lcd
 */

/*
 * switch to a color selected in menu
 */
void menu_lcd_select_color(int color_line)
{
  // select color + 1
  lcd.setBacklight(color_line + 1);
}


/*
 * real time clock
 */

/*
 * set one entry in real time clock
 */
void menu_select_clock_entry(int menu_num)
{
  // read old time
  int year, month, day;
  int hour, minute;
#ifdef HAVE_RTC
  DateTime now = RTC.now();
  year = now.year();
  month = now.month();
  day = now.day();
  hour = now.hour();
  minute = now.minute();
#else
  year = 2012;
  month = 07;
  day = 15;
  hour = 19;
  minute = 02;
#endif

  strcpy_P(buffer, menu_clock[menu_num].item);
  switch(menu_num) {
    // hour
  case 0:
    hour = menu_unsigned_long(buffer, hour, 0, 23);
    break;
    // minute
  case 1:
    minute = menu_unsigned_long(buffer, minute, 0, 59);
    break;
    // year
  case 2:
    year = menu_unsigned_long(buffer, year, 2012);
    break;
    // month
  case 3:
    month = menu_unsigned_long(buffer, month, 1, 12);
    break;
    // day
  case 4:
    day = menu_unsigned_long(buffer, day, 1, 31);
    break;
  default:
    break;
  }

  DEBUG_PRINT_PSTR("set hour to:"); DEBUG_PRINTLN(hour);
  DEBUG_PRINT_PSTR("set minute to:"); DEBUG_PRINTLN(minute);
  DEBUG_PRINT_PSTR("set year to:"); DEBUG_PRINTLN(year);
  DEBUG_PRINT_PSTR("set month to:"); DEBUG_PRINTLN(month);
  DEBUG_PRINT_PSTR("set day to:"); DEBUG_PRINTLN(day);
#ifdef HAVE_RTC
  RTC.adjust(DateTime(year, month, day, hour, minute, 0));
#endif
}



/*
 * version
 */

/**
 * version
 */
void menu_select_version(int unused)
{
  uint8_t c1, c2;

  lcd.clear();
  lcd.setCursor(0, 0);
  print_version();

  do {
    c1 = READ_BUTTONS();
    delay(DEBOUNCE_TIME);
    c2 = READ_BUTTONS();
  } while(!(c1 && (c1 == c2)));
}


#if 0
/*
 *
 */
void menu_select_ext_trigger(int menu_num)
{
  strcpy_P(buffer, menu_settings[menu_num].item);
  external_triggered = menu_bool(buffer, external_triggered);
  DEBUG_PRINT_PSTR("external triggered: ");
  DEBUG_PRINTLN(external_triggered);
}
#endif


/*
 * the loop
 *
 * run menu
 */
void menu_loop()
{
  // print menu
  static bool redraw_lcd = true;
  if(redraw_lcd) {
    int i, y;

    lcd.clear();
    lcd.setCursor(0, 0);

    DEBUG_PRINT_PSTR("menu from "); DEBUG_PRINTLN(menu_start_line);

    // redraw menu
    for(i = menu_start_line, y = 0;
	(i - menu_start_line < LCD_ROWS) && (NULL != menu_current[i].item);
	i++, y++) {
      strcpy_P(buffer, menu_current[i].item);
      DEBUG_PRINT_PSTR("print menu item "); DEBUG_PRINTLN(buffer);
      lcd.setCursor(0, y);
      if(i == menu_cursor_line) {
	DEBUG_PRINT_PSTR("active menu item "); DEBUG_PRINTLN(buffer);
	PRINT(">");
      }
      else {
	PRINT(" ");
      }
      PRINT(buffer);
    }
  }
  redraw_lcd = false;

  // read buttons
  uint8_t c1 = READ_BUTTONS();
  delay(DEBOUNCE_TIME);
  uint8_t c2 = READ_BUTTONS();

  if(c1 == c2) {
    // switch to menu
    switch(c1) {
    case BUTTON_LEFT:
      {
	const menu_entry_t *up_menu = NO_MENU;
#ifdef MENU_USE_LEFT_KEY
	up_menu = menu_current[menu_cursor_line].left;
#else
	up_menu = menu_toppop();
#endif
	if(NO_MENU != up_menu) {
	  menu_current = up_menu;
	  menu_start_line = 0;
	  menu_cursor_line = 0;
	  redraw_lcd = true;
	}
      }
      break;
    case BUTTON_RIGHT:
      {
	const menu_entry_t *down_menu = menu_current[menu_cursor_line].right;
	if(NO_MENU != down_menu) {
#ifndef MENU_USE_LEFT_KEY
	  menu_push(menu_current);
#endif
	  menu_current = down_menu;
	  menu_start_line = 0;
	  menu_cursor_line = 0;
	  redraw_lcd = true;
	}
	else {
	  if(NO_FUNC != menu_current[menu_cursor_line].func_right) {
	    (*menu_current[menu_cursor_line].func_right)(menu_cursor_line);
	    redraw_lcd = true;
	  }
	}
      }
      break;
    case BUTTON_UP:
      if(0 < menu_cursor_line) {
	--menu_cursor_line;
	redraw_lcd = true;
	if(menu_cursor_line < menu_start_line) {
	  menu_start_line = menu_cursor_line;
	}
      }
      else { // 0 == menu_cursor_line !!
	if(0 < menu_start_line) {
	  --menu_start_line;
	  redraw_lcd = true;
	}
      }
      break;
    case BUTTON_DOWN:
      if(NULL != menu_current[menu_cursor_line + 1].item) {
	++menu_cursor_line;
	if(LCD_ROWS <= menu_cursor_line - menu_start_line) {
	  ++menu_start_line;
	}
	redraw_lcd = true;
      }
      break;
    case BUTTON_SELECT:
      if(NO_FUNC != menu_current[menu_cursor_line].func_select) {
	(*menu_current[menu_cursor_line].func_select)(menu_cursor_line);
	redraw_lcd = true;
      }
      break;
      
    default:
      break;
    }
  }
  
}
