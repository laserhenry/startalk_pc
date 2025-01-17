//
// Created by 王超 on 2020/6/8.
//

#ifndef QTALK_V2_PROCESSEXCEPTDAO_H
#define QTALK_V2_PROCESSEXCEPTDAO_H

#include "DaoInterface.h"
    class ProcessExceptDao : public DaoInterface{
    public:
        explicit ProcessExceptDao(qtalk::sqlite::database *sqlDb);

        bool creatTable() override;

    public:
        void addExceptCpu(double cpu, long long time, const std::string& stack);
    };

#endif //QTALK_V2_PROCESSEXCEPTDAO_H
