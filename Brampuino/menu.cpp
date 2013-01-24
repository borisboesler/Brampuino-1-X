
    /* this file is generated - do not edit */
#include "config.h"
#include "debug.h"
#include "menuitem.h"

    
extern const menu_entry_t menu_top[3+1];
    
extern const menu_entry_t menu_settings[4+1];
    
extern const menu_entry_t menu_exposure[6+1];
    
extern const menu_entry_t menu_exposure_num[2+1];
    
extern const menu_entry_t menu_ramping[5+1];
    
extern const menu_entry_t menu_iso[3+1];
    
extern const menu_entry_t menu_interval[3+1];
    
prog_char menu_label_settings[] PROGMEM = "Settings";
    
prog_char menu_label_startdelay[] PROGMEM = "Start Delay";
    
prog_char menu_label_exposure[] PROGMEM = "Exposure";
    
prog_char menu_label_exptime[] PROGMEM = "Exp.Time";
    
prog_char menu_label_expmin[] PROGMEM = "Exp.Min";
    
prog_char menu_label_expmax[] PROGMEM = "Exp.Max";
    
prog_char menu_label_expoffset[] PROGMEM = "Exp.Offset";
    
prog_char menu_label_exposure_num[] PROGMEM = "Num. of Frames";
    
prog_char menu_label_maxexp[] PROGMEM = "Max. Frames";
    
prog_char menu_label_leadin[] PROGMEM = "Lead In";
    
prog_char menu_label_ramping[] PROGMEM = "Ramping";
    
prog_char menu_label_expev[] PROGMEM = "Exp.EV.Change";
    
prog_char menu_label_expfps[] PROGMEM = "Exp.FPS";
    
prog_char menu_label_rampcalctime[] PROGMEM = "Calc FPS";
    
prog_char menu_label_rampfixedtime[] PROGMEM = "Ramp Time";
    
prog_char menu_label_iso[] PROGMEM = "ISO";
    
prog_char menu_label_isoauto[] PROGMEM = "ISO Ramp";
    
prog_char menu_label_isomin[] PROGMEM = "ISO Min";
    
prog_char menu_label_isomax[] PROGMEM = "ISO Max";
    
prog_char menu_label_interval[] PROGMEM = "Interval";
    
prog_char menu_label_intervaltime[] PROGMEM = "Interval Time";
    
prog_char menu_label_intervalmin[] PROGMEM = "Intv.Min";
    
prog_char menu_label_intervalmax[] PROGMEM = "Intv.Max";
    
prog_char menu_label_movefocus[] PROGMEM = "Move Focus Steps";
    
prog_char menu_label_start[] PROGMEM = "Start";
    
prog_char menu_label_version[] PROGMEM = "Version";
    
const menu_entry_t menu_top[3+1] = {
    
      { menu_label_settings,

      
#ifdef MENU_USE_LEFT_KEY
      NO_MENU,
#endif

      
	  menu_settings,

      
	  NO_FUNC
	,
      NO_FUNC },
    
      { menu_label_start,

      
#ifdef MENU_USE_LEFT_KEY
      NO_MENU,
#endif

      NO_MENU,

      menu_select_start,
      menu_select_start },
    
      { menu_label_version,

      
#ifdef MENU_USE_LEFT_KEY
      NO_MENU,
#endif

      NO_MENU,

      menu_select_version,
      menu_select_version },
    
    { NULL,
#ifdef MENU_USE_LEFT_KEY
    NO_MENU,
#endif
    NO_MENU, NO_FUNC,NO_FUNC }
};

    
const menu_entry_t menu_settings[4+1] = {
    
      { menu_label_startdelay,

      
#ifdef MENU_USE_LEFT_KEY
      
	  menu_top,
#endif

      NO_MENU,

      menu_select_delay_time,
      menu_select_delay_time },
    
      { menu_label_exposure,

      
#ifdef MENU_USE_LEFT_KEY
      
	  menu_top,
#endif

      
	  menu_exposure,

      
	  NO_FUNC
	,
      NO_FUNC },
    
      { menu_label_interval,

      
#ifdef MENU_USE_LEFT_KEY
      
	  menu_top,
#endif

      
	  menu_interval,

      
	  NO_FUNC
	,
      NO_FUNC },
    
      { menu_label_movefocus,

      
#ifdef MENU_USE_LEFT_KEY
      
	  menu_top,
#endif

      NO_MENU,

      menu_select_move_focus_step,
      menu_select_move_focus_step },
    
    { NULL,
#ifdef MENU_USE_LEFT_KEY
    NO_MENU,
#endif
    NO_MENU, NO_FUNC,NO_FUNC }
};

    
const menu_entry_t menu_exposure[6+1] = {
    
      { menu_label_exptime,

      
#ifdef MENU_USE_LEFT_KEY
      
	  menu_settings,
#endif

      NO_MENU,

      menu_select_exp_time,
      menu_select_exp_time },
    
      { menu_label_expmin,

      
#ifdef MENU_USE_LEFT_KEY
      
	  menu_settings,
#endif

      NO_MENU,

      menu_select_exp_min,
      menu_select_exp_min },
    
      { menu_label_expmax,

      
#ifdef MENU_USE_LEFT_KEY
      
	  menu_settings,
#endif

      NO_MENU,

      menu_select_exp_max,
      menu_select_exp_max },
    
      { menu_label_expoffset,

      
#ifdef MENU_USE_LEFT_KEY
      
	  menu_settings,
#endif

      NO_MENU,

      menu_select_exp_offset,
      menu_select_exp_offset },
    
      { menu_label_exposure_num,

      
#ifdef MENU_USE_LEFT_KEY
      
	  menu_settings,
#endif

      
	  menu_exposure_num,

      
	  NO_FUNC
	,
      NO_FUNC },
    
      { menu_label_ramping,

      
#ifdef MENU_USE_LEFT_KEY
      
	  menu_settings,
#endif

      
	  menu_ramping,

      
	  NO_FUNC
	,
      NO_FUNC },
    
    { NULL,
#ifdef MENU_USE_LEFT_KEY
    NO_MENU,
#endif
    NO_MENU, NO_FUNC,NO_FUNC }
};

    
const menu_entry_t menu_exposure_num[2+1] = {
    
      { menu_label_maxexp,

      
#ifdef MENU_USE_LEFT_KEY
      
	  menu_exposure,
#endif

      NO_MENU,

      menu_select_max_exposures,
      menu_select_max_exposures },
    
      { menu_label_leadin,

      
#ifdef MENU_USE_LEFT_KEY
      
	  menu_exposure,
#endif

      NO_MENU,

      menu_select_exp_lead_in,
      menu_select_exp_lead_in },
    
    { NULL,
#ifdef MENU_USE_LEFT_KEY
    NO_MENU,
#endif
    NO_MENU, NO_FUNC,NO_FUNC }
};

    
const menu_entry_t menu_ramping[5+1] = {
    
      { menu_label_expev,

      
#ifdef MENU_USE_LEFT_KEY
      
	  menu_exposure,
#endif

      NO_MENU,

      menu_select_exp_ev_change,
      menu_select_exp_ev_change },
    
      { menu_label_expfps,

      
#ifdef MENU_USE_LEFT_KEY
      
	  menu_exposure,
#endif

      NO_MENU,

      menu_select_exp_fps,
      menu_select_exp_fps },
    
      { menu_label_rampcalctime,

      
#ifdef MENU_USE_LEFT_KEY
      
	  menu_exposure,
#endif

      NO_MENU,

      menu_select_ramp_calc_time,
      menu_select_ramp_calc_time },
    
      { menu_label_rampfixedtime,

      
#ifdef MENU_USE_LEFT_KEY
      
	  menu_exposure,
#endif

      NO_MENU,

      menu_select_ramp_time,
      menu_select_ramp_time },
    
      { menu_label_iso,

      
#ifdef MENU_USE_LEFT_KEY
      
	  menu_exposure,
#endif

      
	  menu_iso,

      
	  NO_FUNC
	,
      NO_FUNC },
    
    { NULL,
#ifdef MENU_USE_LEFT_KEY
    NO_MENU,
#endif
    NO_MENU, NO_FUNC,NO_FUNC }
};

    
const menu_entry_t menu_iso[3+1] = {
    
      { menu_label_isoauto,

      
#ifdef MENU_USE_LEFT_KEY
      
	  menu_ramping,
#endif

      NO_MENU,

      menu_select_iso_ramp,
      menu_select_iso_ramp },
    
      { menu_label_isomin,

      
#ifdef MENU_USE_LEFT_KEY
      
	  menu_ramping,
#endif

      NO_MENU,

      menu_select_iso_min,
      menu_select_iso_min },
    
      { menu_label_isomax,

      
#ifdef MENU_USE_LEFT_KEY
      
	  menu_ramping,
#endif

      NO_MENU,

      menu_select_iso_max,
      menu_select_iso_max },
    
    { NULL,
#ifdef MENU_USE_LEFT_KEY
    NO_MENU,
#endif
    NO_MENU, NO_FUNC,NO_FUNC }
};

    
const menu_entry_t menu_interval[3+1] = {
    
      { menu_label_intervaltime,

      
#ifdef MENU_USE_LEFT_KEY
      
	  menu_settings,
#endif

      NO_MENU,

      menu_select_interval_time,
      menu_select_interval_time },
    
      { menu_label_intervalmin,

      
#ifdef MENU_USE_LEFT_KEY
      
	  menu_settings,
#endif

      NO_MENU,

      menu_select_interval_min,
      menu_select_interval_min },
    
      { menu_label_intervalmax,

      
#ifdef MENU_USE_LEFT_KEY
      
	  menu_settings,
#endif

      NO_MENU,

      menu_select_interval_max,
      menu_select_interval_max },
    
    { NULL,
#ifdef MENU_USE_LEFT_KEY
    NO_MENU,
#endif
    NO_MENU, NO_FUNC,NO_FUNC }
};

    