/* hey emacs this is a -*- c -*- file */
/*
 * Brampuino 1-X
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

#define BRAMPUINO_NAME "Brampuino 1-X"
#define BRAMPUINO_VERSION_MAJOR "0"

#ifdef HAVE_CAMERA
# define BRAMPUINO_VERSION_MINOR "2"
# define BRAMPUINO_VERSION_PATCHLEVEL "7a"
#else
# define BRAMPUINO_VERSION_MINOR "No"
# define BRAMPUINO_VERSION_PATCHLEVEL "Camera"
#endif

#define BRAMPUINO_AUTO_ISO_CHANGE settings.iso.auto_ramp

// include the library code:
#include <Arduino.h>
#include <stdint.h>
#include <avr/pgmspace.h>
#include <avr/eeprom.h>
#include "MsTimer2.h"
#include <math.h>


#if 0
// arduino SDK needs this to add include paths, no spaces allowed
#include <Wire.h>
#include <Adafruit_RGBLCDShield.h>
#include <Usb.h>
#include <canoneos.h>
//#include <SoftwareSerial.h>
//#include <Adafruit_MCP23017.h>
//#include <avrpins.h>
//#include <max3421e.h>
//#include <usbhost.h>
//#include <usb_ch9.h>
//#include <usbhub.h>
//#include <address.h>
//#include <message.h>
//#include <ptp.h>
#endif

#include "brampuino.h"
#include "lcd.h"
#include "button.h"
#include "menuitem.h"
#include "camera.h"

#ifdef AVR
# define ATTRIBUT_INTERRUPT __attribute__((interrupt))
#else
# define ATTRIBUT_INTERRUPT
#endif


/*
 * connection to hardware
 */
#define MX2ShutterPin  2
#define MX2FocusPin    3

/*
 * end of connection to hardware
 */

/**
 * current state
 */
volatile static brampuino_states_t current_state = BRAMPUINO_STATE_MENU;

/**
 * configured settings
 */
settings_t settings = { BRAMPUINO_DEFAULT_START_DELAY,
			{
			  BRAMPUINO_DEFAULT_EXPOSURE_TIME,
			  BRAMPUINO_DEFAULT_EXPOSURE_TIME,
			  BRAMPUINO_DEFAULT_MIN_EXPOSURE_TIME,
			  BRAMPUINO_DEFAULT_MAX_EXPOSURE_TIME,
			  BRAMPUINO_DEFAULT_OFFSET,
			  {
			    {
			      BRAMPUINO_DEFAULT_INCREMENT
			    },
			    {
			      BRAMPUINO_DEFAULT_EV_CHANGE
			    },
			  },
			  BRAMPUINO_DEFAULT_LEAD_IN,
			},
			{
			  BRAMPUINO_DEFAULT_ISO,
			  BRAMPUINO_DEFAULT_MIN_ISO,
			  BRAMPUINO_DEFAULT_MAX_ISO,
			  BRAMPUINO_DEFAULT_ISO_AUTO_RAMP
			},
			{
			  BRAMPUINO_DEFAULT_INTERVAL,
			  BRAMPUINO_DEFAULT_MIN_INTERVAL,
			  BRAMPUINO_DEFAULT_MAX_INTERVAL
			},
			BRAMPUINO_DEFAULT_MAX_EXPOSURES,
			BRAMPUINO_DEFAULT_FPS,
			BRAMPUINO_DEFAULT_MOVE_FOCUS_STEP,
			~BRAMPUINO_EEPROM_MAGIC
};

/**
 * number of shots taken
 */
volatile uint16_t exposure_count = 0;
volatile uint16_t adjusted_exposure_count = 0;
/**
 * stop shouting?
 */
volatile uint16_t exposure_stop = 0;

/**
 * current expsoure time and interval in ms, entries will be modified
 * whle Brampuino 1-X is running
 */
static settings_t current_settings;


#ifdef VERIFY_EXPOSURE_TIME
unsigned long exposure_start_ms = 0;
#endif

/**
 * the camera
 */
CanonCamera camera;

const unsigned long iso_values[ISO_RANGE_MAX_COUNT] = {
  /* on the 5DMkII:
   * - camera cant display ISO values 64 and 80 on top display
   * - transitions from ISO 50 -> 64 -> 80 -> 100 are horrible
   */
  50,    /*64,  80,*/
  100,  125, 160,
  200,  250, 320,
  400,  500, 640,
  800, 1000,1250,
  1600,2000,2500,
  3200,4000,5000,
  6400 };
const unsigned long iso_codes[ISO_RANGE_MAX_COUNT] =  {
  ISO_50,    /*64,  80,*/
  ISO_100,  ISO_125, ISO_160,
  ISO_200,  ISO_250, ISO_320,
  ISO_400,  ISO_500, ISO_640,
  ISO_800, ISO_1000,ISO_1250,
  ISO_1600,ISO_2000,ISO_2500,
  ISO_3200,ISO_4000,ISO_5000,
  ISO_6400
 };


/**
 * primitive interface to interact with LCD while shooting
 */
struct _lcd_action {
  bool darken;
  bool brighten;
} lcd_actions = { false, false } ;

/* **************************************** */
/* todo */
/*
 * external trigger instead of interval
 */
bool external_triggered = false;

/* **************************************** */

void print_version()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  PRINT(BRAMPUINO_NAME);
  lcd.setCursor(0, 1);
  PRINT("v" BRAMPUINO_VERSION_MAJOR
	"." BRAMPUINO_VERSION_MINOR
	"." BRAMPUINO_VERSION_PATCHLEVEL
	);
}


/* **************************************** */
bool do_move_focus = false;

/* **************************************** */

typedef enum {
  LOG_VOID = 0, /**< dont do anything */ 
  LOG_CLOSE,    /**< close file */ 
  LOG_WRITE     /**< write logging info */ 
} logging_action_t;

static volatile logging_action_t logging_action = LOG_VOID;

/* **************************************** */

/**
 * write logging information
 */
static void logging()
{
  // date and time
  // number of exposure
  DEBUG_PRINTLN(exposure_count);
  DEBUG_PRINTLN(adjusted_exposure_count);
  // current exposure time and interval
  DEBUG_PRINTLN(current_settings.exposure.exp_time + settings.exposure.offset);
  DEBUG_PRINTLN(settings.exposure.u.exponential.ev_change);

  // exposure times set by user
  DEBUG_PRINTLN(settings.exposure.start_time);
  DEBUG_PRINTLN(settings.exposure.min);
  DEBUG_PRINTLN(settings.exposure.max);

  // interval times set by user
  DEBUG_PRINTLN(settings.interval.time);
  DEBUG_PRINTLN(settings.interval.min);
  DEBUG_PRINTLN(settings.interval.max);

  // ISO
  DEBUG_PRINTLN(iso_values[current_settings.iso.iso_index]);

  // max exposure
  DEBUG_PRINTLN(settings.max_exposures);
  //write(file, current_settings.exposure);
}


/* **************************************** */
/*
 * new: bulb_time = start_time * pow(2.0, (ev_change * (shot-1)) / FPS)
 */
static
unsigned long new_bulb_time(unsigned long shot,
			    unsigned long start_time, unsigned long last_time)
{
  unsigned long bulb_time = 0;
  bool have_new_bulb_time = false;

  DEBUG_PRINT_PSTR("Shot: #");
  DEBUG_PRINTLN(shot);

  if(exposure_count <= settings.exposure.lead_in) {
    DEBUG_PRINT("Lead In with time: ");
    DEBUG_PRINTLN(last_time);
    adjusted_exposure_count = 0;
    return(last_time);
  }

  while(!have_new_bulb_time) {
    double dshot = shot - 1;
    double dfps = settings.fps;
    double exponent = (settings.exposure.u.exponential.ev_change * dshot) / dfps;
    double bulb_time_d = start_time * pow(2.0, exponent);
    // round
    bulb_time = bulb_time_d + 0.5;

    DEBUG_PRINT_PSTR("new bulb time:");
    DEBUG_PRINTLN(bulb_time);

    if(BRAMPUINO_AUTO_ISO_CHANGE) {
      if(bulb_time > settings.exposure.max) {
	DEBUG_PRINTLN_PSTR("RAMP ISO check max");
	// inc ISO
	if(current_settings.iso.iso_index + 1 <= settings.iso.max_index) {
	  unsigned long new_time;
	  new_time = (bulb_time * iso_values[current_settings.iso.iso_index])
	    / iso_values[current_settings.iso.iso_index + 1];
	  //Serial.println(new_time);
	  if(new_time >= settings.exposure.min) {
	    // increment ISO
	    ++current_settings.iso.iso_index;
	    // set new start time
	    DEBUG_PRINTLN_PSTR("RAMP ISO check exp inc");
	    current_settings.exposure.start_time = new_time;
	    bulb_time = new_time;
	    start_time = new_time;
	    adjusted_exposure_count = 1;
	    shot = 1;
	  }
	}
	else {
	  DEBUG_PRINTLN_PSTR("RAMP ISO check out of ISO");
	  have_new_bulb_time = true;
	}
      }
      else {
	if(bulb_time < settings.exposure.min) {
	  DEBUG_PRINTLN_PSTR("RAMP ISO check min");
	  // dec ISO
	  if((1 <= current_settings.iso.iso_index) // can decrement
	     && (current_settings.iso.iso_index - 1 >= settings.iso.min_index)) {
	    unsigned long new_time;
	    new_time = (bulb_time * iso_values[current_settings.iso.iso_index])
	      / iso_values[current_settings.iso.iso_index - 1];
	    if(new_time <= settings.exposure.max) {
	      // decrement ISO
	      --current_settings.iso.iso_index;
	      // set new start time
	      DEBUG_PRINTLN_PSTR("RAMP ISO check exp dec");
	      current_settings.exposure.start_time = new_time;
	      bulb_time = new_time;
	      start_time = new_time;
	      adjusted_exposure_count = 1;
	      shot = 1;
	    }
	  }
	  else {
	    DEBUG_PRINTLN_PSTR("RAMP ISO check out of ISO");
	    have_new_bulb_time = true;
	  }
	}
	else {
	  DEBUG_PRINTLN_PSTR("RAMP ISO check ok");
	  have_new_bulb_time = true;
	}
      }
    }
    else {
      DEBUG_PRINTLN_PSTR("No RAMP ISO");
      have_new_bulb_time = true;
    }
  }

  bulb_time = MIN(bulb_time, settings.exposure.max);
  bulb_time = MAX(bulb_time, settings.exposure.min);
  return(bulb_time);
}

/* **************************************** */
/* **************************************** */
/*
 * interrupt routines
 */

/*
 * stop interrupt
 */
void stop_interrupt()
{
  DEBUG_PRINTLN_PSTR("Stop interrupt");
  MsTimer2::stop();
  current_state = BRAMPUINO_STATE_MENU;
}


/*
 * stop an exposure
 */
void ATTRIBUT_INTERRUPT stop_exposure()
{
  // stop interrupt
  MsTimer2::stop();

  // close camera's shutter
  if(0 != current_settings.exposure.start_time) {
#ifdef HAVE_CAMERA
    camera.stop_bulb();
#endif
  }
  // move focus
  do_move_focus = true;

#ifdef VERIFY_EXPOSURE_TIME
  int exposure_time = millis() - exposure_start_ms;
  DEBUG_PRINT_PSTR("real exposure time:");
  DEBUG_PRINTLN(exposure_time);
#endif

  // modify intervall time
  // check wrap around
  if(settings.interval.time
     > settings.interval.time - current_settings.exposure.exp_time) {
    current_settings.interval.time
      = settings.interval.time - current_settings.exposure.exp_time;
    current_settings.interval.time
      = MIN(current_settings.interval.time, settings.interval.max);
    current_settings.interval.time
      = MAX(current_settings.interval.time, settings.interval.min);
  }
  else {
    current_settings.interval.time = settings.interval.min;
  }

  // if Live View is turned on on Canon 5D MkII
  if(2000 > current_settings.interval.time) {
    DEBUG_PRINTLN_PSTR("interval conflict");
  }

  // start new exposure and modify exposure time
  if((0 == exposure_stop)
     && ((0 == settings.max_exposures)
	 || (exposure_count < settings.max_exposures))) {
#ifdef DEBUG_IN_INTERRUPT
    DEBUG_PRINT_PSTR("Stop exposure, start interval:");
    DEBUG_PRINTLN(current_settings.interval.time);
#endif
    MsTimer2::set(current_settings.interval.time, start_exposure);
    MsTimer2::start();
  }
  else {
    // if stop was set by user then clear again
    exposure_stop = 0;
    current_state = BRAMPUINO_STATE_MENU;
#ifdef DEBUG_IN_INTERRUPT
    DEBUG_PRINTLN_PSTR("Stop. Made all shots");
#endif
    lcd_actions.brighten = true;
    logging_action = LOG_CLOSE;
  }
}


/*
 * start an exposure
 */
void ATTRIBUT_INTERRUPT start_exposure()
{
  MsTimer2::stop();

  ++exposure_count;
  ++adjusted_exposure_count;
  // increment exposure time
  current_settings.exposure.exp_time
    = new_bulb_time(adjusted_exposure_count,
		    current_settings.exposure.start_time,
		    current_settings.exposure.exp_time);

#ifdef DEBUG_IN_INTERRUPT
  DEBUG_PRINT_PSTR("Start exposure #"); DEBUG_PRINT(exposure_count);
  DEBUG_PRINT_PSTR(":"); DEBUG_PRINTLN(current_settings.exposure);
#endif

  // open shutter
#ifdef HAVE_CAMERA
  // if the start time is 0 then use capture to make a shot
  if(0 == current_settings.exposure.start_time) {
    camera.capture();
  }
  else {
    // if we are ISO ramping then set current ISO
    if(BRAMPUINO_AUTO_ISO_CHANGE) {
      camera.set_iso(iso_codes[current_settings.iso.iso_index]);
    }
    camera.start_bulb();
  }
#endif

#ifdef VERIFY_EXPOSURE_TIME
  exposure_start_ms = millis();
#endif
  // set interrupt to close shutter again
  unsigned long stop_time = 0;
  if(0 == current_settings.exposure.exp_time) {
    stop_time = current_settings.interval.time;
  }
  else {
    stop_time
      = current_settings.exposure.exp_time + current_settings.exposure.offset;
  }
  MsTimer2::set(stop_time, stop_exposure);
  MsTimer2::start();

  logging_action = LOG_WRITE;
}


/*
 * start interval with a delay
 */
void start_interval_delay()
{
  // store setings in EEPROM?
  if(menu_changed_settings) {
    DEBUG_PRINTLN_PSTR("write setting to EEPROM");
    // mark as valid
    settings.settings_in_eeprom = BRAMPUINO_EEPROM_MAGIC;
    settings_t *eeprom_settings = (settings_t *)BRAMPUINO_EEPROM_SETTING_ADDRESS;
    // write &settings -> eeprom_settings
    eeprom_write_block((const void*)&settings,
		       (void*)eeprom_settings,
		       sizeof(settings_t));
    // clear setting change
    menu_changed_settings = false;
  }

  if(BRAMPUINO_STATE_EXPOSING == current_state) {
    // continue shooting with new settings
    return;
  }
  if(BRAMPUINO_STATE_EXPOSING_MENU == current_state) {
    // continue shooting with new settings
    current_state = BRAMPUINO_STATE_EXPOSING;
    return;
  }

  DEBUG_PRINT_PSTR("Start interval delay:");
  DEBUG_PRINTLN(settings.start_delay);
  lcd_actions.darken = true;
  // init current settings
  current_settings = settings;

  // init exposure
  current_settings.exposure.exp_time = current_settings.exposure.start_time;

  // init count
  adjusted_exposure_count = exposure_count = 0;

  // if we are ISO ramping then init current ISO
  if(BRAMPUINO_AUTO_ISO_CHANGE) {
    // init ISO with ISO max if we ramp with a negative ev.fps value
    if(0.0 > current_settings.exposure.u.exponential.ev_change) {
      current_settings.iso.iso_index = current_settings.iso.max_index;
    }
    else {
      // init with minimum
      current_settings.iso.iso_index = current_settings.iso.min_index;
    }
  }

  // open file for logging
  DEBUG_PRINTLN_PSTR("open file");
  //openfile(year/month/day/time.csv);

  // start interrupt for first exposure
  if(0 < settings.start_delay) {
    MsTimer2::set(settings.start_delay, start_exposure);
    MsTimer2::start();
  }
  else {
    // start directly
    start_exposure();
  }

  current_state = BRAMPUINO_STATE_EXPOSING;
}


/* **************************************** */
/* **************************************** */

/**
 * set up Brampuino
 *
 * set up all sub-modules
 */
void setup(void)
{
  // init Debugging output
  DEBUG_GLOBAL_INIT;
  DEBUG_PRINTLN_PSTR("Starting ...");

  /*
   * set up all used modules
   */
  // set up lcd
  lcd_setup();
  // Print a message to the LCD.
  print_version();

  // set up buttons
  button_setup();

#ifdef HAVE_CAMERA
  // camera
  camera.setup();
#endif

  // Brampuino hardware
#if 0
  pinMode(MX2FocusPin, INPUT);
  pinMode(MX2ShutterPin, INPUT);
#endif

  // set up start state
  current_state = BRAMPUINO_STATE_MENU;

  // load setting from EEPROM
  {
    settings_t *eeprom_settings = (settings_t *)BRAMPUINO_EEPROM_SETTING_ADDRESS;
    uint32_t read_magic = eeprom_read_dword(&(eeprom_settings -> settings_in_eeprom));
    if(BRAMPUINO_EEPROM_MAGIC == read_magic) {
      DEBUG_PRINTLN_PSTR("load settings from EEPROM");
      // eeprom_settings -> setting
      eeprom_read_block((void*)&settings,
			(const void*)eeprom_settings,
			sizeof(settings_t));

      // print settings
      DEBUG_PRINTLN_PSTR("some current settings:");
      logging();
    }
    else {
      DEBUG_PRINTLN_PSTR("no settings in EEPROM");
      DEBUG_PRINTLN(read_magic, HEX);
    }
  }

  // leave message on screen
  delay(1000);
}


/**
 * the loop
 *
 * run menu
 */
void loop()
{
#ifdef HAVE_CAMERA
  camera.loop();
#endif
  unsigned long current_time = millis();

  // check interface to lcd
  if(lcd_actions.brighten) {
    lcd_actions.brighten = false;
#ifdef HAVE_ADAFRUIT_RGB_SHIELD
    lcd_set_color(GREEN);
#endif
  }
  if(lcd_actions.darken) {
    lcd_actions.darken = false;
#ifdef HAVE_ADAFRUIT_RGB_SHIELD
    lcd_set_color(RED);
#endif
  }

  // check focusing action
  if(do_move_focus) {
    do_move_focus = false;
    if(0 != settings.move_focus) {
#ifdef HAVE_CAMERA
      uint16_t movement = (0 < settings.move_focus)
	? (settings.move_focus) : (0x8000 | (-settings.move_focus)) ;
      camera.move_focus(movement);
      //Serial.println(movement, HEX);
      //delay(10);
#else
      DEBUG_PRINT_PSTR("move focus:");
      DEBUG_PRINTLN(settings.move_focus);
#endif
    }
  }

  // check logging action
  switch(logging_action) {
  case LOG_CLOSE:
    DEBUG_PRINTLN_PSTR("close file");
    // close(file);
    logging_action = LOG_VOID;
    break;
  case LOG_WRITE:
    DEBUG_PRINTLN_PSTR("write info to file");
    logging();
    logging_action = LOG_VOID;
    break;
  default:
    break;
  }

  // check state
  if((BRAMPUINO_STATE_MENU == current_state)
     || (BRAMPUINO_STATE_EXPOSING_MENU == current_state)) {
    menu_loop();
  }
  else if(BRAMPUINO_STATE_EXPOSING == current_state) {
    // print on screen info
    // shot number
    lcd.setCursor(0, 0);
    PRINT(exposure_count);
    PRINT("/");
    if(0 == settings.max_exposures) {
      PRINT("Inf");
    }
    else {
      PRINT(settings.max_exposures);
    }

    // ISO
    if(BRAMPUINO_AUTO_ISO_CHANGE) {
      lcd.setCursor(8, 0);
      PRINT(" ISO");
      PRINT(iso_values[current_settings.iso.iso_index]);
      PRINT(" ");
    }

    // exposure
    lcd.setCursor(0, 1);
    PRINT("B:");
    PRINT(current_settings.exposure.exp_time + current_settings.exposure.offset);

    // EV change
    PRINT("/EV:");
    PRINT(settings.exposure.u.exponential.ev_change);
    PRINT("  ");

    // cancel?
    uint8_t c1 = READ_BUTTONS();
    delay(40);
    uint8_t c2 = READ_BUTTONS();
    if(c1 && (c1 == c2)) {
      switch(c1) {

      case MENU_BUTTON_MAIN_CANCEL:
	// stop shutter
	// exposure_count = settings.max_exposures;
	exposure_stop = 1;
	current_state = BRAMPUINO_STATE_MENU;
	break;

      case MENU_BUTTON_MAIN_MENU:
	// menu
	current_state = BRAMPUINO_STATE_EXPOSING_MENU;
	// Start will continue
	menu_loop();
	break;

      case MENU_BUTTON_MAIN_DECREMENT:
	{
	  // set current exposure time as new start time
	  DEBUG_PRINTLN_PSTR("Manual dec exp ev change");
	  unsigned long bulb_time = current_settings.exposure.exp_time;
	  bulb_time = MIN(bulb_time, settings.exposure.max);
	  bulb_time = MAX(bulb_time, settings.exposure.min);
	  current_settings.exposure.start_time = bulb_time;
	  adjusted_exposure_count = 1;
	  // decrement EV change
	  settings.exposure.u.exponential.ev_change -= 0.1;
	}
	break;

      case MENU_BUTTON_MAIN_INCREMENT:
	{
	  // set current exposure time as new start time
	  DEBUG_PRINTLN_PSTR("Manual inc exp ev change");
	  unsigned long bulb_time = current_settings.exposure.exp_time;
	  bulb_time = MIN(bulb_time, settings.exposure.max);
	  bulb_time = MAX(bulb_time, settings.exposure.min);
	  current_settings.exposure.start_time = bulb_time;
	  adjusted_exposure_count = 1;
	  // increment EV change
	  settings.exposure.u.exponential.ev_change += 0.1;
	}
	break;
	
      case MENU_BUTTON_MAIN_CHANGE_ISO:
	if(BRAMPUINO_AUTO_ISO_CHANGE) {
	  // if ISO change is possible
	  if(current_settings.iso.iso_index + 1 <= settings.iso.max_index) {
	    DEBUG_PRINTLN_PSTR("Inc ISO by User");
	    unsigned long bulb_time = current_settings.exposure.exp_time;
	    unsigned long new_time
	      = (bulb_time * iso_values[current_settings.iso.iso_index])
	      / iso_values[current_settings.iso.iso_index + 1];
	    if(new_time > settings.exposure.min) {
	      // increment ISO
	      ++current_settings.iso.iso_index;
	      // set new start time
	      current_settings.exposure.start_time = new_time;
	      adjusted_exposure_count = 1;
	    }
	  }
	}
	break;

      default:
	break;
      }
    }
  }

}
