//
// Created by cc on 2019/11/29.
//

#include "SearchUserView.h"
#include <QScrollBar>
#include "SearchItemWgt.h"

/** SearchUserView **/
SearchUserView::SearchUserView(QWidget *parent)
        : QListView(parent) {
    this->setFrameShape(QFrame::NoFrame);
    this->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    this->verticalScrollBar()->setSingleStep(12);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
}

//
void SearchUserView::currentChanged(const QModelIndex &current, const QModelIndex &previous) {

    auto* current_wgt = qobject_cast<SearchItemBase*>(this->indexWidget(current));
    auto* previous_wgt = qobject_cast<SearchItemBase*>(this->indexWidget(previous));
    if(current_wgt)
        current_wgt->setDetailButtonVisible(true);
    if(previous_wgt)
        previous_wgt->setDetailButtonVisible(false);

    QListView::currentChanged(current, previous);
}
