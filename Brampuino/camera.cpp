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

#include "config.h"
#include "debug.h"

#include <inttypes.h>
#include <avr/pgmspace.h>

#include <avrpins.h>
#include <max3421e.h>
#include <usbhost.h>
#include <usb_ch9.h>
#include <Usb.h>
#include <usbhub.h>
#include <address.h>

#include <message.h>

#include <ptp.h>
#include <canoneos.h>

#include "camera.h"


/* **************************************** */

class CamStateHandlers : public PTPStateHandlers
{
  bool stateConnected;

public:
  CamStateHandlers() : stateConnected(false) {};

  virtual void OnDeviceDisconnectedState(PTP *ptp);
  virtual void OnDeviceInitializedState(PTP *ptp);
  virtual bool isConnected();
} CamStates;

void CamStateHandlers::OnDeviceDisconnectedState(PTP *ptp)
{
  if (stateConnected)
    {
      stateConnected = false;
      Notify(PSTR("Camera disconnected\r\n"));
    }
}

void CamStateHandlers::OnDeviceInitializedState(PTP *ptp)
{
  if (!stateConnected)
    stateConnected = true;
}

bool CamStateHandlers::isConnected()
{
  return(stateConnected);
}


/* **************************************** */

USB         Usb;
CanonEOS    Eos(&Usb, &CamStates);


/* **************************************** */

/*
 * set up camera
 */
void CanonCamera::setup()
{
  if (Usb.Init() == -1) {
    DEBUG_PRINTLN_PSTR("OSC did not start.");
  }
}

/*
 * in the loop
 */
void CanonCamera::loop()
{
  Usb.Task();
}

/*
 * is the camera connected
 */
bool CanonCamera::is_connected()
{
  return(CamStates.isConnected());
}

/* **************************************** */

/*
 * take a shot with current camera's settings
 */
void CanonCamera::capture()
{
  if(is_connected()) {
    uint16_t rc = Eos.Capture();
    if (rc != PTP_RC_OK) {
      ErrorMessage<uint16_t>("Error", rc);
    }
  }
}


/*
 * start a shot with current setting in bulb mode
 */
void CanonCamera::start_bulb()
{
  DEBUG_PRINTLN_PSTR("start bulb");
  if(is_connected()) {
    uint16_t rc = Eos.StartBulb();
    if (rc != PTP_RC_OK) {
      ErrorMessage<uint16_t>("Error", rc);
    }
  }
  else {
    DEBUG_PRINTLN_PSTR("Camera not connected");
  }
}


/*
 * stop a shot with current setting in bulb mode
 */
void CanonCamera::stop_bulb()
{
  DEBUG_PRINTLN_PSTR("stop bulb");
  if(is_connected()) {
    uint16_t rc = Eos.StopBulb();
    if (rc != PTP_RC_OK) {
      ErrorMessage<uint16_t>("Error", rc);
    }
  }
  else {
    DEBUG_PRINTLN_PSTR("Camera not connected");
  }
}


/*
 * Turns Live View on/off.
 * on	Turns Live View on if true, off otherwise.
 */
uint16_t CanonCamera::switch_liveview(bool on)
{
  if(is_connected()) {
    Eos.SwitchLiveView(on);
  }
}


/*
 * Moves focus on some EOS cameras capable of performing this operation.
 */
uint16_t CanonCamera::move_focus(uint16_t step)
{
  if(is_connected()) {
    Eos.MoveFocus(step);
  }
}


/*
 * set aperture
 */
void CanonCamera::set_aperture(float ap)
{
#if 0
  Eos.SetProperty(EOS_DPC_Aperture, (uint16_t)ap);
#endif
}


/*
 * set ISO
 */
void CanonCamera::set_iso(int iso)
{
  if(is_connected()) {
    Eos.SetProperty(EOS_DPC_Iso, (uint16_t)iso);
  }
}


// end from camera.cpp
