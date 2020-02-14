#include "MainWindow.h"
#include "UIGolbalManager.h"
#include <QEvent>
#include <assert.h>
#include <QCloseEvent>
#include <QMenu>
#include <QAction>
#include <QApplication>
#include <QDir>
#include <QStandardItemModel>
#include <QDesktopWidget>
#include <QDateTime>
#include <QtNetwork/QHostInfo>
#include <QPropertyAnimation>
#include <QMenuBar>
#include <QtConcurrent>
#include "../UICom/uicom.h"
#include "../interface/view/IUITitleBarPlug.h"
#include "../interface/view/IUINavigationPlug.h"
#include "../interface/view/IUIChatViewPlug.h"
#include "../interface/view/IUIGroupManagerPlug.h"
#include "../interface/view/IUIPictureBroswerPlug.h"
#include "../UICom/UIEntity.h"
#include "../CustomUi/UShadowEffect.h"
#include "../interface/view/IUICardManagerPlug.h"
#include "../interface/view/IUILoginPlug.h"
#include "../Platform/Platform.h"
#include "../LogicManager/LogicManager.h"
#include "../UICom/uicom.h"
#include "MessageManager.h"
#include "SystemTray.h"
#include "../interface/view/IUIAddressBookPlug.h"
#include "../interface/view/IUIOAManagerPlug.h"
#include "../QtUtil/Utils/Log.h"
#include "../Platform/AppSetting.h"
#include "../entity/UID.h"
#include "../CustomUi/QtMessageBox.h"
#include "MacApp.h"
#ifdef _WINDOWS
#include <windows.h>
#include <windowsx.h>
#endif

#define WIN_STATE "WIN_STATE"
#define WIN_WIDTH "WIN_WIDTH"
#define WIN_HEIGHT "WIN_HEIGHT"
#define WIN_X "WIN_X"
#define WIN_Y "WIN_Y"

//class StSessionInfo;
bool MainWindow::_sys_run = true;
MainWindow::MainWindow(QWidget *parent) :
#ifdef _MACOS
    UShadowDialog(parent, true, false),
#else
    UShadowDialog(parent, true, true),
#endif
	_initUi(true),
    _titleBarPlugin(nullptr),
    _navigationPlugin(nullptr),
    _chatViewPlugin(nullptr),
    _titleBar(nullptr),
    _navigationPanel(nullptr),
    _chatViewPanel(nullptr),
    _mainLayout(nullptr),
    _bottomFrm(nullptr),
    _bottomSplt(nullptr),
	_pCardManagerPlug(nullptr),
	_pCardManager(nullptr),
    _logindlg(nullptr),
    _pLoginPlug(nullptr),
	_pPictureBrowser(nullptr),
	_pPictureBrowserPlug(nullptr),
	_pConfigLoader(nullptr),
	_pAddressBook(nullptr),
	_pAddressBookPlug(nullptr),
	_pOAManager(nullptr),
	_pOAManagerPlug(nullptr),
    _pOfflineTimer(nullptr),
    _login_t(0),
    _logout_t(0),
    _isOffline(true),
    _noOperatorThread(nullptr)
{
    _pListener = new QTalkMsgListener(this);
    _pMessageManager = new QTalkMsgManager;
    // 系统设置
    AppSetting::instance();
    //
#if defined(_STARTALK)
    setWindowTitle("StarTalk");
#elif defined(_QCHAT)
    setWindowTitle("QChat");
#else
    setWindowTitle("QTalk");
#endif
    //
    _timr = new QTimer;
    _timr->setInterval(90 * 1000);
    connect(_timr, &QTimer::timeout, this, &MainWindow::sendHeartBeat);
    connect(this, &MainWindow::LoginSuccess, this, &MainWindow::LoginResult);
    connect(this, &MainWindow::sgRestartWithMessage, this, &MainWindow::restartWithMessage, Qt::QueuedConnection);
    //
    QHostInfo info = QHostInfo::fromName(QHostInfo::localHostName());
    for(const QHostAddress &address : info.addresses())
    {
        if(address.protocol() == QAbstractSocket::IPv4Protocol)
        {
            auto addr = address.toString();
            if(addr != "127.0.0.1")
                _ipv4Address += QString("%1; ").arg(addr);
        }
    }
    //
#ifdef _MACOS
    _pWindowMenuBar = new QMenuBar(nullptr);
    //
    QMenu *toolMenu = _pWindowMenuBar->addMenu(tr("工具"));
    auto* addNew = new QAction(tr("程序多开"), toolMenu);
    _pFeedBackLog = new QAction(tr("快速反馈日志"), toolMenu);
    toolMenu->addAction(addNew);
    toolMenu->addAction(_pFeedBackLog);
    //
    connect(addNew, &QAction::triggered, this, &MainWindow::sgRunNewInstance);
#endif
}


MainWindow::~MainWindow()
{
    if(_pLocalServer)
        delete _pLocalServer;

//   UIGolbalManager::GetUIGolbalManager()->UnloadPluginQt("UILoginPlug");
//    UIGolbalManager::GetUIGolbalManager()->UnloadPluginQt("UICardManager");
//	UIGolbalManager::GetUIGolbalManager()->UnloadPluginQt("UIGroupManager");
//    UIGolbalManager::GetUIGolbalManager()->UnloadPluginQt("UIPictureBrowser");
//    UIGolbalManager::GetUIGolbalManager()->UnloadPluginQt("UITitlebarPlug");
//    UIGolbalManager::GetUIGolbalManager()->UnloadPluginQt("UINavigationPlug");
//    UIGolbalManager::GetUIGolbalManager()->UnloadPluginQt("UIChatViewPlug");
}


bool MainWindow::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
#ifdef _WINDOWS
	MSG* msg = (MSG*)message;
	int pixelRatio = this->devicePixelRatio();
	pixelRatio = qMax(pixelRatio, 1);
	int tempBorderWidth = boundaryWidth;
	switch (msg->message)
	{
	case WM_NCHITTEST:
		qreal xPos = GET_X_LPARAM(msg->lParam) / pixelRatio - this->frameGeometry().x();
		qreal yPos = GET_Y_LPARAM(msg->lParam) / pixelRatio - this->frameGeometry().y();

		if (xPos < tempBorderWidth && yPos < tempBorderWidth)
			*result = HTTOPLEFT;
		else if (xPos >= width() - tempBorderWidth && yPos < tempBorderWidth)
			*result = HTTOPRIGHT;
		else if (xPos < tempBorderWidth&&yPos >= height() - tempBorderWidth)
			*result = HTBOTTOMLEFT;
		else if (xPos >= width() - tempBorderWidth && yPos >= height() - tempBorderWidth)
			*result = HTBOTTOMRIGHT;
		else if (xPos < tempBorderWidth)
			*result = HTLEFT;
		else if (xPos >= width() - tempBorderWidth)
			*result = HTRIGHT;
		else if (yPos < tempBorderWidth)
			*result = HTTOP;
		else if (yPos >= height() - tempBorderWidth)
			*result = HTBOTTOM;
		else
			return false;
		return true;
	}
#endif // _WINDOWS
	return false;
}

/**
  * @函数名
  * @功能描述 登陆成功
  * @参数
  * @date 2018.9.28
  */
void MainWindow::synSeverFinish()
{

}

void MainWindow::closeEvent(QCloseEvent *e)
{
	if (_sys_run)
	{
		this->setVisible(false);
		e->ignore();
	}
}

void MainWindow::init()
{
    initPanels();
    //
    UIGolbalManager::GetUIGolbalManager()->setStyleSheetAll();
}

/**
  * @函数名
  * @功能描述 获取所需面板插件及初始化面板
  * @参数
  * @date 2018.9.16
  */
void MainWindow::initPanels()
{
    initTitleBar();
    initChatView();
	initAddressBook();
    initGroupManager();
    initCardManager();
    initPictureBrowser();
    initOAManager();
    initNavigation();
}

void MainWindow::initLayouts()
{
    _mainFrm = new QFrame(this);
    _mainFrm->setObjectName("MainWindowMainFrm");
    auto *glay = new QVBoxLayout;
    glay->setContentsMargins(0, 0, 0, 0);
    glay->setSpacing(0);
    _pCenternWgt->setLayout(glay);
    glay->addWidget(_mainFrm);
//    _mainBottomFrm = new QFrame(this);
//    _mainBottomFrm->setFixedHeight(6);
//    _mainBottomFrm->setObjectName("MainWindowMainBottomFrm");
//    glay->addWidget(_mainBottomFrm);

    if (!_mainLayout)
    {
        _mainLayout = new QVBoxLayout;
        _mainLayout->setContentsMargins(0, 0, 0, 0);
        _mainLayout->setSpacing(0);
        _mainFrm->setLayout(_mainLayout);
    }
    if (_titleBar)
    {
        _mainLayout->addWidget(_titleBar);
        if (_titleBarPlugin)
        {
            _titleBarPlugin->setCtrlWdt(this);
        }
    }
    _bottomFrm = new QFrame(this);
    _mainLayout->addWidget(_bottomFrm);
    bodyLay = new QStackedLayout(_bottomFrm);
    bodyLay->setMargin(0);
    bodyLay->setSpacing(0);

	// 聊天窗口区域
    _bottomSplt = new QSplitter;
    _bottomSplt->setHandleWidth(1);
    bodyLay->addWidget(_bottomSplt);
    bodyLay->setCurrentWidget(_bottomSplt);
    if (_navigationPanel)
    {
        _bottomSplt->addWidget(_navigationPanel);
    }
    if (_chatViewPanel)
    {
        _bottomSplt->addWidget(_chatViewPanel);
    }
    _bottomSplt->setStretchFactor(1,1);
    _bottomSplt->setCollapsible(0, false);
    _bottomSplt->setCollapsible(1, false);

	// 通讯录相关
	if (_pAddressBook)
	{
		_pAddressBook->setVisible(false);
        bodyLay->addWidget(_pAddressBook);
	}
	// OA相关
	if (_pOAManager)
	{
		_pOAManager->setVisible(false);
        bodyLay->addWidget(_pOAManager);
	}
}

/**
  * @功能描述 加载标题栏插件
  * @参数
  * @date 2018.9.16
  */
void MainWindow::initTitleBar()
{
    QObject* plugin = UIGolbalManager::GetUIGolbalManager()->GetPluginInstanceQt("UITitlebarPlug");
    if (plugin)
    {
        _titleBarPlugin = qobject_cast<IUITitlebarPlug *>(plugin);
        if (_titleBarPlugin)
        {
            _titleBarPlugin->init();
            _titleBar = _titleBarPlugin->widget();
//            UIGolbalManager::GetUIGolbalManager()->setStyleSheetForPlugin("UITitlebarPlug");
            //
        }
    }
    assert(_titleBarPlugin);
    assert(_titleBar);
}


void MainWindow::initCardManager()
{
    QObject* plugin = UIGolbalManager::GetUIGolbalManager()->GetPluginInstanceQt("UICardManager");
    if (plugin)
    {
        _pCardManagerPlug = qobject_cast<IUICardManagerPlug *>(plugin);
        if (_pCardManagerPlug)
        {
            _pCardManagerPlug->init();
            _pCardManager = _pCardManagerPlug->widget();
//            UIGolbalManager::GetUIGolbalManager()->setStyleSheetForPlugin("UICardManager");
        }
    }
    assert(_pCardManagerPlug);
    assert(_pCardManager);
}


/**
  * @功能描述 加载导航插件
  * @参数
  * @date 2018.9.17
  */
void MainWindow::initNavigation()
{
    QObject* plugin = UIGolbalManager::GetUIGolbalManager()->GetPluginInstanceQt("UINavigationPlug");
    if (plugin)
    {
        _navigationPlugin = qobject_cast<IUINavigationPlug *>(plugin);
        if (_navigationPlugin)
        {
            _navigationPlugin->init();
            _navigationPanel = _navigationPlugin->widget();
//            UIGolbalManager::GetUIGolbalManager()->setStyleSheetForPlugin("UINavigationPlug");
        }
    }
    assert(_navigationPlugin);
    assert(_navigationPanel);
}

/**
  * @功能描述 加载聊天面板插件
  * @参数
  * @date 2018.9.17
  */
void MainWindow::initChatView()
{
    QObject* plugin = UIGolbalManager::GetUIGolbalManager()->GetPluginInstanceQt("UIChatViewPlug");
    if (plugin)
    {
        _chatViewPlugin = qobject_cast<IUIChatViewPlug *>(plugin);
        if (_chatViewPlugin)
        {
            _chatViewPlugin->init();
            _chatViewPanel = _chatViewPlugin->widget();
//            UIGolbalManager::GetUIGolbalManager()->setStyleSheetForPlugin("UIChatViewPlug");
        }
    }
    assert(_chatViewPlugin);
    assert(_chatViewPanel);
}

/**
  * @功能描述 链接插件间信号槽
  * @参数
  * @date 2018.9.18
  */
void MainWindow::connectPlugs()
{
    qRegisterMetaType<QVector<int>>("QVector<int>");
//    connect(_chatViewPanel, SIGNAL(recvMessageSignal()), _pSysTrayIcon, SLOT(onRecvMessage()));
    connect(_titleBar, SIGNAL(sgCurFunChanged(int)),
            this, SLOT(onCurFunChanged(int)));
    connect(_pAddressBook, SIGNAL(sgSwitchCurFun(int)),
            this, SLOT(onCurFunChanged(int)));
    connect(_pCardManager, SIGNAL(sgSwitchCurFun(int)),
            this, SLOT(onCurFunChanged(int)));
    connect(_chatViewPanel, SIGNAL(sgSwitchCurFun(int)),
            this, SLOT(onCurFunChanged(int)));

    connect(_titleBar, SIGNAL(sgJumpToNewMessage()), _navigationPanel, SLOT(jumpToNewMessage()));
    connect(this, SIGNAL(appDeactivated()), _titleBar, SLOT(onAppDeactivated()));
    connect(this, SIGNAL(appDeactivated()), _chatViewPanel, SLOT(onAppDeactivated()));
    connect(this, SIGNAL(appDeactivated()), _navigationPanel, SLOT(onAppDeactivated()));
    connect(this, SIGNAL(appDeactivated()), _pSysTrayIcon, SLOT(onAppDeactivated()));

    connect(_navigationPanel, SIGNAL(sgSessionInfo(const StSessionInfo&)),
            _chatViewPanel, SLOT(onChatUserChanged(const StSessionInfo&)), Qt::QueuedConnection);
    connect(_titleBar, SIGNAL(showSelfUserCard(const QString&)),
            _pCardManager, SLOT(shwoUserCard(const QString&)));
    connect(_chatViewPanel, SIGNAL(sgShowUserCard(const QString&)),
            _pCardManager, SLOT(shwoUserCard(const QString&)));
    connect(_chatViewPanel, SIGNAL(showUserCardSignal(const QString&)),
            _pCardManager, SLOT(shwoUserCard(const QString&)));
    connect(_navigationPanel, SIGNAL(showUserCardSignal(const QString&)),
            _pCardManager, SLOT(shwoUserCard(const QString&)));
    connect(_navigationPanel, SIGNAL(showGroupCardSignal(const QString&)),
            _pCardManager, SLOT(showGroupCard(const QString&)));
	connect(_chatViewPanel, SIGNAL(showGroupCardSignal(const QString&)),
		_pCardManager, SLOT(showGroupCard(const QString&)));

	qRegisterMetaType<QTalk::Entity::UID>("QTalk::Entity::UID");
    connect(_navigationPanel, SIGNAL(removeSession(const QTalk::Entity::UID&)),
            _chatViewPanel, SLOT(onRemoveSession(const QTalk::Entity::UID&)));
    connect(_titleBar, SIGNAL(sgOpenNewSession(const StSessionInfo&)),
            _navigationPanel, SLOT(onNewSession(const StSessionInfo&)));
    connect(this, SIGNAL(sgJumtoSession(const StSessionInfo&)),
            _navigationPanel, SLOT(onNewSession(const StSessionInfo&)));
	connect(_pCardManager, SIGNAL(sgOpenNewSession(const StSessionInfo&)),
		_navigationPanel, SLOT(onNewSession(const StSessionInfo&)));
	connect(_chatViewPanel, SIGNAL(sgOpenNewSession(const StSessionInfo&)),
		_navigationPanel, SLOT(onNewSession(const StSessionInfo&)));
    connect(_pAddressBook, SIGNAL(sgOpenNewSession(const StSessionInfo&)),
            _navigationPanel, SLOT(onNewSession(const StSessionInfo&)));
    connect(_pAddressBook, SIGNAL(sgSwitchCurFun(int)),
            _titleBar, SLOT(onSwitchFunc(int)));
    connect(_pCardManager, SIGNAL(sgSwitchCurFun(int)),
            _titleBar, SLOT(onSwitchFunc(int)));
    connect(_chatViewPanel, SIGNAL(sgJumpToStructre(const QString&)),
            _pAddressBook, SLOT(jumpToUserStructre(const QString&)));
    connect(_chatViewPanel, SIGNAL(sgSwitchCurFun(int)),
            _titleBar, SLOT(onSwitchFunc(int)));
    connect(_chatViewPanel, SIGNAL(creatGroup(const QString&)),
            _pGroupManager, SLOT(onCreatGroup(const QString&)));
    connect(_titleBar, SIGNAL(creatGroup(const QString&)),
            _pGroupManager, SLOT(onCreatGroup(const QString&)));
    connect(_chatViewPanel, SIGNAL(addGroupMember(const QString&)),
            _pGroupManager, SLOT(onAddGroupMember(const QString&)));
    connect(_chatViewPanel, SIGNAL(showChatPicture(const QString&, const QString&, int)),
            _pPictureBrowser, SLOT(onShowChatPicture(const QString&, const QString&, int)));
    connect(_chatViewPanel, SIGNAL(sgShowPicture(const QString&, const QString&)),
            _pPictureBrowser, SLOT(showPicture(const QString&, const QString&)));
//    connect(_pGroupManager, SIGNAL(setTreeDataFinised()), _pAddressBook, SLOT(updateStaffUi()));
    connect(_navigationPanel, SIGNAL(updateTotalUnreadCount(int)), _titleBar, SLOT(updateUnreadCount(int)));
	connect(_titleBar, SIGNAL(feedbackLog(const QString&)), _chatViewPanel, SLOT(packAndSendLog(const QString&)));
	connect(this, SIGNAL(systemShortCut()), _chatViewPanel, SLOT(systemShortCut()));
	connect(_titleBar, SIGNAL(systemQuitSignal()), this, SLOT(systemQuit()));

	connect(_chatViewPanel, SIGNAL(sgShortCutSwitchSession(int)),
	        _navigationPanel, SLOT(onShortCutSwitchSession(int)));

	connect(_navigationPanel, SIGNAL(sgShowUnreadMessage(int, const QTalk::Entity::UID&, const QString&, qint64, int)),
	        _pSysTrayIcon, SIGNAL(sgShowUnreadMessage(int, const QTalk::Entity::UID&, const QString&, qint64, int)));
	//
	connect(_pCardManager, SIGNAL(sgJumpToStructre(const QString&)), _pAddressBook, SLOT(onJumpToStructre(const QString&)));
    connect(_pCardManager, SIGNAL(sgShowHeadWnd(const QString&, bool)),
            _titleBar, SLOT(onShowHeadWnd(const QString&, bool)));
	//connect(_chatViewPanel, SIGNAL(sgForwardMessage(const QString&)), _pGroupManager, SLOT(onForfardMessage(const QString&)));
	connect(_titleBar, SIGNAL(sgSetAutoLogin(bool)), this, SLOT(setAutoLogin(bool)));
	connect(_titleBar, SIGNAL(sgUpdateHotKey()), this, SLOT(onUpdateHotKey()));

	connect(_titleBar, SIGNAL(sgSaveSysConfig()), UIGolbalManager::GetUIGolbalManager(), SLOT(saveSysConfig()));
	connect(_titleBar, SIGNAL(sgSaveSysConfig()), this, SLOT(onSaveSysConfig()));
	connect(_titleBar, SIGNAL(msgSoundChanged()), _chatViewPanel, SLOT(onMsgSoundChanged()));

	//
	connect(_titleBar, SIGNAL(sgShowMessageRecordWnd(const QString&, const QString&)),
	        _chatViewPanel, SLOT(onShowSearchResult(const QString&, const QString&)));
	connect(_titleBar, SIGNAL(sgShowFileRecordWnd(const QString&)),
	        _chatViewPanel, SLOT(onShowSearchFileWnd(const QString&)));

    connect(_chatViewPanel, SIGNAL(sgWakeUpWindow()), this, SLOT(wakeUpWindow()));
    connect(this, SIGNAL(sgAppActive()), _chatViewPanel, SLOT(onAppActive()));
    connect(this, SIGNAL(sgAppActive()), _navigationPanel, SLOT(onAppActive()));
    connect(_titleBar, SIGNAL(sgActiveWnd()), this, SLOT(wakeUpWindow()));
    //
    connect(_chatViewPanel, SIGNAL(sgShockWnd()), this, SLOT(onShockWnd()));
    connect(_chatViewPanel, SIGNAL(sgUserSendMessage()),
            this, SLOT(onUserSendMessage()));
    connect(UIGolbalManager::GetUIGolbalManager(), SIGNAL(sgMousePressGlobalPos(QPoint)), _titleBar,
            SLOT(onMousePressGolbalPos(QPoint)));
    // show draft
    connect(_chatViewPanel, SIGNAL(sgShowDraft(const QTalk::Entity::UID&, const QString&)),
            _navigationPanel, SIGNAL(sgShowDraft(const QTalk::Entity::UID&, const QString&)));
    // sgShowNotify
    qRegisterMetaType<QTalk::StNotificationParam>("QTalk::StNotificationParam");
    connect(_chatViewPanel, SIGNAL(sgShowNotify(const QTalk::StNotificationParam&)),
            _pSysTrayIcon, SLOT(onShowNotify(const QTalk::StNotificationParam&)));
    // 自动回复
    connect(_titleBar, SIGNAL(sgAutoReply(bool)), _chatViewPanel, SLOT(setAutoReplyFlag(bool)));

    // 操作日志
    connect(_titleBar, SIGNAL(sgOperator(const QString&)), this, SLOT(addOperatorLog(const QString&)));
    connect(_chatViewPanel, SIGNAL(sgOperator(const QString&)), this, SLOT(addOperatorLog(const QString&)));
    connect(_navigationPanel, SIGNAL(sgOperator(const QString&)), this, SLOT(addOperatorLog(const QString&)));
    connect(_pCardManager, SIGNAL(sgOperator(const QString&)), this, SLOT(addOperatorLog(const QString&)));
    connect(_pAddressBook, SIGNAL(sgOpeartor(const QString&)), this, SLOT(addOperatorLog(const QString&)));
    connect(_pOfflineTimer, &QTimer::timeout, [this](){
        if(_pMessageManager)
        {
            _isOffline = true;
            _pOfflineTimer->stop();
            _logout_t = QDateTime::currentMSecsSinceEpoch();
            _pMessageManager->sendOnlineState(_login_t, _logout_t, _ipv4Address.toStdString());
        }
    });
    connect(_pLogTimer, &QTimer::timeout, [this](){
        if(_pMessageManager)
        {
            if(_operators.empty())
                return;

            std::vector<QTalk::StActLog> operators(_operators);
            _pMessageManager->sendOperatorStatistics(_ipv4Address.toStdString(), operators);
            QMutexLocker locker(&_logMutex);
            _operators.clear();
        }
    });
}

/**
 *
 * @param autoFlag
 */
void MainWindow::setAutoLogin(bool autoFlag)
{
   _pLoginPlug->setAutoLoginFlag(autoFlag);
}

/**
  * @函数名   initSystemTray
  * @功能描述 初始化系统托盘
  * @参数
  * @author   cc
  * @date     2018/11/09
  */
void MainWindow::initSystemTray()
{
	_pSysTrayIcon = new SystemTray(this);
#ifdef _MACOS
    connect(_pFeedBackLog, &QAction::triggered, _pSysTrayIcon, &SystemTray::onSendLog);
#endif
}

/**
  * @函数名   
  * @功能描述 
  * @参数
  * @author   cc
  * @date     2018/11/11
  */
void MainWindow::initGroupManager()
{
	QObject* plugin = UIGolbalManager::GetUIGolbalManager()->GetPluginInstanceQt("UIGroupManager");
	if (plugin)
	{
        _pGroupManagerPlug = qobject_cast<IUIGroupManagerPlug *>(plugin);
        if (_pGroupManagerPlug)
		{
            _pGroupManagerPlug->init();
            _pGroupManager = _pGroupManagerPlug->widget();
//            UIGolbalManager::GetUIGolbalManager()->setStyleSheetForPlugin("UIGroupManager");
		}
	}
	assert(_pGroupManagerPlug);
	assert(_pGroupManager);
}

void MainWindow::InitLogin(bool _enable, const QString& loginMsg)
{
    QObject* plugin = UIGolbalManager::GetUIGolbalManager()->GetPluginInstanceQt("UILoginPlug");
    if (plugin)
    {
        _pLoginPlug = qobject_cast<IUILoginPlug *>(plugin);
        if (_pLoginPlug)
        {
            _pLoginPlug->init();
            _pLoginPlug->enableAutoLogin(_enable);
            _pLoginPlug->initConf();
            if(!loginMsg.isEmpty())
                _pLoginPlug->setLoginMessage(loginMsg);
            _logindlg = (QDialog *)_pLoginPlug->widget();
        }
        if (_logindlg)
        {
            UIGolbalManager::GetUIGolbalManager()->setStyleSheetForPlugin("UILoginPlug");
            connect(_logindlg, SIGNAL(sgSynDataSuccess()), this, SLOT(openMainWindow()));
            _logindlg->show();
        }
    }
}

// 登陆成功 打开数据库同步服务器数据
void MainWindow::LoginResult(bool result)
{
    if (result)
    {
        _timr->start();
    }
}

void MainWindow::OnLoginSuccess(const std::string& strSessionId)
{
    emit LoginSuccess(true);
}

void MainWindow::openMainWindow()
{
	if (_initUi)
	{
        _isOffline = false;
        _login_t = QDateTime::currentMSecsSinceEpoch();
        _pOfflineTimer = new QTimer(this);
        _pLogTimer = new QTimer(this);
        _pOfflineTimer->setInterval(1000 * 60 * 60);
        _pLogTimer->setInterval(1000 * 60 * 60);
        _pLogTimer->start();
        //
        _pLocalServer = new LocalServer;
        _pLocalServer->runServer(qApp->applicationName());
	    //
		_initUi = false;
		init();
		initLayouts();
		connectPlugs();
		_chatViewPanel->setFocus();
		//
		_navigationPlugin->updateReadCount();
        // setwindow states
		QString configPath = QString("%1/mainWnd").arg(Platform::instance().getConfigPath().data());
        //
        _pConfigLoader = new QTalk::ConfigLoader(configPath.toLocal8Bit());
        if(_pConfigLoader->reload())
        {
            int wndstate = _pConfigLoader->getInteger(WIN_STATE);
            int wndWidth = _pConfigLoader->getInteger(WIN_WIDTH);
            int wndHeight = _pConfigLoader->getInteger(WIN_HEIGHT);
            int wndX = _pConfigLoader->getInteger(WIN_X);
            int wndY = _pConfigLoader->getInteger(WIN_Y);
            if(wndWidth <= 0 || wndHeight <= 0)
            {
                QDesktopWidget *deskTop = QApplication::desktop();
                int curMonitor = deskTop->screenNumber(_logindlg);
                QRect deskRect = deskTop->screenGeometry(curMonitor);
                wndWidth = deskRect.height();
                wndHeight = (int)(deskRect.height() * 0.75);
                wndX = (deskRect.width() - wndWidth) / 2  + deskRect.x();
                wndY = (deskRect.height() - wndHeight) / 2 + deskRect.y();
                //
                _pConfigLoader->setInteger(WIN_WIDTH, wndWidth);
                _pConfigLoader->setInteger(WIN_HEIGHT, wndHeight);
                _pConfigLoader->setInteger(WIN_X, wndX);
                _pConfigLoader->setInteger(WIN_Y, wndY);
                _pConfigLoader->saveConfig();
            }

            setGeometry(wndX, wndY, wndWidth, wndHeight);
            switch (wndstate)
            {
                case WND_MAXSIZE:
                    //showMaximized();
                    //break;
                case WND_NORMAL:
                default:
                {
                    showNormal();
                    // 判断是否超出屏幕范围
                    adjustWndRect();
                    break;
                }
            }
        }
        UICom::getInstance()->setAcltiveMainWnd(this);
        // 注册快捷键
        _pScreentShot = new QHotkey(this);
        _pWakeWnd = new QHotkey(this);
        onUpdateHotKey();
		connect(_pScreentShot, &QHotkey::activated, this, &MainWindow::onScreentShot);
		connect(_pWakeWnd, &QHotkey::activated, this, &MainWindow::wakeUpWindow);
		connect(_pLocalServer, &LocalServer::sgWakeupWindow, this, &MainWindow::wakeUpWindow);
        //
		if (_logindlg)
        {
			_logindlg->setVisible(false);
            _pLoginPlug->saveHeadPath();
		}

        std::function<int(STLazyQueue<bool > *)> func
            = [this](STLazyQueue<bool > *queue)->int
            {
		        int count = 0;
                while (!queue->empty()) {
                    queue->pop();
                    count++;
                }
                emit systemShortCut();
                return count;
		    };
        _pScreentShotQueue = new STLazyQueue<bool >(300, func);

        //
        _noOperatorThread = new NoOperationThread;
        connect(this, &MainWindow::sgResetOperator,
                _noOperatorThread, &NoOperationThread::resetUnOperatorTime,
                Qt::QueuedConnection);
        connect(_noOperatorThread, SIGNAL(sgUserLeave(bool)), _chatViewPanel, SLOT(setAutoReplyFlag(bool)));
        connect(_noOperatorThread, SIGNAL(sgUserLeave(bool)), this, SLOT(setUserStatus(bool)));
        connect(_titleBar, SIGNAL(sgAutoReply(bool)), _noOperatorThread, SLOT(setAutoReplyFlag(bool)));
        //
#ifdef _MACOS
        QMenu *wndMenu = _pWindowMenuBar->addMenu(tr("窗口"));
        auto* minSize = new QAction(tr("最小化"), wndMenu);
        auto* maxSize = new QAction(tr("缩放"), wndMenu);
        auto* showWnd = new QAction(tr("显示面板"), wndMenu);
        wndMenu->addAction(minSize);
        wndMenu->addAction(maxSize);
        wndMenu->addSeparator();
        wndMenu->addAction(showWnd);

        QMenu *systemMenu = _pWindowMenuBar->addMenu(tr("系统"));
        auto* setting = new QAction(tr("系统设置"), wndMenu);
        auto* about = new QAction(tr("关于"), wndMenu);
        systemMenu->addAction(setting);
        systemMenu->addAction(about);

        connect(minSize, &QAction::triggered, [this](){
            MacApp::showMinWnd(this);
        });
        connect(maxSize, &QAction::triggered, [this](){
            if(this->isMaximized())
                this->showNormal();
            else
                this->showMaximized();
        });
        connect(showWnd, &QAction::triggered, [this](){
            this->wakeUpWindow();
        });

        connect(setting, SIGNAL(triggered()), _titleBar, SLOT(onShowSystemWnd()));
        connect(about, SIGNAL(triggered()), _titleBar, SLOT(onShowAboutWnd()));
#endif
        //
        dealDumpFile();
        checkUpdater();
	}

	synSeverFinish();
}

/**
  * @函数名   
  * @功能描述 
  * @参数
  * @author   cc
  * @date     2018/12/14
  */
void MainWindow::onCurFunChanged(int index)
{
    //
    switch (index)
    {
        case 0:
            bodyLay->setCurrentWidget(_bottomSplt);
            break;
        case 1:
            bodyLay->setCurrentWidget(_pAddressBook);
            break;
        case 2:
            bodyLay->setCurrentWidget(_pOAManager);
            break;
        default:break;
    }
}

//
void MainWindow::onAppActive()
{
    emit sgResetOperator();

    if(_initUi)
    {
        //_logindlg->setVisible(true);
    }
    else
    {
        if(_isOffline)
        {
            _isOffline = false;
            _login_t = QDateTime::currentMSecsSinceEpoch();
        }
        else
        {
            if(_pOfflineTimer)
                _pOfflineTimer->stop();
        }

        _pSysTrayIcon->onWndActived();
#ifdef _MACOS
        if(!this->isVisible())
            MacApp::wakeUpWnd(this);
#endif
        emit sgAppActive();
        setVisible(true);
    }
}

void MainWindow::initPictureBrowser()
{
    QObject* plugin = UIGolbalManager::GetUIGolbalManager()->GetPluginInstanceQt("UIPictureBrowser");
    if (plugin)
    {
        _pPictureBrowserPlug = qobject_cast<IUIPictureBroswerPlug *>(plugin);
        if (_pPictureBrowserPlug)
        {
            _pPictureBrowserPlug->init();
            _pPictureBrowser = _pPictureBrowserPlug->widget();
//            UIGolbalManager::GetUIGolbalManager()->setStyleSheetForPlugin("UIPictureBrowser");
        }
    }
    assert(_pPictureBrowserPlug);
    assert(_pPictureBrowser);
}

/**
  * @函数名   
  * @功能描述 
  * @参数
  * @author   cc
  * @date     2018/12/15
  */
void MainWindow::initAddressBook()
{
	QObject* plugin = UIGolbalManager::GetUIGolbalManager()->GetPluginInstanceQt("UIAddressBook");
	if (plugin)
	{
		_pAddressBookPlug = qobject_cast<IUIAddressBookPlug*>(plugin);
		if (_pAddressBookPlug)
		{
			_pAddressBookPlug->init();
			_pAddressBook = _pAddressBookPlug->widget();
//			UIGolbalManager::GetUIGolbalManager()->setStyleSheetForPlugin("UIAddressBook");
		}
	}
	assert(_pAddressBookPlug);
	assert(_pAddressBook);
}

/**
  * @函数名   
  * @功能描述 
  * @参数
  * @author   cc
  * @date     2018/12/15
  */
void MainWindow::initOAManager()
{
	QObject* plugin = UIGolbalManager::GetUIGolbalManager()->GetPluginInstanceQt("UIOAManager");
	if (plugin)
	{
		_pOAManagerPlug = qobject_cast<IUIOAManagerPlug*>(plugin);
		if (_pOAManagerPlug)
		{
			_pOAManagerPlug->init();
			_pOAManager = _pOAManagerPlug->widget();
//			UIGolbalManager::GetUIGolbalManager()->setStyleSheetForPlugin("UIOAManager");
		}
	}
	assert(_pOAManagerPlug);
	assert(_pOAManager);
}

/**
  * @函数名   
  * @功能描述 
  * @参数
  * @author   cc
  * @date     2018/12/15
  */
void MainWindow::onAppDeactivate()
{
    if(!_isOffline && _pOfflineTimer)
    {
        _pOfflineTimer->start();
    }

    emit appDeactivated();
}

void MainWindow::saveWndState()
{
    int wndstate = WND_NORMAL;
    if(windowState() == Qt::WindowMaximized)
    {
        wndstate = WND_MAXSIZE;
    }
    QRect geo = this->normalGeometry();

    if(_pConfigLoader)
    {
        _pConfigLoader->setInteger(WIN_STATE, wndstate);
        _pConfigLoader->setInteger(WIN_WIDTH, geo.width());
        _pConfigLoader->setInteger(WIN_HEIGHT, geo.height());
        _pConfigLoader->setInteger(WIN_X, geo.x());
        _pConfigLoader->setInteger(WIN_Y, geo.y());
        _pConfigLoader->saveConfig();
    }
}

/**
 * 系统退出
 */
void MainWindow::systemQuit()
{
    _sys_run = false;
    _logout_t = QDateTime::currentMSecsSinceEpoch();
#ifndef _LINUX
    if(_pMessageManager)
    {
        _pMessageManager->sendOnlineState(_login_t, _logout_t, _ipv4Address.toStdString());

        if(!_operators.empty())
        {
            _pMessageManager->sendOperatorStatistics(_ipv4Address.toStdString(), _operators);
        }
    }
#endif
	saveWndState();
	QApplication::exit(0);
}

/**
 *
 */
void MainWindow::sendHeartBeat()
{
    if(_pMessageManager)
    {
        _pMessageManager->sendHearBeat();
    }
}

// 多屏处理
void MainWindow::adjustWndRect() {

    QRect thisGeo = this->geometry();
    QDesktopWidget *deskTop = QApplication::desktop();
    int curMonitor = deskTop->screenNumber(this);
    QRect deskRect = deskTop->screenGeometry(curMonitor);
    if(!deskRect.contains(thisGeo, true))
    {

        if(thisGeo.width() > deskRect.width())
        {
            thisGeo.setWidth(deskRect.width() - 100);
        }
        if(thisGeo.height() > deskRect.height())
        {
            thisGeo.setHeight(deskRect.height() - 100);
        }
        thisGeo.setTopLeft(deskRect.topLeft());

        setGeometry(thisGeo);
    }
}

/**
 *
 */
void MainWindow::onUpdateHotKey()
{
	std::string screentHot = AppSetting::instance().getScreenshotHotKey();
	std::string wakeWnd = AppSetting::instance().getWakeWndHotKey();
	bool isok = _pScreentShot->setShortcut(QKeySequence::fromString(screentHot.data(), QKeySequence::NativeText), true);
	isok = _pWakeWnd->setShortcut(QKeySequence::fromString(wakeWnd.data(), QKeySequence::NativeText), true);
}

void MainWindow::onScreentShot()
{
	_pScreentShotQueue->push(true);
}

void MainWindow::hideEvent(QHideEvent *e) {
    QWidget::hideEvent(e);
}

void MainWindow::keyPressEvent(QKeyEvent *e)
{
	if (e->key() == Qt::Key_Escape || e->key() == Qt::Key_Space)
	{
		e->accept();
	}

#ifdef _MACOS
	if (e->modifiers() == Qt::ControlModifier)
	{
	    if(e->key() == Qt::Key_W)
		    this->setVisible(false);
	    else if(e->key() == Qt::Key_M)
            this->showMinimized();
	    else if(e->key() == Qt::Key_Q)
        {
//            int ret =
//            if(ret == QtMessageBox::EM_BUTTON_YES)
//            {
//                exit(0);
//            }
//            e->accept();
//            return;
        }

	}
#endif
	UShadowDialog::keyPressEvent(e);
}

QWidget * MainWindow::getActiveWnd() {
    QWidget* wakeUpWgt = nullptr;
    if(_initUi)
        wakeUpWgt = _logindlg;
    else
        wakeUpWgt = this;

    return wakeUpWgt;
}

void MainWindow::wakeUpWindow()
{
    QWidget* wakeUpWgt = getActiveWnd();

#ifdef _WINDOWS
    if (wakeUpWgt->isActiveWindow())
		return;

	bool isMax = wakeUpWgt->isMaximized();
	wakeUpWgt->setWindowState(Qt::WindowMinimized);
#endif // _WINDOWS

    if(wakeUpWgt->isMinimized())
        wakeUpWgt->setWindowState((wakeUpWgt->windowState() & ~Qt::WindowMinimized) | Qt::WindowActive);

	wakeUpWgt->setVisible(true);
    QApplication::setActiveWindow(wakeUpWgt);
    wakeUpWgt->raise();
#ifdef _WINDOWS
	if (isMax) {
		wakeUpWgt->showMaximized();
	}
#endif // _WINDOWS
}

/**
 *
 * @param desc
 */
void MainWindow::addOperatorLog(const QString &desc)
{
    QTalk::StActLog log;
    log.desc = desc.toStdString();
    log.operatorTime = QDateTime::currentMSecsSinceEpoch();
    QMutexLocker locker(&_logMutex);
    _operators.push_back(log);
}

/**
 *
 */
void MainWindow::onShockWnd()
{
    if(!this->isMaximized() && !this->isFullScreen())
    {
        QRect now = this->geometry();
        auto* animation = new QPropertyAnimation(this, "pos");
        animation->setDuration(500);
        animation->setStartValue(QPoint(now.x(), now.y() + 20));
        animation->setEndValue(QPoint(now.x(), now.y() - 20));
        animation->setEasingCurve(QEasingCurve::InOutBounce);
        connect(animation, &QPropertyAnimation::finished, [this, now](){
            setGeometry(now);
        });
        animation->start(QAbstractAnimation::DeleteWhenStopped);
    }
}

/**
 *
 */
void MainWindow::onSaveSysConfig()
{
    if(_noOperatorThread)
        _noOperatorThread->setLeaveMinute();

    //
    UIGolbalManager::GetUIGolbalManager()->saveSysConfig();
}

void MainWindow::setUserStatus(bool isAway)
{
    if(_pMessageManager)
        _pMessageManager->chanegUserStatus(isAway ? "away" : "online");
}

void MainWindow::onUserSendMessage()
{
    _noOperatorThread->resetUnOperatorTime();
    if(_pMessageManager)
        _pMessageManager->chanegUserStatus("online");
}

void MainWindow::checkUpdater() {

    auto version = UIGolbalManager::GetUIGolbalManager()->_updater_version;
    QFuture<std::string> future = QtConcurrent::run(_pMessageManager, &QTalkMsgManager::checkUpdater, version);
//    future.waitForFinished();
    while(!future.isFinished())
        QApplication::processEvents(QEventLoop::AllEvents, 100);

    std::string ret = future.result();
    if(!ret.empty())
    {
#if defined(_LINUX) || defined(_STARTALK) || defined(_QCHAT)
        return;
#endif
//        std::ostringstream src;
//        src << Platform::instance().getAppdataRoamingPath()
//            << "/updater/updater."
//            #if defined(_WINDOWS)
//            << "exe";
//            #elif defined(_MACOS)
//            << "dmg";
//            #endif
//
//        QString updaterPath = QString(src.str().data());
//        QFileInfo exec_info(updaterPath);
//        if(exec_info.exists() && exec_info.isFile())
//        {
//            int _ret = QtMessageBox::question(this, "更新器有更新，是否打开更新？",
//#if defined(_WINDOWS)
//                    "更新后请在桌面或者开始菜单中启动程序"
//#elif defined(_MACOS)
//                    "更新后请在Applications中启动程序"
//#else
//
//#endif
//                    );
//            if(_ret == QtMessageBox::EM_BUTTON_YES)
//            {
//#if defined(_WINDOWS)
//                QProcess::startDetached(updaterPath, QStringList());
//#elif defined(_MACOS)
//                QStringList params;
//                params << updaterPath;
//                QProcess::execute("open", params);
//#else
//
//#endif
//                exit(0);
//            }
//        }

        int btn_ret = QtMessageBox::information(this,
                tr("更新提示"),
                QString(tr("更新器有更新，您可以点击 <a href=%1>%1</a> 下载更新").arg(ret.data())), QtMessageBox::EM_BUTTON_CANCEL_UPDATE);

//        if(btn_ret == QtMessageBox::EM_BUTTON_YES)
//        {
//
//        }
    }
}

#ifdef _MACOS
void MainWindow::onShowMinWnd() {
    MacApp::showMinWnd(this);
}


#endif

void MainWindow::changeEvent(QEvent *event) {
	if (event->type() == QEvent::LanguageChange)
	{

	}
	UShadowDialog::changeEvent(event);
}

void MainWindow::onGetHistoryError() {
    emit sgRestartWithMessage(tr("获取历史记录失败, 请重新登录!"));
}

void MainWindow::restartWithMessage(const QString &msg) {

    QString program = QApplication::applicationFilePath();
    QStringList arguments;
    arguments << "AUTO_LOGIN=OFF"
              << QString("MSG=%1").arg(msg);
    QProcess::startDetached(program, arguments);
    exit(0);
}

void delDmpFun(const QString& path, const std::string& ip)
{
    QDir dir(path);
    QFileInfoList infoList = dir.entryInfoList(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot);
    for (const auto& tmpInfo : infoList)
    {
        if(tmpInfo.isSymLink()) continue;

        if (tmpInfo.isFile())
        {
            if (tmpInfo.suffix().toLower() == "dmp")
            {
#if QT_VERSION >= QT_VERSION_CHECK(5, 10, 0)
                auto birth = tmpInfo.birthTime().toMSecsSinceEpoch();
#else
                auto birth = tmpInfo.lastModified().toMSecsSinceEpoch();
#endif
                auto now = QDateTime::currentMSecsSinceEpoch();
                if(tmpInfo.size() > 0 && tmpInfo.size() <= 50 * 1024 * 1024 && now - birth < 1000 * 60 * 60 * 24 * 7)
                {
                    std::string dumpFilePath = std::string(tmpInfo.absoluteFilePath().toLocal8Bit());
                    QTalkMsgManager::reportDump(ip, dir.dirName().toStdString(), dumpFilePath, birth);
                }
                QFile::remove(tmpInfo.absoluteFilePath());
            }
        }
        else if(tmpInfo.isDir())
        {
            auto infoPath = tmpInfo.absoluteFilePath();
            delDmpFun(infoPath, ip);
            if(QDir(infoPath).isEmpty())
                dir.rmpath(infoPath);
        }
    }
}

//
void MainWindow::dealDumpFile()
{
    QTimer::singleShot(60 * 1000, [this](){
        // deal dump
        QDateTime curDateTime = QDateTime::currentDateTime();
        auto appdata = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation).toLocal8Bit();
        QString logDirPath = QString("%1/logs/").arg(appdata.data());
        //
        QtConcurrent::run(delDmpFun, logDirPath, _ipv4Address.toStdString());
    });
}
