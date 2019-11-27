//
// Created by QITMAC000260 on 2019/11/05.
//

#ifndef QTALK_V2_TITLEBAR_H
#define QTALK_V2_TITLEBAR_H

#include <QFrame>
#include <QToolButton>
#include "customui_global.h"

/**
* @description: TitleBar
* @author: cc
* @create: 2019-11-05 11:34
**/
class CUSTOMUISHARED_EXPORT TitleBar : public QFrame {
public:
    explicit TitleBar(const QString& title, QWidget* controlWgt = nullptr, QWidget* parent = nullptr);

public:
    QToolButton* getCloseBtn(){ return _pCloseBtn; };

private:
    QToolButton* _pCloseBtn;
};


#endif //QTALK_V2_TITLEBAR_H
