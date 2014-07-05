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
 *  along with XBMC; see the file COPYING.  If not, write to
 *  the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
 *  http://www.gnu.org/copyleft/gpl.html
 *
 */

#include <vector>
#include <string>
#include "client.h"
#include "xbmc/xbmc_adsp_dll.h"
#include "xbmc/util/util.h"
#include "xbmc/util/StdString.h"
#include "AudioDSPBasic.h"

using namespace std;
using namespace ADDON;

#ifdef TARGET_WINDOWS
#define snprintf _snprintf
#endif

int            m_iStreamsPresent  = 0;
bool           m_bCreated         = false;
ADDON_STATUS   m_CurStatus        = ADDON_STATUS_UNKNOWN;

/* User adjustable settings are saved here.
 * Default values are defined inside client.h
 * and exported to the other source files.
 */
std::string   g_strUserPath       = "";
std::string   g_strAddonPath      = "";


CHelper_libXBMC_addon *XBMC       = NULL;
CHelper_libXBMC_adsp  *ADSP       = NULL;
CHelper_libXBMC_gui   *GUI        = NULL;



/*!
 * Pointer array for active dsp processing classes, for this reason the
 * stream id is never more as AE_DSP_STREAM_MAX_STREAMS and can be used as pointer to this array.
 */
extern cDSPProcessorStream *g_usedDSPs[AE_DSP_STREAM_MAX_STREAMS];

extern "C" {

void ADDON_ReadSettings(void)
{

}

ADDON_STATUS ADDON_Create(void* hdl, void* props)
{
  if (!hdl || !props)
    return ADDON_STATUS_UNKNOWN;

  AE_DSP_PROPERTIES* adspprops = (AE_DSP_PROPERTIES*)props;

  XBMC = new CHelper_libXBMC_addon;
  if (!XBMC->RegisterMe(hdl))
  {
    SAFE_DELETE(XBMC);
    return ADDON_STATUS_PERMANENT_FAILURE;
  }

  GUI = new CHelper_libXBMC_gui;
  if (!GUI->RegisterMe(hdl))
  {
    SAFE_DELETE(GUI);
    SAFE_DELETE(XBMC);
    return ADDON_STATUS_PERMANENT_FAILURE;
  }

  ADSP = new CHelper_libXBMC_adsp;
  if (!ADSP->RegisterMe(hdl))
  {
    SAFE_DELETE(ADSP);
    SAFE_DELETE(GUI);
    SAFE_DELETE(XBMC);
    return ADDON_STATUS_PERMANENT_FAILURE;
  }

  XBMC->Log(LOG_DEBUG, "%s - Creating the Audio DSP demo add-on", __FUNCTION__);

  m_CurStatus     = ADDON_STATUS_UNKNOWN;
  g_strUserPath   = adspprops->strUserPath;
  g_strAddonPath  = adspprops->strAddonPath;

  ADDON_ReadSettings();

  if (!g_DSPProcessor.InitDSP())
    return m_CurStatus;

  m_CurStatus = ADDON_STATUS_OK;
  m_bCreated = true;
  m_iStreamsPresent = 0;
  return m_CurStatus;
}

ADDON_STATUS ADDON_GetStatus()
{
  return m_CurStatus;
}

void ADDON_Destroy()
{
  m_bCreated = false;
  m_iStreamsPresent = 0;

  g_DSPProcessor.DestroyDSP();

  SAFE_DELETE(ADSP);
  SAFE_DELETE(GUI);
  SAFE_DELETE(XBMC);

  m_CurStatus = ADDON_STATUS_UNKNOWN;
}

bool ADDON_HasSettings()
{
  return true;
}

unsigned int ADDON_GetSettings(ADDON_StructSetting ***sSet)
{
  (void) sSet; // Remove compiler warning
  return 0;
}

ADDON_STATUS ADDON_SetSetting(const char *settingName, const void *settingValue)
{
  return g_DSPProcessor.SetSetting(settingName, settingValue);
}

void ADDON_Stop()
{
}

void ADDON_FreeSettings()
{
}

void ADDON_Announce(const char *flag, const char *sender, const char *message, const void *data)
{
  (void) flag; (void) sender; (void) message; (void) data; // Remove compiler warning
}


/***********************************************************
 * Audio DSP Client AddOn specific public library functions
 ***********************************************************/

const char* GetAudioDSPAPIVersion(void)
{
  static const char *strApiVersion = XBMC_AE_DSP_API_VERSION;
  return strApiVersion;
}

const char* GetMininumAudioDSPAPIVersion(void)
{
  static const char *strMinApiVersion = XBMC_AE_DSP_MIN_API_VERSION;
  return strMinApiVersion;
}

const char* GetGUIAPIVersion(void)
{
  static const char *strGuiApiVersion = XBMC_GUI_API_VERSION;
  return strGuiApiVersion;
}

const char* GetMininumGUIAPIVersion(void)
{
  static const char *strMinGuiApiVersion = XBMC_GUI_MIN_API_VERSION;
  return strMinGuiApiVersion;
}

AE_DSP_ERROR GetAddonCapabilities(AE_DSP_ADDON_CAPABILITIES* pCapabilities)
{
  pCapabilities->bSupportsInputProcess    = g_DSPProcessor.SupportsInputProcess();
  pCapabilities->bSupportsInputResample   = g_DSPProcessor.SupportsInputResample();
  pCapabilities->bSupportsPreProcess      = g_DSPProcessor.SupportsPreProcess();
  pCapabilities->bSupportsMasterProcess   = g_DSPProcessor.SupportsMasterProcess();
  pCapabilities->bSupportsPostProcess     = g_DSPProcessor.SupportsPostProcess();
  pCapabilities->bSupportsOutputResample  = g_DSPProcessor.SupportsOutputResample();

  return AE_DSP_ERROR_NO_ERROR;
}

const char *GetDSPName(void)
{
  static const char *strBackendName = "Alwin Esch's demo Audio DSP add-on ;)";
  return strBackendName;
}

const char *GetDSPVersion(void)
{
  static CStdString strDSPVersion = "0.1";
  return strDSPVersion.c_str();
}

const char *GetConnectionString(void)
{
  static const char *emptyStr = "";
  return emptyStr;
}

AE_DSP_ERROR CallMenuHook(const AE_DSP_MENUHOOK &menuhook, const AE_DSP_MENUHOOK_DATA &item)
{
  return g_DSPProcessor.CallMenuHook(menuhook, item);
}


/*!
 * Control function for start and stop of dsp processing.
 */

AE_DSP_ERROR StreamCreate(const AE_DSP_SETTINGS *addonSettings, const AE_DSP_STREAM_PROPERTIES* pProperties)
{
  return g_DSPProcessor.StreamCreate(addonSettings, pProperties);
}

AE_DSP_ERROR StreamDestroy(AE_DSP_STREAM_ID id)
{
  return g_DSPProcessor.StreamDestroy(id);
}

AE_DSP_ERROR StreamInitialize(const AE_DSP_SETTINGS *settings)
{
  AE_DSP_ERROR err = AE_DSP_ERROR_UNKNOWN;

  if (g_usedDSPs[settings->iStreamID])
    err = g_usedDSPs[settings->iStreamID]->StreamInitialize(settings);

  return err;
}

AE_DSP_ERROR StreamIsModeSupported(AE_DSP_STREAM_ID id, AE_DSP_MODE_TYPE type, unsigned int mode_id, int unique_db_mode_id)
{
  AE_DSP_ERROR err = AE_DSP_ERROR_UNKNOWN;

  if (type == AE_DSP_MODE_TYPE_INPUT_RESAMPLE && mode_id == ID_POST_PROCESS_INPUT_RESAMPLER)
    return AE_DSP_ERROR_NO_ERROR;

  if (g_usedDSPs[id])
    err = g_usedDSPs[id]->StreamIsModeSupported(type, mode_id, unique_db_mode_id);

  return err;
}


/*!
 * Input processing related functions
 */

bool InputProcess(AE_DSP_STREAM_ID id, const float **array_in, unsigned int samples)
{
  return g_usedDSPs[id]->InputProcess(array_in, samples);
}


/*!
 * Resampling related functions before master processing.
 * only one dsp addon is allowed to do this
 */

unsigned int InputResampleProcessNeededSamplesize(AE_DSP_STREAM_ID id)
{
  return g_usedDSPs[id]->InputResampleProcessNeededSamplesize();
}

int InputResampleSampleRate(AE_DSP_STREAM_ID id)
{
  return g_usedDSPs[id]->InputResampleSampleRate();
}

float InputResampleGetDelay(AE_DSP_STREAM_ID id)
{
  return g_usedDSPs[id]->InputResampleGetDelay();
}

unsigned int InputResampleProcess(AE_DSP_STREAM_ID id, float **array_in, float **array_out, unsigned int samples)
{
  return g_usedDSPs[id]->InputResampleProcess(array_in,  array_out, samples);
}


/*!
 * Pre processing related functions
 * all enabled addons allowed todo this
 */

unsigned int PreProcessNeededSamplesize(AE_DSP_STREAM_ID id, unsigned int mode_id)
{
  return g_usedDSPs[id]->PreProcessNeededSamplesize(mode_id);
}

float PreProcessGetDelay(AE_DSP_STREAM_ID id, unsigned int mode_id)
{
  return g_usedDSPs[id]->PreProcessGetDelay(mode_id);
}

unsigned int PreProcess(AE_DSP_STREAM_ID id, unsigned int mode_id, float **array_in, float **array_out, unsigned int samples)
{
  return g_usedDSPs[id]->PreProcess(mode_id, array_in,  array_out, samples);
}

/*!
 * Master processing functions
 * only one during playback selectable dsp addon is allowed to do this
 */

AE_DSP_ERROR MasterProcessSetMode(AE_DSP_STREAM_ID id, AE_DSP_STREAMTYPE type, unsigned int client_mode_id, int unique_db_mode_id)
{
  return g_usedDSPs[id]->MasterProcessSetMode(type, client_mode_id, unique_db_mode_id);
}

unsigned int MasterProcessNeededSamplesize(AE_DSP_STREAM_ID id)
{
  return g_usedDSPs[id]->MasterProcessNeededSamplesize();
}

float MasterProcessGetDelay(AE_DSP_STREAM_ID id)
{
  return g_usedDSPs[id]->MasterProcessGetDelay();
}

unsigned int MasterProcess(AE_DSP_STREAM_ID id, float **array_in, float **array_out, unsigned int samples)
{
  return g_usedDSPs[id]->MasterProcess(array_in, array_out, samples);
}

int MasterProcessGetOutChannels(AE_DSP_STREAM_ID id, unsigned long &out_channel_present_flags)
{
  return g_usedDSPs[id]->MasterProcessGetOutChannels(out_channel_present_flags);
}

const char *MasterProcessGetStreamInfoString(AE_DSP_STREAM_ID id)
{
  return g_usedDSPs[id]->MasterProcessGetStreamInfoString();
}


/*!
 * Post processing related functions
 * all enabled addons allowed todo this
 */

unsigned int PostProcessNeededSamplesize(AE_DSP_STREAM_ID id, unsigned int mode_id)
{
  return g_usedDSPs[id]->PostProcessNeededSamplesize(mode_id);
}

float PostProcessGetDelay(AE_DSP_STREAM_ID id, unsigned int mode_id)
{
  return g_usedDSPs[id]->PostProcessGetDelay(mode_id);
}

unsigned int PostProcess(AE_DSP_STREAM_ID id, unsigned int mode_id, float **array_in, float **array_out, unsigned int samples)
{
  return g_usedDSPs[id]->PostProcess(mode_id, array_in, array_out, samples);
}


/*!
 * Resampling related functions after final processing.
 * only one dsp addon is allowed to do this
 */

unsigned int OutputResampleProcessNeededSamplesize(AE_DSP_STREAM_ID id)
{
  return g_usedDSPs[id]->OutputResampleProcessNeededSamplesize();
}

int OutputResampleSampleRate(AE_DSP_STREAM_ID id)
{
  return g_usedDSPs[id]->OutputResampleSampleRate();
}

float OutputResampleGetDelay(AE_DSP_STREAM_ID id)
{
  return g_usedDSPs[id]->OutputResampleGetDelay();
}

unsigned int OutputResampleProcess(AE_DSP_STREAM_ID id, float **array_in, float **array_out, unsigned int samples)
{
  return g_usedDSPs[id]->OutputResampleProcess(array_in,  array_out, samples);
}

}
