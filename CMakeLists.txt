project(adsp.basic)

cmake_minimum_required(VERSION 2.6)

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR})

enable_language(CXX)

find_package(xbmc REQUIRED)
find_package(xbmcplatform REQUIRED)
find_package(SampleRate REQUIRED)
find_package(FFTW3 REQUIRED)

include_directories(${xbmcplatform_INCLUDE_DIRS}
                    ${XBMC_INCLUDE_DIR}
                    ${FFTW3_INCLUDE_DIRS}
                    ${SAMPLERATE_INCLUDE_DIRS})

set(BASIC_SOURCES src/AudioDSPBasic.cpp
                  src/GUIDialogSpeakerDistance.cpp
                  src/client.cpp
                  src/Process_Stereo/DSPProcessStereo.cpp
                  src/GUIDialogSpeakerGain.cpp
                  src/DSPProcessMaster.cpp
                  src/AudioDSPSettings.cpp
                  src/filter/high_shelf.cpp
                  src/filter/delay.cpp
                  src/filter/complex.cpp
                  src/filter/filter.cpp
                  src/filter/mkfilter.cpp
                  src/AudioDSPSoundTest.cpp)

set(DEPLIBS ${xbmcplatform_LIBRARIES}
            ${FFTW3_LIBRARIES}
            ${SAMPLERATE_LIBRARIES})

build_addon(adsp.basic BASIC DEPLIBS)

include(CPack)
