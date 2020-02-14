//
// Created by cc on 2019/12/27.
//

#include "ChatMainSoreModel.h"
#include "../entity/im_message.h"
#include <QDebug>

/** message  **/
ChatMainSoreModel::ChatMainSoreModel(QObject *parent)
        : QSortFilterProxyModel(parent)
{

}

//
bool ChatMainSoreModel::lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const {

    if (!source_left.isValid() || !source_right.isValid())
        return false;

    qint64 leftTime = source_left.data(EM_USER_MSG_TIME).toLongLong();
    qint64 rightTime = source_right.data(EM_USER_MSG_TIME).toLongLong();
    if(leftTime == rightTime)
    {
        int leftRole = source_left.data(EM_USER_MSG_TYPE).toInt();
        int rightRole = source_right.data(EM_USER_MSG_TYPE).toInt();
        if(QTalk::Entity::MessageTypeTime == leftRole || QTalk::Entity::MessageTypeTime == rightRole)
            return QTalk::Entity::MessageTypeTime == leftRole;
        else
            return false;
    }
    return leftTime < rightTime;
}
