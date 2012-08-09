/*
 * drive camera
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

#ifndef __CAMERA_H__
# define __CAMERA_H__

# include "config.h"

class CanonCamera {
 public:
  void setup();
  void loop();

  /**
   * is the camera connected
   */
  bool is_connected();

  /**
   * take a shot with camera's current settings
   */
  void capture(void);

  /**
   * start a shot with current setting in bulb mode
   */
  void start_bulb(void);

  /**
   * stop a shot with current setting in bulb mode
   */
  void stop_bulb(void);

  /**
   * Turns Live View on/off.
   * on	Turns Live View on if true, off otherwise.
   */
  uint16_t switch_liveview(bool on);

  /**
   * Moves focus on some EOS cameras capable of performing this operation.
   */
  uint16_t move_focus(uint16_t step);

  /**
   * set aperture
   */
  void set_aperture(float ap);

  /**
   * set ISO
   */
  void set_iso(int iso);
};

#endif
