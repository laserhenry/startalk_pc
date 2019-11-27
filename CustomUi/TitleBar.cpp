//
// Created by QITMAC000260 on 2019/11/05.
//

#include "TitleBar.h"
#include <QHBoxLayout>
#include <QLabel>

TitleBar::TitleBar(const QString& title, QWidget* controlWgt, QWidget* parent)
    :QFrame(parent)
{
    this->setObjectName("TitleBar");

    auto * titleLay = new QHBoxLayout(this);
    titleLay->setMargin(10);
    _pCloseBtn = new QToolButton(this);
    auto *titleLbl = new QLabel(title, this);
    titleLbl->setObjectName("TitleBarLabel");
    titleLbl->setAlignment(Qt::AlignmentFlag::AlignCenter);

#ifdef _MACOS
    _pCloseBtn->setFixedSize(10, 10);
    titleLay->addWidget(_pCloseBtn);
    _pCloseBtn->setObjectName("gmCloseBtn");
    titleLay->addWidget(titleLbl);
    titleLay->addItem(new QSpacerItem(12, 12));
#else
    titleLay->addWidget(titleLbl);
    _pCloseBtn->setFixedSize(20, 20);
    titleLay->addItem(new QSpacerItem(10, 10, QSizePolicy::Expanding));
    _pCloseBtn->setObjectName("gwCloseBtn");
    titleLay->addWidget(_pCloseBtn);
#endif
    //
    if(nullptr != controlWgt)
        connect(_pCloseBtn, &QToolButton::clicked, [controlWgt](){controlWgt->setVisible(false);});
}