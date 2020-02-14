//
// Created by cc on 2019/12/12.
//

#ifndef QTALK_V2_THREADHELPER_H
#define QTALK_V2_THREADHELPER_H

void setThreadName(const char* name)
{
#ifdef _MACOS
    pthread_setname_np(name);
#endif
}

#endif //QTALK_V2_THREADHELPER_H
