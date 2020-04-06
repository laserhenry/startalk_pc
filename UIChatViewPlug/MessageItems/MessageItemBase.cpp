#include "MessageItemBase.h"
#include <QLabel>
#include <QtCore/QEvent>
#include "ChatViewMainPanel.h"
#include <QDebug>
#include <QFileInfo>
#include <QTimer>
#include "../../CustomUi/HeadPhotoLab.h"
#include "../QtUtil/Entity/JID.h"
#include "../ChatMainWgt.h"
#include "../InputWgt.h"
#include "../../Platform/NavigationManager.h"
#include "../../Platform/Platform.h"
#include "../../CustomUi/QtMessageBox.h"
#include "TextMessItem.h"

extern ChatViewMainPanel* g_pMainPanel;
MessageItemBase::MessageItemBase(const StNetMessageResult &msgInfo, QWidget *parent) :
    QFrame(parent), _readSts(0)
{
	_msgInfo = msgInfo;
    _btnShareCheck = new QToolButton(this);
    _btnShareCheck->setCheckable(true);
    _btnShareCheck->setFixedSize(20, 20);
    _btnShareCheck->setObjectName("ShareCheck");
    connect(_btnShareCheck, &QToolButton::clicked, this, &MessageItemBase::sgSelectItem);

    if (QTalk::Enum::ChatType::GroupChat == _msgInfo.type
        && QTalk::Entity::MessageDirectionReceive == _msgInfo.direction ) {
        if (!_nameLab) {
            _nameLab = new QLabel(this);
        }
        _nameLab->setObjectName("chatitemnameLab");
        _nameLab->setFixedHeight(16);
        _nameLab->setTextFormat(Qt::PlainText);
        _nameLab->installEventFilter(this);
        _nameLab->setText(_msgInfo.user_name);
        // 勋章状态
        updateUserMedal();
    }

	QFileInfo fInfo(_msgInfo.user_head);
	if (_msgInfo.user_head.isEmpty() || !fInfo.exists() || fInfo.isDir())
	{
#ifdef _STARTALK
        _msgInfo.user_head = ":/QTalk/image1/StarTalk_defaultHead.png";
#else
        _msgInfo.user_head = ":/QTalk/image1/headPortrait.png";
#endif
	}

	if (NavigationManager::instance().isShowmsgstat() &&
	    _msgInfo.direction == QTalk::Entity::MessageDirectionSent &&
        (_msgInfo.type == QTalk::Enum::TwoPersonChat ||
        _msgInfo.type == QTalk::Enum::Consult ||
        _msgInfo.type == QTalk::Enum::ConsultServer))
	{
		_readSts = EM_SENDDING;
		_readStateLabel = new QLabel(this);
		_readStateLabel->setContentsMargins(0, 0, 0, 0);
		_readStateLabel->setObjectName("ReadStateLabel");
	}

	if(QTalk::Entity::MessageDirectionSent == _msgInfo.direction)
    {
        _resending = new HeadPhotoLab;
        _resending->setParent(this);
        _resending->setHead(":/chatview/image1/error.png", 8, false, false, true);
        _resending->installEventFilter(this);

        _sending = new HeadPhotoLab(":/QTalk/image1/loading.gif", 10, false, false, true, this);
        bool unSend = ( 0 == _msgInfo.state);
        bool isSending = g_pMainPanel->isSending(msgInfo.msg_id.toStdString());
        if(unSend)
        {
            _sending->setVisible(isSending);
            if(isSending)
                _sending->startMovie();
            _resending->setVisible(!isSending);
        }
        else
        {
            _sending->setVisible(false);
            _resending->setVisible(false);
        }
    }

	qRegisterMetaType<std::string>("std::string");
	connect(this, &MessageItemBase::sgDisconnected, this, &MessageItemBase::onDisconnected, Qt::QueuedConnection);
	connect(this, &MessageItemBase::sgUpdateUserMedal, this, &MessageItemBase::updateUserMedal, Qt::QueuedConnection);

}

MessageItemBase::~MessageItemBase()= default;

bool MessageItemBase::eventFilter(QObject *o, QEvent *e)
{
    if((o == _headLab || (_nameLab && o == _nameLab)) && g_pMainPanel)
    {
        if(e->type() == QEvent::MouseButtonDblClick)
        {
            if(_msgInfo.type == QTalk::Enum::GroupChat)
            {
                _isPressEvent = false;

                StSessionInfo sessionInfo;
                sessionInfo.userId = _msgInfo.from;
                sessionInfo.chatType = QTalk::Enum::TwoPersonChat;
                emit g_pMainPanel->sgOpenNewSession(sessionInfo);
            }

        }
        else if(e->type() == QEvent::MouseButtonPress)
        {
            _isPressEvent = true;
            _alreadyRelease = false;
            QTimer::singleShot(350, [this](){
                if(_isPressEvent){

                    if(!_alreadyRelease && _msgInfo.type == QTalk::Enum::GroupChat)
                    {
                        g_pMainPanel->insertAt(_msgInfo.xmpp_id, _msgInfo.from);
                    }
                    else
                    {
                        if(_msgInfo.type == QTalk::Enum::ChatType::ConsultServer){
                            g_pMainPanel->showUserCard(_msgInfo.real_id);
                        } else{
                            g_pMainPanel->showUserCard(_msgInfo.from);
                        }
                    }
                }
            });
        }
        else if(e->type() == QEvent::MouseButtonRelease)
        {
            _alreadyRelease = true;
        }
        else if(e->type() == QEvent::Enter)
        {
            setCursor(Qt::PointingHandCursor);
        }
        else if(e->type() == QEvent::Leave)
        {
            setCursor(Qt::ArrowCursor);
        }
    }
    else if(o == _resending)
    {
        if(e->type() == QEvent::MouseButtonPress)
        {
            if(g_pMainPanel && g_pMainPanel->getConnectStatus())
            {
                //
                int ret = QtMessageBox::question(g_pMainPanel, tr("提醒"), tr("确认重发此消息？"));
                if(ret == QtMessageBox::EM_BUTTON_YES)
                {
                    //
                    if(_sending)
                    {
                        _sending->setVisible(true);
                        _sending->startMovie();
                    }
                    if(_resending)
                        _resending->setVisible(false);

                    g_pMainPanel->resendMessage(this);
                }
            }
        }
    }

    return QObject::eventFilter(o, e);
}

void MessageItemBase::setReadState(const QInt32& state)
{
    if (_readSts == EM_BLACK_RET) {
        return;
    }
    //
    if(_readSts > state)
        return;
    _readSts = state;
    if(state > 0)
    {
        _msgInfo.read_flag = state;
        _msgInfo.state = 1;
        if(_sending)
        {
            _sending->deleteLater();
            _sending = nullptr;
        }
        if(_resending)
        {
            _resending->deleteLater();
            _resending = nullptr;
        }
    }

    if (_readStateLabel == nullptr) {
        return;
    }

    if(state & EM_READSTS_READED)
    {
        _readStateLabel->setText(tr("已读"));
        _readStateLabel->setStyleSheet("color:rgba(181, 181, 181, 1)");
    }
    else if(state & EM_READSTS_UNREAD)
    {
        _readStateLabel->setText(tr("未读"));
        _readStateLabel->setStyleSheet("color:rgba(0, 195, 188, 1)");
    }
}

bool MessageItemBase::contains(const QPoint& pos)
{
    return _contentFrm->geometry().contains(mapFromGlobal(pos));
}

void MessageItemBase::showShareCheckBtn(bool show)
{
    _btnShareCheck->setVisible(show);
}

void MessageItemBase::checkShareCheckBtn(bool check)
{
    _btnShareCheck->setChecked(check);
}

void MessageItemBase::onDisconnected() {

    if(!Platform::instance().isMainThread())
    {
        emit sgDisconnected();
        return;
    }

    if(0 == _readSts)
    {
        if(_sending)
        {
            _sending->setVisible(false);
            _sending->stopMovie();
        }
        if(_resending)
            _resending->setVisible(true);
    }
}

void MessageItemBase::updateUserMedal() {
    if (QTalk::Enum::ChatType::GroupChat == _msgInfo.type
        && QTalk::Entity::MessageDirectionReceive == _msgInfo.direction ) {

        if(nullptr == _medalWgt)
            _medalWgt = new MedalWgt(18, this);
        std::set<QTalk::StUserMedal> user_medal;
        g_pMainPanel->getUserMedal(_msgInfo.from.toStdString(), user_medal);
        _medalWgt->addMedals(user_medal, true);
    }
}

void MessageItemBase::updateMessageInfo(const StNetMessageResult &messageInfo) {
    this->_msgInfo = messageInfo;
    switch (messageInfo.msg_type)
    {

        case QTalk::Entity::MessageTypeFile:
            break;
        case QTalk::Entity::MessageTypeText:
        case QTalk::Entity::MessageTypeGroupAt:
        default:
        {
            auto* pItem = qobject_cast<TextMessItem*>(this);
            if(pItem)
                pItem->setStTextMessages(messageInfo.text_messages);
            break;
        }
    }
}
