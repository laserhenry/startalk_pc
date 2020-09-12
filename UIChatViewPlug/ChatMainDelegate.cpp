//
// Created by cc on 2019/12/27.
//

#include "ChatMainDelegate.h"
#include "../entity/im_message.h"
#include "NativeChatStruct.h"
#include "../UICom/qimage/qimage.h"
#include "../UICom/StyleDefine.h"
#include "ChatMainWgt.h"
#include <QPainter>
#include <QFile>
#include <MessageItems/TextMessItem.h>
#include <MessageItems/FileSendReceiveMessItem.h>
#include <MessageItems/CommonTrdInfoItem.h>
#include <MessageItems/CodeItem.h>
#include <MessageItems/AudioVideoItem.h>
#include <MessageItems/VideoMessageItem.h>
#include <MessageItems/TipMessageItem.h>
#include <MessageItems/VoiceMessageItem.h>
#include <MessageItems/NoteMessageItem.h>
#include <MessageItems/SystemMessageItem.h>
#include <MessageItems/NoticeMessageItem.h>
#include <MessageItems/MedalMind.h>
#include <MessageItems/MeetingRemindItem.h>
#include <MessageItems/HotLineAnswerItem.h>
#include <MessageItems/HotLineTipItem.h>
#include <MessageItems/EmojiMessItem.h>
#include <MessageItems/ImageMessItem.h>
#include <MessageItems/TextBrowser.h>


ChatMainDelegate::ChatMainDelegate(QMap<QString, MessageItemBase*> &mapItemWgt, QListView* parent)
        :QStyledItemDelegate(parent), _parentWgt(parent), _mapItems(mapItemWgt) {
}

//
QString getStrTime(qint64 time)
{
    QString content = "";
    QDateTime dateTime = QDateTime::currentDateTime();
    QInt64 thisDay = dateTime.toMSecsSinceEpoch() - dateTime.time().msecsSinceStartOfDay();
    if (time < thisDay) {
        content = QDateTime::fromMSecsSinceEpoch(time).toString("yyyy-MM-dd hh:mm:ss");
    } else {
        content = QDateTime::fromMSecsSinceEpoch(time).toString("hh:mm:ss");
    }
    return  content;
}


QWidget* ChatMainDelegate::creatWgt(const QModelIndex &index)
{
    auto msg_type = index.data(EM_USER_MSG_TYPE).toInt();
    auto showShare = index.data(EM_USER_SHOW_SHARE).toBool();

    StNetMessageResult info = index.data(EM_USER_INFO).value<StNetMessageResult>();
    QWidget* item = nullptr;
    switch (msg_type)
    {
        case QTalk::Entity::MessageTypeRevoke:
        case QTalk::Entity::MessageTypeGroupNotify:
        case QTalk::Entity::MessageTypeShock:
        {
            auto* pItem = new TipMessageItem();
            pItem->setText(info.body);
            return pItem;
        }
        case QTalk::Entity::MessageTypeTime:
        {
            auto time = index.data(EM_USER_MSG_TIME).toLongLong();
            auto* pItem = new TipMessageItem();
            pItem->setText(getStrTime(time));
            return pItem;
        }
        case QTalk::Entity::MessageTypeRobotTurnToUser:
        {
            item = new HotLineTipItem(info);
            break;
        }
        case QTalk::Entity::MessageTypeVoice:
        {
            item = new VoiceMessageItem(info);
            break;
        }
        case QTalk::Entity::MessageTypeProduct:
        case QTalk::Entity::MessageTypeNote:
        {
            item = new NoteMessageItem(info);
            break;
        }
        case QTalk::Entity::MessageTypeSystem:
        {
            item = new SystemMessageItem(info);
            break;
        }
        case QTalk::Entity::MessageTypeNotice:
        {
            item = new NoticeMessageItem(info);
            break;
        }
        case QTalk::Entity::MessageTypeMedalRemind:
        {
            item = new MedalMind(info);
            break;
        }
        case QTalk::Entity::MessageTypeMeetingRemind:
        {
            item = new MeetingRemindItem(info);
            break;
        }
        case QTalk::Entity::MessageTypeRobotAnswerList:
        {
            item = new HotLineAnswerItem(info);
            break;
        }
        case QTalk::Entity::MessageTypeFile:
        {
            item = new FileSendReceiveMessItem(info);
            break;
        }
        case QTalk::Entity::MessageTypeCommonTrdInfo:
        case QTalk::Entity::MessageTypeCommonTrdInfoV2:
        {
            item =  new CommonTrdInfoItem(info);
            break;
        }
        case QTalk::Entity::MessageTypeSourceCode:
        {
            item =  new CodeItem(info);
            break;
        }
        case QTalk::Entity::WebRTC_MsgType_VideoCall:
        case QTalk::Entity::WebRTC_MsgType_AudioCall:
        case QTalk::Entity::WebRTC_MsgType_Video:
        case QTalk::Entity::WebRTC_MsgType_Video_Group:
        {
            item =  new AudioVideoItem(info);
            break;
        }
        case QTalk::Entity::MessageTypeSmallVideo:
        {
            item =  new VideoMessageItem(info);
            break;
        }
        case QTalk::Entity::MessageTypePhoto:
        case QTalk::Entity::MessageTypeImageNew:
        {
            if(info.text_messages.size() == 1)
            {
                auto message = info.text_messages[0];
                if(message.type == StTextMessage::EM_EMOTICON)
                {
                    item = new EmojiMessItem(info, message.content, {message.imageWidth, message.imageHeight});
                    break;
                }
                else if(info.text_messages[0].type == StTextMessage::EM_IMAGE)
                {
                    item = new ImageMessItem(info, message.imageLink, message.content, {message.imageWidth, message.imageHeight});
                    break;
                }
                else {}
            }
        }
        case QTalk::Entity::MessageTypeText:
        case QTalk::Entity::MessageTypeGroupAt:
        case QTalk::Entity::MessageTypeRobotAnswer:
        default:
        {
            item =  new TextMessItem(info);
            break;
        }
    }

//    info_log("create message item {0} -> {1}", info.xmpp_id.toStdString(), info.msg_id.toStdString());
    auto *base = qobject_cast<MessageItemBase*>(item);
    if(!base) return item;
    //

    base->showShareCheckBtn(showShare);
    _mapItems.insert(info.msg_id, base);
    if (info.state) {
        if (info.state & 1)
            base->setReadState(info.read_flag | 1);
        else
            base->setReadState(info.read_flag);
    }
    else
    {

    }

    connect(base, SIGNAL(sgSelectItem(bool)), _parentWgt, SLOT(onItemCheckChanged(bool)));
    return base;
}


void ChatMainDelegate::dealWidget(const QModelIndex &index)
{
    QWidget* indexWgt = _parentWgt->indexWidget(index);
    if (indexWgt)
    {
        auto msg_type = index.data(EM_USER_MSG_TYPE).toInt();
        if(QTalk::Entity::MessageTypeTime == msg_type)
        {
            auto time = index.data(EM_USER_MSG_TIME).toLongLong();
            auto* timeItem = qobject_cast<TipMessageItem*>(indexWgt);
            if(timeItem) timeItem->setText(getStrTime(time));
        }

		indexWgt->setFixedWidth(_parentWgt->width());
    }
    else
    {
        auto *itemBase = creatWgt(index);
        _parentWgt->setIndexWidget(index, itemBase);
    }
}

void ChatMainDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    painter->save();
    painter->setRenderHint(QPainter::TextAntialiasing);

    QStyleOptionViewItem opt = option;
    initStyleOption(&opt, index);
    QRect rect = opt.rect;
    painter->fillRect(rect, QTalk::StyleDefine::instance().getMainWindowColor());

    auto* pDelegate = const_cast<ChatMainDelegate*>(this);
    if (pDelegate)
        pDelegate->dealWidget(index);
    painter->restore();
}

QSize getsize(const std::vector<StTextMessage>& messages, qreal width)
{
    width -= 20;
    auto* pBrowser = new TextBrowser;

    QTextCharFormat f;
    f.setFontLetterSpacingType(QFont::AbsoluteSpacing);
    f.setFontWordSpacing(0);
    f.setFontLetterSpacing(0);

    for (const auto &msg : messages) {
        switch (msg.type) {
            case StTextMessage::EM_TEXT:
                pBrowser->insertPlainText(msg.content);
                break;
            case StTextMessage::EM_IMAGE:
            case StTextMessage::EM_EMOTICON: {
                QString imagePath = msg.content;
                qreal imageWidth = msg.imageWidth;
                qreal imageHeight = msg.imageHeight;
                if (imagePath.isEmpty() || !QFile::exists(imagePath)) {
                    imagePath = ":/chatview/image1/defaultImage.png";
                    imageWidth = imageHeight = 80;
                }

                if (QPixmap(imagePath).isNull()) {
                    QString realPath = QTalk::qimage::getRealImagePath(imagePath);
                    if (QPixmap(realPath).isNull()) {
                        imagePath = ":/chatview/image1/defaultImage.png";
                        imageWidth = imageHeight = 80;
                    } else {
                        imagePath = realPath;
                    }
                }

                QTextImageFormat imageFormat;
                imageFormat.setWidth(imageWidth);
                imageFormat.setHeight(imageHeight);
                imageFormat.setName(imagePath);
                pBrowser->textCursor().insertImage(imageFormat);
                pBrowser->setCurrentCharFormat(f);
                //
                break;
            }
            case StTextMessage::EM_LINK: {
                QString content = msg.content;
                QTextCharFormat linkFormat = pBrowser->textCursor().charFormat();
                linkFormat.setForeground(QBrush(QTalk::StyleDefine::instance().getLinkUrl()));
                linkFormat.setAnchor(true);
                linkFormat.setAnchorHref(msg.content);
#if QT_DEPRECATED_SINCE(5, 13)
                linkFormat.setAnchorNames(QStringList() << msg.content);
#else
                linkFormat.setAnchorName(msg.content);
#endif
                pBrowser->textCursor().insertText(msg.content, linkFormat);

                pBrowser->setCurrentCharFormat(f);
                break;
            }
            case StTextMessage::EM_ATMSG: {
                QString content = QString(DEM_AT_HTML).arg(msg.content);
                pBrowser->insertHtml(content);
                pBrowser->setCurrentCharFormat(f);
                break;
            }
            default:
                break;
        }
    }
    
    pBrowser->document()->setTextWidth(width);
    QSizeF docSize = pBrowser->document()->size();
    QSize ret((int)width, (int)docSize.height() + 43);
    delete pBrowser;
    return ret;
}

//
QSize ChatMainDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const {

    StNetMessageResult info = index.data(EM_USER_INFO).value<StNetMessageResult>();
    auto msg_type = index.data(EM_USER_MSG_TYPE).toInt();
    switch (msg_type)
    {
        case QTalk::Entity::MessageTypeRevoke:
        case QTalk::Entity::MessageTypeGroupNotify:
        case QTalk::Entity::MessageTypeShock:
        case QTalk::Entity::MessageTypeRobotTurnToUser:
        case QTalk::Entity::MessageTypeTime:
            return {_parentWgt->width(), 40};
        case QTalk::Entity::MessageTypeFile:
//            return {_parentWgt->width(), 140};
        case QTalk::Entity::MessageTypeSourceCode:
//            return {_parentWgt->width(), 230};
        case QTalk::Entity::MessageTypeProduct:
        case QTalk::Entity::MessageTypeNote:
        case QTalk::Entity::MessageTypeSystem:
        case QTalk::Entity::MessageTypeNotice:
        case QTalk::Entity::MessageTypeRobotAnswerList:
        case QTalk::Entity::MessageTypeMedalRemind:
        case QTalk::Entity::MessageTypeMeetingRemind:
        case QTalk::Entity::MessageTypeCommonTrdInfo:
        case QTalk::Entity::MessageTypeCommonTrdInfoV2:
        case QTalk::Entity::WebRTC_MsgType_VideoCall:
        case QTalk::Entity::WebRTC_MsgType_AudioCall:
        case QTalk::Entity::WebRTC_MsgType_Video:
        case QTalk::Entity::WebRTC_MsgType_Video_Group:
        case QTalk::Entity::MessageTypeSmallVideo:
        case QTalk::Entity::MessageTypeVoice:
        case QTalk::Entity::MessageTypeText:
        case QTalk::Entity::MessageTypePhoto:
        case QTalk::Entity::MessageTypeGroupAt:
        case QTalk::Entity::MessageTypeImageNew:
        case QTalk::Entity::MessageTypeRobotAnswer:
        default:
            auto* indexWgt = qobject_cast<MessageItemBase*>(_parentWgt->indexWidget(index)) ;
            if (indexWgt)
            {
                return {_parentWgt->width(), indexWgt->itemWdtSize().height()};
            }
            break;
    }
    //

    switch (msg_type)
    {
        case QTalk::Entity::MessageTypeText:
        case QTalk::Entity::MessageTypeGroupAt:
        {
            auto size = getsize(info.text_messages, _parentWgt->width());
            return {_parentWgt->width(), size.height()};
        }
        case QTalk::Entity::MessageTypeSmallVideo:
        {
            return {_parentWgt->width(),
                    info.direction == QTalk::Entity::MessageDirectionSent ?
                    (int)info.video.height + 18 : (int)info.video.height + 30};
        }
        case QTalk::Entity::MessageTypeFile:
        {
            return {_parentWgt->width(), info.direction == QTalk::Entity::MessageDirectionSent ? 134 : 122};
        }
        case QTalk::Entity::MessageTypeSourceCode:
            return {_parentWgt->width(), 230};
        case QTalk::Entity::WebRTC_MsgType_VideoCall:
        case QTalk::Entity::WebRTC_MsgType_AudioCall:
        case QTalk::Entity::WebRTC_MsgType_Video:
        case QTalk::Entity::WebRTC_MsgType_Video_Group:
        {
            return {_parentWgt->width(), info.direction == QTalk::Entity::MessageDirectionSent ? 58 : 70};
        }
        case QTalk::Entity::MessageTypeVoice:
        {
            return {_parentWgt->width(), info.direction == QTalk::Entity::MessageDirectionSent ? 48 : 60};
        }
        default:
        {
            return {_parentWgt->width(), 250};
        }
    }
}
