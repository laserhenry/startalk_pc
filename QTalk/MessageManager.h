#ifndef _MESSAGEMANAGER_H_
#define _MESSAGEMANAGER_H_

#include <string>
#include "../EventBus/Object.hpp"
#include "../Message/LoginMessgae.h"
#include "../EventBus/EventHandler.hpp"
#include "../EventBus/HandlerRegistration.hpp"
#include "../include/CommonStrcut.h"
#include "../Message/StatusMessage.h"

class QTalkMsgManager : public Object
{
public:
	QTalkMsgManager(){};

public:
    static void sendHearBeat();
    static void sendOnlineState(const QInt64& login_t, const QInt64& logout_t, const std::string& ip);
    static void sendOperatorStatistics(const std::string& ip, const std::vector<QTalk::StActLog>& operators);
    static void sendLogReport(const std::string& desc, const std::string &logPath);
    static void chanegUserStatus(const std::string &status);
    static void reportDump(const std::string& ip, const std::string& id, const std::string & dump, QInt64 crashTime);
    static std::string checkUpdater(int version);
};

//
class MainWindow;
class QTalkMsgListener : public EventHandler<LoginSuccessMessage>
        , public EventHandler<GetHistoryError>
{
public:
	explicit QTalkMsgListener(MainWindow* pUiControl);
	~QTalkMsgListener() override;

public:
	void onEvent(LoginSuccessMessage& e) override;
	void onEvent(GetHistoryError& e) override;

private:
	MainWindow*           _pUiControl;

};

#endif // _MESSAGEMANAGER_H_
