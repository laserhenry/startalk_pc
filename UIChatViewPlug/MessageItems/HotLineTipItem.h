//
// Created by cc on 2019/08/29.
//

#ifndef QTALK_V2_HOTLINETIPITEM_H
#define QTALK_V2_HOTLINETIPITEM_H

#include <QFrame>
#include <NativeChatStruct.h>
#include "../../entity/im_message.h"

/**
* @description: HotlineTipItem
* @author: cc
* @create: 2019-08-29 16:51
**/
class HotLineTipItem : public QFrame{
public:
    explicit HotLineTipItem(const StNetMessageResult& info, QWidget* parent = nullptr);
    ~HotLineTipItem() override;

private:
    StNetMessageResult _msgInfo;
};


#endif //QTALK_V2_HOTLINETIPITEM_H
