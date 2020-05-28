//
// Created by cc on 2019/11/28.
//

#include "MessageSortModel.h"

/** message  **/
MessageSortModel::MessageSortModel(QObject *parent)
        : QSortFilterProxyModel(parent)
{

}

//
bool MessageSortModel::lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const {

    if (!source_left.isValid() || !source_right.isValid())
        return false;

    qint64 leftTime = source_left.data(SEARCH_USER_MSG_TIME).toLongLong();
    qint64 rightTime = source_right.data(SEARCH_USER_MSG_TIME).toLongLong();

    return leftTime <= rightTime;
}
