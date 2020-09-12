//
// Created by cc on 2019/10/15.
//

#ifndef QTALK_V2_DAOINTERFACE_H
#define QTALK_V2_DAOINTERFACE_H

#include <utility>

#include "sqlite/database.h"

class DaoInterface
{
public:
    explicit DaoInterface(qtalk::sqlite::database *sqlDb, std::string dbName)
        : _pSqlDb(sqlDb), _dbName(std::move(dbName)) {
    }

    virtual bool creatTable() = 0;

    bool clearData()
    {
        if(!_pSqlDb) return false;

        std::string sql = "DELETE FROM " + _dbName;
        qtalk::sqlite::statement query(*_pSqlDb, sql);
        return query.executeStep();
    }

protected:
    qtalk::sqlite::database *_pSqlDb = nullptr;
    std::string _dbName;
};

#endif //QTALK_V2_DAOINTERFACE_H
