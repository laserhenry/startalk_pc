//
// Created by cc on 2019/11/29.
//

#ifndef QTALK_V2_SEARCHUSERVIEW_H
#define QTALK_V2_SEARCHUSERVIEW_H

#include <QListView>

/**
* @description: SearchUserView
* @author: cc
**/
class SearchUserView : public  QListView
{
public:
    explicit SearchUserView(QWidget* parent = nullptr);

protected:
    void currentChanged(const QModelIndex &current, const QModelIndex &previous) override;
};

#endif //QTALK_V2_SEARCHUSERVIEW_H
