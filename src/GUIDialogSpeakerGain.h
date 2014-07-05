#pragma once
/*
 *      Copyright (C) 2005-2014 Team XBMC
 *      http://www.xbmc.org
 *
 *  This Program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  This Program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with XBMC; see the file COPYING.  If not, write to
 *  the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
 *  http://www.gnu.org/copyleft/gpl.html
 *
 */

#include <string>

#include "AudioDSPSettings.h"

class CGUIDialogSpeakerGain : private CDSPSettings
{
public:
  CGUIDialogSpeakerGain(unsigned int streamId);
  virtual ~CGUIDialogSpeakerGain();

  bool Show();
  void Close();
  void DoModal();

  void ContinuesTestSwitchInfoCB(AE_DSP_CHANNEL channel);

private:
  bool OnClick(int controlId);
  bool OnFocus(int controlId);
  bool OnInit();
  bool OnAction(int actionId);

  static bool OnClickCB(GUIHANDLE cbhdl, int controlId);
  static bool OnFocusCB(GUIHANDLE cbhdl, int controlId);
  static bool OnInitCB(GUIHANDLE cbhdl);
  static bool OnActionCB(GUIHANDLE cbhdl, int actionId);

  void SetVolumeSpin(int id, AE_DSP_CHANNEL channel, bool present);
  void SetVolumeSpins();

  const unsigned int        m_StreamId;
  int                       m_GainTestSound;
  CAddonGUIWindow          *m_window;
  CAddonGUISpinControl     *m_spinSpeakerGainTest;
  CAddonGUIRadioButton     *m_radioSpeakerContinuesTest;
};

