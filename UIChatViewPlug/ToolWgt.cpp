#include "ToolWgt.h"
#include <QPushButton>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QTimer>
#include <QDebug>
#include <QCheckBox>
#include <QComboBox>
#include <QApplication>
#include <QtConcurrent>
#include <QJsonDocument>
#include <QJsonObject>
#include <QSettings>
#include "InputWgt.h"

#include "../UICom/uicom.h"
#include "ChatViewMainPanel.h"
#include "../Emoticon/EmoticonMainWgt.h"
#include "../Platform/Platform.h"
#include "../WebService/AudioVideo.h"
#include "../QtUtil/Entity/JID.h"
#include "../WebService/WebService.h"
#include "../Platform/NavigationManager.h"
#include "../Platform/dbPlatForm.h"
#include "../CustomUi/LiteMessageBox.h"
#include "../CustomUi/QtMessageBox.h"
#include "search/LocalSearchMainWgt.h"
#ifdef _MACOS
#include "../Screenshot/mac/SnipScreenTool.h"
#else
#include "../Screenshot/SnipScreenTool.h"
#endif


#define DEM_BTN_ICON_LEN  30
extern ChatViewMainPanel* g_pMainPanel;
ToolWgt::ToolWgt(InputWgt* pInputWgt, ChatViewItem* chatItem)
    :QFrame(), _pInputWgt(pInputWgt), _pChatItem(chatItem)
{
	initUi();
}


ToolWgt::~ToolWgt()
{
    qInfo() << "tool wgt delete";
}

/**
  * @函数名   initUi
  * @功能描述 初始化画面
  * @参数
  * @author   cc
  * @date     2018/09/19
  */
void ToolWgt::initUi()
{
	this->setObjectName("ToolWgt");
#ifdef _QCHAT
    qchatMoreFun = new QchatMoreFun(_pChatItem->getPeerId(), _pChatItem->_chatType);
#endif

    QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "qunar.com", QApplication::applicationName());
	//
	_pBtnEmoticon = new QPushButton( this);
	_pBtnScreenshot = new QPushButton( this);
	_pBtnScreenshotSet = new QPushButton( this);
	_pBtnFile = new QPushButton( this);
    _pBtnCode = new QPushButton( this);
    _pBtnMultifunction = new QPushButton(this);
	//
    _pBtnEmoticon->setFocusPolicy(Qt::NoFocus);
    _pBtnScreenshot->setFocusPolicy(Qt::NoFocus);
    _pBtnScreenshotSet->setFocusPolicy(Qt::NoFocus);
    _pBtnFile->setFocusPolicy(Qt::NoFocus);
    _pBtnCode->setFocusPolicy(Qt::NoFocus);
    _pBtnMultifunction->setFocusPolicy(Qt::NoFocus);
    //
    _pMenu = new QMenu(this);
    _pMenu->setAttribute(Qt::WA_TranslucentBackground, true);
    _pScreenShotHideWnd = new QAction(tr("截屏时隐藏当前窗口"), _pMenu);
    _pScreenShotHideWnd->setCheckable(true);
    _pMenu->addAction(_pScreenShotHideWnd);

    //
    _pBtnEmoticon->setObjectName("Emoticon");
    _pBtnScreenshot->setObjectName("Screenshot");
    _pBtnScreenshotSet->setObjectName("ScreenshotSet");
    _pBtnFile->setObjectName("File");
    _pScreenShotHideWnd->setObjectName("ScreenShotHideWndAction");
    _pBtnCode->setObjectName("CodeBtn");
    _pBtnMultifunction->setObjectName("Multifunction");
    //
    _pBtnEmoticon->setToolTip(tr("表情"));
    _pBtnScreenshot->setToolTip(tr("截图"));
    _pBtnFile->setToolTip(tr("发送文件或图片"));
    _pBtnCode->setToolTip(tr("发送代码"));
    _pBtnMultifunction->setToolTip(tr("多功能"));
    //
    _pBtnEmoticon->setFixedSize(DEM_BTN_ICON_LEN, DEM_BTN_ICON_LEN);
    _pBtnScreenshot->setFixedSize(DEM_BTN_ICON_LEN, DEM_BTN_ICON_LEN);
    _pBtnScreenshotSet->setFixedSize(10, 10);
    _pBtnFile->setFixedSize(DEM_BTN_ICON_LEN, DEM_BTN_ICON_LEN);
    _pBtnCode->setFixedSize(DEM_BTN_ICON_LEN, DEM_BTN_ICON_LEN);
    _pBtnMultifunction->setFixedSize(DEM_BTN_ICON_LEN, DEM_BTN_ICON_LEN);

    _pBtnVideo = new QPushButton( this);
    _pBtnVideo->setFocusPolicy(Qt::NoFocus);
    _pBtnVideo->setObjectName("Video");
    _pBtnVideo->setToolTip(tr("实时视频"));
    _pBtnVideo->setFixedSize(DEM_BTN_ICON_LEN, DEM_BTN_ICON_LEN);

    _pBtnHistory = new QPushButton( this);
    _pBtnHistory->setFocusPolicy(Qt::NoFocus);
    _pBtnHistory->setObjectName("History");
    _pBtnHistory->setToolTip(tr("历史消息"));
    _pBtnHistory->setFixedSize(DEM_BTN_ICON_LEN, DEM_BTN_ICON_LEN);

    _pBtnShock = new QPushButton( this);
    _pBtnShock->setFocusPolicy(Qt::NoFocus);
    _pBtnShock->setObjectName("Shock");
    _pBtnShock->setToolTip(tr("窗口抖动"));
    _pBtnShock->setFixedSize(DEM_BTN_ICON_LEN, DEM_BTN_ICON_LEN);

    //
    auto * screenShotLayout = new QHBoxLayout;
	screenShotLayout->setMargin(0);
	screenShotLayout->setSpacing(0);
	screenShotLayout->addWidget(_pBtnScreenshot);
	screenShotLayout->addWidget(_pBtnScreenshotSet);
    auto * layout = new QHBoxLayout(this);
	layout->setSpacing(13);
	layout->addWidget(_pBtnEmoticon);
	layout->addLayout(screenShotLayout);
	layout->addWidget(_pBtnFile);
    layout->addWidget(_pBtnMultifunction);
    layout->addWidget(_pBtnVideo);
    layout->addWidget(_pBtnCode);
    layout->addWidget(_pBtnHistory);
    layout->addWidget(_pBtnShock);

    auto* rightLayout = new QHBoxLayout;
    layout->addLayout(rightLayout,1);

	layout->addItem(new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Fixed));
	//
	setLayout(layout);
	//
    _pMultiMenu = new QMenu(this);
    _pMultiMenu->setAttribute(Qt::WA_TranslucentBackground, true);
    _pMultiMenu->setAttribute(Qt::WA_TranslucentBackground, true);

    auto* scanQRCode = new QAction(tr("扫一扫"), this);

    _pMultiMenu->addAction(scanQRCode);

    QMenu* menu = nullptr;

//    if(_pChatItem->_chatType != QTalk::Enum::GroupChat && _pChatItem->_chatType != QTalk::Enum::System )
    {
        menu = new QMenu(this);
        pVideoAct = new QAction(tr("视频通话"), menu);
        pAudioAct = new QAction(tr("音频通话"), menu);
        menu->addAction(pVideoAct);
        menu->addAction(pAudioAct);

        connect(pVideoAct, &QAction::triggered, [this](bool){
            std::string peerId = QTalk::Entity::JID(_pChatItem->getPeerId().usrId()).basename();
            g_pMainPanel->start2Talk_old(peerId, true, true);
        });

        connect(pAudioAct, &QAction::triggered, [this](bool){
            std::string peerId = QTalk::Entity::JID(_pChatItem->getPeerId().usrId()).basename();
            g_pMainPanel->start2Talk_old(peerId, false, true);
        });
    }


    connect(scanQRCode, &QAction::triggered, [](bool){
        emit g_pMainPanel->showQRcode();
    });

	//_pBtnFile
	connect(_pBtnFile, &QPushButton::clicked, this, &ToolWgt::onFileBtnClicked);
	//
	connect(_pBtnScreenshot, &QPushButton::clicked, this, &ToolWgt::onBtnScreenshot);
	connect(_pBtnEmoticon, &QPushButton::clicked, this, &ToolWgt::onpBtnEmoticon);
	connect(_pBtnScreenshotSet, &QPushButton::clicked, [&]()
	{
	    bool hideWnd = AppSetting::instance().getScreentShotHideWndFlag();
        _pScreenShotHideWnd->setChecked(hideWnd);
		_pMenu->exec(QCursor::pos());
	});

    connect(_pScreenShotHideWnd, &QAction::triggered, [](bool isChecked){
        AppSetting::instance().setScreentShotHideWndFlag(isChecked);
    });

    connect(_pBtnVideo, &QPushButton::clicked, [this, menu]()
    {

#ifdef _WINDOWS
#ifdef PLATFORM_WIN32
        QtMessageBox::information(g_pMainPanel, tr("提醒"), tr("暂不支持此功能!"));
        return;
#endif
#endif

        if(_pChatItem)
        {
            if(_pChatItem->_chatType == QTalk::Enum::TwoPersonChat)
            {
                if(menu)
                    menu->exec(QCursor::pos());
            }
            else if(_pChatItem->_chatType == QTalk::Enum::GroupChat)
            {
                g_pMainPanel->sendAudioVideoMessage(_pChatItem->_uid, _pChatItem->_chatType);
                QString groupId = _pChatItem->_uid.qUsrId();
                auto info = DB_PLAT.getGroupInfo(groupId.toStdString());
                g_pMainPanel->startGroupTalk(groupId, QString::fromStdString(QTalk::getGroupName(info)));
            }
        }

    });
    connect(_pBtnCode, &QPushButton::clicked, [this](){
        if(g_pMainPanel)
        {
            emit g_pMainPanel->sgOperator(tr("发送代码"));
            g_pMainPanel->showSendCodeWnd(_pChatItem->getPeerId());
        }
    });
    connect(_pBtnHistory, &QPushButton::clicked, this, &ToolWgt::showSearchWnd);
    connect(_pBtnMultifunction, &QPushButton::clicked, [this](){
        _pMultiMenu->exec(QCursor::pos());
    });

    connect(_pBtnShock, &QPushButton::clicked, [this](){

        static long long time = 0;
        long long cur_t = QDateTime::currentMSecsSinceEpoch();
        if(cur_t - time > 1000 * 5)
        {
            time = cur_t;
            g_pMainPanel->sendShockMessage(_pChatItem->_uid, _pChatItem->_chatType);
//            emit g_pMainPanel->sgShockWnd();
        }
        else {
            LiteMessageBox::failed(tr("抖动太频繁, 待会再试吧!"), 2000);
        }

    });
}

/**
  * @函数名   onFileBtnClicked
  * @功能描述 选择发送文件
  * @参数
  * @author   cc
  * @date     2018/09/21
  */
void ToolWgt::onFileBtnClicked()
{
    emit g_pMainPanel->sgOperator(tr("发送文件"));

    QString strHistoryFileDir = QString::fromStdString(PLAT.getHistoryDir());
    QString strFilePath = QFileDialog::getOpenFileName(g_pMainPanel, tr("选择需要发送的文件"), strHistoryFileDir);
	if (!strFilePath.isEmpty() && _pInputWgt)
	{
        PLAT.setHistoryDir(strFilePath.toStdString());
		_pInputWgt->dealFile(strFilePath, true);
	}

	_pInputWgt->setFocus();

}

/**
  * @函数名   
  * @功能描述 
  * @参数
  * @author   cc
  * @date     2018/10/17
  */
void ToolWgt::onBtnScreenshot()
{
    emit g_pMainPanel->sgOperator(tr("截图"));

	int waitSecond = 0;
    bool hideWnd = AppSetting::instance().getScreentShotHideWndFlag();
	if (hideWnd)
	{
		waitSecond = 200;
		UICom::getInstance()->getAcltiveMainWnd()->hide();
	}
	QTimer::singleShot(waitSecond, [this]()
	{
        SnipScreenTool::getInstance()->setConversionId(_pChatItem->conversionId());
		SnipScreenTool::getInstance()->Init().Start();
	});
}

/**
  * @函数名   
  * @功能描述 
  * @参数
  * @author   cc
  * @date     2018/10/17
  */
void ToolWgt::onpBtnEmoticon()
{
    emit g_pMainPanel->sgOperator(tr("表情"));

	QPoint pos = QCursor::pos();
    EmoticonMainWgt::instance()->setConversionId(_pChatItem->conversionId());
    EmoticonMainWgt::instance()->setVisible(false);
    EmoticonMainWgt::instance()->setVisible(true);
    EmoticonMainWgt::instance()->move(pos.x() - 60, pos.y() - 300);
    EmoticonMainWgt::instance()->setFocus();
}

void ToolWgt::openLinkWithCkey(const QUrl& url)
{
    MapCookie cookies;
    cookies["q_ckey"] = QString::fromStdString(PLAT.getClientAuthKey());
    WebService::loadUrl(url, false, cookies);
}

void ToolWgt::sendJsonPrud(const QString &products) {
    if(g_pMainPanel && _pChatItem){
        QTalk::Entity::UID uid = _pChatItem->_uid;
        QTalk::Entity::JID jid(uid.realId());
        QTalk::Entity::JID userId(uid.usrId());
        std::string type = _pChatItem->_chatType == QTalk::Enum::ConsultServer ? "consult" : "note";
        ChatMsgManager::sendProduct(jid.username(),userId.username(),products.toStdString(),type);
    }
}

void ToolWgt::sendQuickReply(const std::string &text) {
    if(g_pMainPanel && _pChatItem){
        g_pMainPanel->sendTextMessage(_pChatItem->_uid, _pChatItem->_chatType, text);
    }
}

void ToolWgt::sessionTransfer(const std::string &newJid, const std::string &reason) {
    if(g_pMainPanel && _pChatItem){
        QTalk::Entity::UID uid = _pChatItem->_uid;
        QtConcurrent::run(&ChatMsgManager::sessionTransfer, uid, newJid, reason);
    }
}


void ToolWgt::switchSession(const int &) {
#if !defined (_STARTALK) && !defined(_QCHAT)
    _pBtnVideo->setVisible(false);
#endif

    _pBtnShock->setVisible(_pChatItem->_chatType == QTalk::Enum::TwoPersonChat);
    bool showVideo = _pChatItem->_chatType != QTalk::Enum::GroupChat && _pChatItem->_chatType != QTalk::Enum::System;
    pVideoAct->setVisible(showVideo);
    pAudioAct->setVisible(showVideo);
}
