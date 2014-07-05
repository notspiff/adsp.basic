/*
 *      Copyright (C) 2005-2014 Team XBMC
 *      http://xbmc.org
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
 *  along with XBMC; see the file COPYING.  If not, see
 *  <http://www.gnu.org/licenses/>.
 *
 */

#include "xbmc/libXBMC_addon.h"
#include "xbmc/util/util.h"
#include "xbmc/util/StdString.h"

#include "DSPProcessMaster.h"
#include "Process_Stereo/DSPProcessStereo.h"

using namespace std;

CDSPProcessMaster::CDSPProcessMaster(AE_DSP_STREAM_ID streamId, unsigned int modeId, const char *modeName)
  : m_StreamId(streamId)
  , m_ModeId(modeId)
  , m_ModeName(modeName)
{
  m_ModeInfoStruct.iModeType = AE_DSP_MODE_TYPE_MASTER_PROCESS;
}

CDSPProcessMaster::~CDSPProcessMaster()
{
}

CDSPProcessMaster *CDSPProcessMaster::AllocateMaster(AE_DSP_STREAM_ID streamId, unsigned int modeId)
{
  CDSPProcessMaster *mode = NULL;
  switch (modeId)
  {
    case ID_MASTER_PROCESS_STEREO_DOWNMIX:
      mode = new CDSPProcess_StereoDownmix(streamId);
      break;
    default:
      break;
  }
  return mode;
}
