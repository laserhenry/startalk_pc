//
// Created by cc on 2019-02-25.
//

#ifndef QTALK_V2_SYSTEMDEFINE_H
#define QTALK_V2_SYSTEMDEFINE_H

#include <string>
#include <sstream>
#include <iostream>
#include "platform_global.h"

#define DB_VERSION 100012

#if defined(_STARTALK)
#define GLOBAL_INTERNAL_VERSION 200001
#define GLOBAL_VERSION "build-2.0.001"
#elif defined(_QCHAT)
#define GLOBAL_INTERNAL_VERSION  200010
#define GLOBAL_VERSION  "2.0.010W"
#else
#define GLOBAL_INTERNAL_VERSION 19009087
#define GLOBAL_VERSION "build-1.9.987"
#endif // !QCHAT

#ifndef _WINDOWS
#define APPLICATION_VERSION "2.0 BETA "  GLOBAL_VERSION
#else
#ifdef PLATFORM_WIN32
#define APPLICATION_VERSION "2.0 BETA Windows(x86) " GLOBAL_VERSION
#else
#define APPLICATION_VERSION "2.0 BETA Windows(x64) " GLOBAL_VERSION
#endif // PLATFORM_WNI32
#endif // _WINDOWS

time_t PLATFORMSHARED_EXPORT build_time();

#endif //QTALK_V2_SYSTEMDEFINE_H
