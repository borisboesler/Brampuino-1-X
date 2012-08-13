/*
 * Constant definitions and interface of Brampuino 1-X
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

#ifndef __BRAMPUINO_H__
# define __BRAMPUINO_H__

# include "config.h"
# include "debug.h"

/**
 * \{
 */

/**
 * the default start delay in milli seconds before the first shot
 */
# define BRAMPUINO_DEFAULT_START_DELAY   (1L * 1000L)
/**
 * the default exposure time in milli seconds
 */
# define BRAMPUINO_DEFAULT_EXPOSURE_TIME      (1L * 1000L)

/**
 * the default exposure minimum time in milli seconds
 */
# define BRAMPUINO_DEFAULT_MIN_EXPOSURE_TIME      (100L)
/**
 * the default exposure maximum time in milli seconds
 */
# define BRAMPUINO_DEFAULT_MAX_EXPOSURE_TIME      (30L * 1000L)

/**
 * the minimal exposure time in milli seconds that we want to have
 */
# define BRAMPUINO_MIN_EXPOSURE_TIME      (0L)
/**
 * the maximal exposure time in milli seconds that we want to have
 */
# define BRAMPUINO_MAX_EXPOSURE_TIME      (60L * 1000L)


/**
 * the default exposure offset in milli seconds
 */
# define BRAMPUINO_DEFAULT_OFFSET      (0L)
/**
 * the minimal exposure offset in milli seconds that we want to have
 */
# define BRAMPUINO_MIN_OFFSET      (0L)
/**
 * the maximal exposure offset in milli seconds that we want to have
 */
# define BRAMPUINO_MAX_OFFSET      (10L * 1000L)


/**
 * the default ISO index
 */
# define BRAMPUINO_DEFAULT_ISO      (0)

/**
 * the default min ISO index
 */
# define BRAMPUINO_DEFAULT_MIN_ISO      (0)

/**
 * the default max ISO index
 */
# define BRAMPUINO_DEFAULT_MAX_ISO      (6)

/**
 * the default ISO auto ramp
 */
# define BRAMPUINO_DEFAULT_ISO_AUTO_RAMP      (false)


/**
 * the default increment in milli seconds from shot to shot
 */
# define BRAMPUINO_DEFAULT_INCREMENT      (100L)

/**
 * the default exposure value change after FPS shots
 */
# define BRAMPUINO_DEFAULT_EV_CHANGE      (0.0)
/**
 * default frames per second
 */
# define BRAMPUINO_DEFAULT_FPS      (25)



/**
 * the default interval in milli seconds between two shots
 */
# define BRAMPUINO_DEFAULT_INTERVAL      (5L * 1000L)

/**
 * the default interval in milli seconds between two shots
 */
# define BRAMPUINO_DEFAULT_MIN_INTERVAL      (2L * 1000L)
/**
 * the default interval in milli seconds between two shots
 */
# define BRAMPUINO_DEFAULT_MAX_INTERVAL      (30L * 1000L)

/**
 * the minimal interval in milli seconds between two shots that we want to have
 */
# define BRAMPUINO_MIN_INTERVAL      (100L)
/**
 * the maximal interval in milli seconds between two shots that we want to have
 */
# define BRAMPUINO_MAX_INTERVAL      (60L * 1000L)

/**
 * the default number of maximal number of exposures
 */
# define BRAMPUINO_DEFAULT_MAX_EXPOSURES (5L)

/**
 * the default move focus step
 */
# define BRAMPUINO_DEFAULT_MOVE_FOCUS_STEP (0)

/**
 * \}
 */

/**
 * address in EEPROM to store settings
 */
#define BRAMPUINO_EEPROM_SETTING_ADDRESS (0)

/**
 * magic to mark setting as saved EEPROM 
 */
#define BRAMPUINO_EEPROM_MAGIC \
  (  ((uint32_t)'B' << 24)     \
   | ((uint32_t)'R' << 16)     \
   | ((uint32_t)'1' << 8)      \
   |  (uint32_t)'X')


#define MIN(_x, _y) (((_x) < (_y)) ? (_x) : (_y))
#define MAX(_x, _y) (((_x) >= (_y)) ? (_x) : (_y))

/**
 * structure to collect all exposure settings
 */
typedef struct _settings_t {
  unsigned long start_delay;  /**< time till start */
  struct _exposure {
    unsigned long start_time; /**< start exposure time */
    unsigned long exp_time;   /**< current exposure time */
    unsigned long min;        /**< min. exposure time */
    unsigned long max;        /**< max. exposure time */
    unsigned long offset;     /**< exposure increment time */
    struct _u {
      struct _linear {
	long increment;       /**< exposure increment time */
      } linear;
      struct _exponential {
	double ev_change;     /**< exposure value change after FPS shots */
      } exponential;
    } u;
  } exposure;
  struct _iso {
    unsigned long iso_index;  /**< the used ISO setting */
    unsigned long min_index;  /**< the min ISO setting */
    unsigned long max_index;  /**< the max ISO setting */
    bool auto_ramp;           /**< automatic ISO ramping */
  } iso;
  struct _interval {
    unsigned long time;       /**< interval time */
    unsigned long min;        /**< interval time */
    unsigned long max;        /**< interval time */
  } interval;
  unsigned long max_exposures;       /**< number of exposures */
  unsigned fps;
  int move_focus;             /**< number of steps to focus */
  uint32_t settings_in_eeprom;/**< if == BRAMPUINO_EEPROM_MAGIC
				   -> settings in EEPROM */
} settings_t;

/**
 * states of operation
 */
typedef enum {
  BRAMPUINO_STATE_MENU,                /**< user is using the menu */
  BRAMPUINO_STATE_START_DELAY_WAITING, /**< wait to start the intervalometer */
  BRAMPUINO_STATE_INTERVAL_WAITING,    /**< waiting after a shot */
  BRAMPUINO_STATE_EXPOSING,            /**< we are shooting */
  BRAMPUINO_STATE_EXPOSING_MENU        /**< we are shooting and switched to menu */
} brampuino_states_t;

/* defined in Brampuino.cpp */
/**
 * a collection of all settings
 */
extern settings_t settings;

/**
 * iso values
 */
extern unsigned long iso_values[7];

/**
 * time when the start delay is over
 */
extern unsigned long delay_over_time;
/**
 * time when the interval is over
 */
extern unsigned long interval_over_time;
/**
 * time when the exposure time is over
 */
extern unsigned long exposure_over_time;
/**
 * the state of Brampuino
 */
//extern brampuino_states_t current_state;

/**
 * external trigger instead of internal control interval
 */
extern bool external_triggered;

extern void print_version(void);

extern void start_interval_delay(void);

/**
 * function called by Arduino environment to initialze everything
 */
extern void setup(void);

/**
 * function called by Arduino environment to check if something has to
 * be done
 */
extern void loop(void);

#endif
