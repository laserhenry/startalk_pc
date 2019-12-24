//
// Created by cc on 2019-02-25.
//

#ifndef QTALK_V2_SYSTEMDEFINE_H
#define QTALK_V2_SYSTEMDEFINE_H

#include <string>
#include <sstream>
#include <iostream>
#include "platform_global.h"

#define DB_VERSION 100011

#ifndef _QCHAT

#define GLOBAL_INTERNAL_VERSION 19009084
#define GLOBAL_VERSION "build-1.9.984"
#ifndef _WINDOWS

#define APPLICATION_VERSION "2.0 BETA "  GLOBAL_VERSION

#else

#ifdef PLATFORM_WIN32

#define APPLICATION_VERSION "2.0 BETA Windows(x86) " GLOBAL_VERSION

#else
#define APPLICATION_VERSION "2.0 BETA Windows(x64) " GLOBAL_VERSION

#endif // PLATFORM_WNI32
#endif // _WINDOWS
#else
#define APPLICATION_NAME  "QunarChatIM"
#define APPLICATION_VERSION  "2.0.008W"
#define GLOBAL_INTERNAL_VERSION  200008
#endif // !QCHAT

time_t PLATFORMSHARED_EXPORT build_time();

#endif //QTALK_V2_SYSTEMDEFINE_H
