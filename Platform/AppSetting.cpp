//
// Created by admin on 2019-02-22.
//

#include "AppSetting.h"
#include "../QtUtil/lib/cjson/cJSON.h"
#include "../QtUtil/lib/cjson/cJSON_inc.h"

AppSetting::AppSetting()
: _newMsgAudioNotify(true)
        , _newMsgTipWindowNotify(true)
        , _phoneAnyReceive(false)
        , _strongWarn(true)
        , _hotCutEnable(true)
        , _screentShotHideWndFlag(false)
        , _showMoodFlag(true)
        , _autoDeleteSession(true)
        , _leaveCheckEnable(false)
//        , _leaveMinute(5)
//        , _autoReplyPreset(true)
        , _autoReplyMsg("不在办公室")
        , _themeMode(1)
        , _fontLevel(FONT_LEVEL_NORMAL)
        , _autoStartUp(false)            // 开机自启动
        , _openOaWithAppBrowser(true)
        , _showSendMessageBtn(false)
        , _supportNativeMessagePrompt(false)
        , _useNativeMessagePrompt(false)
{

}

void AppSetting::initAppSetting(const std::string& setting)
{
	cJSON* obj = cJSON_Parse(setting.data());
	if (obj)
	{
        std::lock_guard<QTalk::util::spin_mutex> lock(sm);
		_newMsgAudioNotify = (bool)QTalk::JSON::cJSON_SafeGetBoolValue(obj, "NEWMSGAUDIO", _newMsgAudioNotify);
		_newMsgAudioPath = QTalk::JSON::cJSON_SafeGetStringValue(obj, "NEWMSGAUDIOPATH", _newMsgAudioPath.data());
		_newMsgTipWindowNotify = (bool)QTalk::JSON::cJSON_SafeGetBoolValue(obj, "NEWMSGTIPWINDOW", _newMsgTipWindowNotify);
		_screenshotKey = QTalk::JSON::cJSON_SafeGetStringValue(obj, "SCREENSHOT", _screenshotKey.data());
		_wakeWndKey = QTalk::JSON::cJSON_SafeGetStringValue(obj, "WAKEWND", _wakeWndKey.data());
		_openOaWithAppBrowser = (bool)QTalk::JSON::cJSON_SafeGetBoolValue(obj, "OPENOAWITHAPPBROWSER", _openOaWithAppBrowser);
        _screentShotHideWndFlag = (bool)QTalk::JSON::cJSON_SafeGetBoolValue(obj, "SCREENTSHOTHIDEWNDFLAG", _screentShotHideWndFlag);
        _showSendMessageBtn = (bool)QTalk::JSON::cJSON_SafeGetBoolValue(obj, "SHOWSENDMESSAGEBTN", _showSendMessageBtn);
        _strongWarn = (bool)QTalk::JSON::cJSON_SafeGetBoolValue(obj, "STRONGWARN", _strongWarn);
        _hotCutEnable = (bool)QTalk::JSON::cJSON_SafeGetBoolValue(obj, "HOTCUTENABLE", _hotCutEnable);
        _sendMessageKey = QTalk::JSON::cJSON_SafeGetStringValue(obj, "SENDMESSAGEKEY", "Enter");
        _showMoodFlag = (bool)QTalk::JSON::cJSON_SafeGetBoolValue(obj, "SHOWMOODFLAG", true);
        _autoStartUp = (bool)QTalk::JSON::cJSON_SafeGetBoolValue(obj, "AUTOSTARTUP", false);
        _leaveCheckEnable = (bool)QTalk::JSON::cJSON_SafeGetBoolValue(obj, "LEAVECHECKENABLE", false);
        _awaysAutoReply = (bool)QTalk::JSON::cJSON_SafeGetBoolValue(obj, "AWAYSAUTOREPLY", false);
        _autoReplyMsg = QTalk::JSON::cJSON_SafeGetStringValue(obj, "AUTOREPLYMSG");
//        _autoReplyCusMsg = QTalk::JSON::cJSON_SafeGetStringValue(obj, "AUTOREPLYCUSMSG");
        _leaveMinute = QTalk::JSON::cJSON_SafeGetIntValue(obj, "LEAVEMINUTE", 5);
        _useNativeMessagePrompt = QTalk::JSON::cJSON_SafeGetBoolValue(obj, "USENATIVEMESSAGEPROMPT", false);
        _autoDeleteSession = QTalk::JSON::cJSON_SafeGetBoolValue(obj, "AUTODELETESESSION", true);

        _autoReplyStartTime = QTalk::JSON::cJSON_SafeGetIntValue(obj, "AUTOREPLYSTARTTIME", 0);
        _autoReplyEndTime = QTalk::JSON::cJSON_SafeGetIntValue(obj, "AUTOREPLYENDTIME", 24);

		cJSON_Delete(obj);
	}
}

AppSetting & AppSetting::instance()
{
    static AppSetting appSetting;
	return appSetting;
	
}

// 消息通知
bool  AppSetting::getNewMsgAudioNotify() {
    std::lock_guard<QTalk::util::spin_mutex> lock(sm);
    return _newMsgAudioNotify;
} //新消息声音提醒

void AppSetting::setNewMsgAudioNotify(bool flag) {
    std::lock_guard<QTalk::util::spin_mutex> lock(sm);
    _newMsgAudioNotify = flag;
}

std::string AppSetting::getgetNewMsgAudioPath()
{
	return _newMsgAudioPath;
}

void AppSetting::setgetNewMsgAudioPath(const std::string & path)
{
	_newMsgAudioPath = path;
}

bool AppSetting::getNewMsgTipWindowNotify() {
    std::lock_guard<QTalk::util::spin_mutex> lock(sm);
    return _newMsgTipWindowNotify;
}    //新消息提示窗

void AppSetting::setNewMsgTipWindowNotify(bool flag) {
    std::lock_guard<QTalk::util::spin_mutex> lock(sm);
    _newMsgTipWindowNotify = flag;
}

bool AppSetting::getPhoneAnyReceive() {
    std::lock_guard<QTalk::util::spin_mutex> lock(sm);
    return _phoneAnyReceive;
}       //手机端随时接收推送

void AppSetting::setPhoneAnyReceive(bool flag) {
    std::lock_guard<QTalk::util::spin_mutex> lock(sm);
    _phoneAnyReceive = flag;
}

void AppSetting::setStrongWarnFlag(bool flag)
{
    std::lock_guard<QTalk::util::spin_mutex> lock(sm);
    _strongWarn = flag;
}

bool AppSetting::getStrongWarnFlag()
{
    std::lock_guard<QTalk::util::spin_mutex> lock(sm);
    return _strongWarn;
}

// 热键设置
bool AppSetting::getHotCutEnable() {
    std::lock_guard<QTalk::util::spin_mutex> lock(sm);
    return _hotCutEnable;
}          //热键是否开启

void AppSetting::setHotCutEnable(bool flag) {
    std::lock_guard<QTalk::util::spin_mutex> lock(sm);
    _hotCutEnable = flag;
}

std::string  AppSetting::getScreenshotHotKey() {
    std::lock_guard<QTalk::util::spin_mutex> lock(sm);
    return _screenshotKey;
}        //截图快捷键

void AppSetting::setScreenshotHotKey(const std::string& hotKey) {
    std::lock_guard<QTalk::util::spin_mutex> lock(sm);
    _screenshotKey = hotKey;
}

std::string  AppSetting::getWakeWndHotKey() {
    std::lock_guard<QTalk::util::spin_mutex> lock(sm);
    return _wakeWndKey;
}

void AppSetting::setWakeWndHotKey(const std::string& hotKey) {
    std::lock_guard<QTalk::util::spin_mutex> lock(sm);
    _wakeWndKey = hotKey;
}

// 自动回复
bool AppSetting::getAutoReplyEnable() {
    std::lock_guard<QTalk::util::spin_mutex> lock(sm);
    return _leaveCheckEnable;
}

void AppSetting::setAutoReplyEnable(bool flag) {
    std::lock_guard<QTalk::util::spin_mutex> lock(sm);
    _leaveCheckEnable = flag;
}

int AppSetting::getLeaveMinute() {
    std::lock_guard<QTalk::util::spin_mutex> lock(sm);
    return _leaveMinute;
}

void AppSetting::setLeaveMinute(int minute) {
    std::lock_guard<QTalk::util::spin_mutex> lock(sm);
    _leaveMinute = minute;
}

bool AppSetting::getAwaysAutoReply() {
    std::lock_guard<QTalk::util::spin_mutex> lock(sm);
    return _awaysAutoReply;
}

void AppSetting::setAwaysAutoReply(bool flag) {
    std::lock_guard<QTalk::util::spin_mutex> lock(sm);
    _awaysAutoReply = flag;
}

std::string AppSetting::getAutoReplyMsg() {
    std::lock_guard<QTalk::util::spin_mutex> lock(sm);
    return _autoReplyMsg;
}

void AppSetting::setAutoReplyMsg(std::string msg) {
    std::lock_guard<QTalk::util::spin_mutex> lock(sm);
    _autoReplyMsg = std::move(msg);
}

//std::string AppSetting::getAutoReplyCusMsg() {
//    std::lock_guard<QTalk::util::spin_mutex> lock(sm);
//    return _autoReplyCusMsg;
//}
//
//void AppSetting::setAutoReplyCusMsg(std::string msg) {
//    std::lock_guard<QTalk::util::spin_mutex> lock(sm);
//    _autoReplyCusMsg = std::move(msg);
//}

void AppSetting::setAutoReplyStartTime(int time)
{
    std::lock_guard<QTalk::util::spin_mutex> lock(sm);
    _autoReplyStartTime = time;
}

void AppSetting::setAutoReplyEndTime(int time)
{
    std::lock_guard<QTalk::util::spin_mutex> lock(sm);
    _autoReplyEndTime = time;
}

//
bool AppSetting::withinEnableTime(int hour)
{
    std::lock_guard<QTalk::util::spin_mutex> lock(sm);
    return hour >= _autoReplyStartTime && hour <= _autoReplyEndTime;
}

//
int AppSetting::getAutoReplyStartTime()
{
    std::lock_guard<QTalk::util::spin_mutex> lock(sm);
    return _autoReplyStartTime;
}

//
int AppSetting::getAutoReplyEndTime()
{
    std::lock_guard<QTalk::util::spin_mutex> lock(sm);
    return _autoReplyEndTime;
}

// 文件目录
std::string AppSetting::getUserDirectory() {
    std::lock_guard<QTalk::util::spin_mutex> lock(sm);
    return _userDirectory;
}

void AppSetting::setUserDirectory(const std::string& userDirectory) {
    std::lock_guard<QTalk::util::spin_mutex> lock(sm);
    _userDirectory = userDirectory;
}

std::string AppSetting::getFileSaveDirectory() {
    std::lock_guard<QTalk::util::spin_mutex> lock(sm);
    return _fileSaveDirectory;
}

void AppSetting::setFileSaveDirectory(const std::string& fileSaveDirectory) {
    std::lock_guard<QTalk::util::spin_mutex> lock(sm);
    _fileSaveDirectory = fileSaveDirectory;
}

// 皮肤设置
int AppSetting::getThemeMode() {
    std::lock_guard<QTalk::util::spin_mutex> lock(sm);
    return _themeMode;
}

void AppSetting::setThemeMode(int mode) {
    std::lock_guard<QTalk::util::spin_mutex> lock(sm);
    _themeMode = mode;
}

std::string AppSetting::getFont() {
    std::lock_guard<QTalk::util::spin_mutex> lock(sm);
    return _font;
}

void AppSetting::setFont(const std::string& font) {
    std::lock_guard<QTalk::util::spin_mutex> lock(sm);
    _font = font;
}

int AppSetting::getFontLevel() {
    std::lock_guard<QTalk::util::spin_mutex> lock(sm);
    return _fontLevel;
}

void AppSetting::setFontLevel(int level) {
    std::lock_guard<QTalk::util::spin_mutex> lock(sm);
    _fontLevel = level;
}

int AppSetting::getLanguage() {
    std::lock_guard<QTalk::util::spin_mutex> lock(sm);
    return _language;
}

void AppSetting::setLanguage(int language) {
    std::lock_guard<QTalk::util::spin_mutex> lock(sm);
    _language = language;
}

// 其他设置
bool AppSetting::getSelfStart() {
    std::lock_guard<QTalk::util::spin_mutex> lock(sm);
    return _autoStartUp;
}

// 开机自启动
void AppSetting::setSelfStart(bool flag) {
    std::lock_guard<QTalk::util::spin_mutex> lock(sm);
    _autoStartUp = flag;
}

bool AppSetting::getAutoLoginEnable() {
    std::lock_guard<QTalk::util::spin_mutex> lock(sm);
    return _autoLoginEnable;
}            // 是否自动登录

void AppSetting::setAutoLoginEnable(bool flag) {
    std::lock_guard<QTalk::util::spin_mutex> lock(sm);
    _autoLoginEnable = flag;
}


bool AppSetting::getOpenOaLinkWithAppBrowser() {
    std::lock_guard<QTalk::util::spin_mutex> lock(sm);
    return _openOaWithAppBrowser;
}

void AppSetting::setOpenOaLinkWithAppBrowser(bool flag) {
    std::lock_guard<QTalk::util::spin_mutex> lock(sm);
    _openOaWithAppBrowser = flag;
}

void AppSetting::setScreentShotHideWndFlag(bool hide) {
    std::lock_guard<QTalk::util::spin_mutex> lock(sm);
    _screentShotHideWndFlag = hide;
}

bool AppSetting::getScreentShotHideWndFlag() {
    std::lock_guard<QTalk::util::spin_mutex> lock(sm);
    return _screentShotHideWndFlag;
}

void AppSetting::setShowSendMessageBtnFlag(bool flag) {
    std::lock_guard<QTalk::util::spin_mutex> lock(sm);
    _showSendMessageBtn = flag;
}

bool AppSetting::getShowSendMessageBtnFlag() {
    std::lock_guard<QTalk::util::spin_mutex> lock(sm);
    return _showSendMessageBtn;
}

void AppSetting::setSendMessageKey(std::string key)
{
    std::lock_guard<QTalk::util::spin_mutex> lock(sm);
    _sendMessageKey = std::move(key);
}

std::string AppSetting::getSendMessageKey()
{
    std::lock_guard<QTalk::util::spin_mutex> lock(sm);
    return _sendMessageKey;
}

void AppSetting::setShowMoodFlag(bool flag)
{
    std::lock_guard<QTalk::util::spin_mutex> lock(sm);
    _showMoodFlag = flag;
}

bool AppSetting::getShowMoodFlag() {
    std::lock_guard<QTalk::util::spin_mutex> lock(sm);
    return _showMoodFlag;
}

void AppSetting::setAutoDeleteSession(bool autoDelete)
{
    std::lock_guard<QTalk::util::spin_mutex> lock(sm);
    _autoDeleteSession = autoDelete;
}

bool AppSetting::getAutoDeleteSession()
{
    std::lock_guard<QTalk::util::spin_mutex> lock(sm);
    return _autoDeleteSession;
}

void AppSetting::setNewVersion(int ver) {
    std::lock_guard<QTalk::util::spin_mutex> lock(sm);
    _newVersion = ver;
}


int AppSetting::getNewVersion() {
    std::lock_guard<QTalk::util::spin_mutex> lock(sm);
    return _newVersion ;
}

//std::string AppSetting::getCoEdit() {
//    std::lock_guard<QTalk::util::spin_mutex> lock(sm);
//    return _coEditor;
//}
//
//void AppSetting::setCoEdit(const std::string& edit) {
//    std::lock_guard<QTalk::util::spin_mutex> lock(sm);
//    _coEditor = edit;
//}

/**
 *
 * @return
 */
std::string AppSetting::saveAppSetting() {

    std::string ret;
    std::lock_guard<QTalk::util::spin_mutex> lock(sm);
	cJSON* obj = cJSON_CreateObject();
	cJSON_AddBoolToObject(obj, "NEWMSGAUDIO", _newMsgAudioNotify);
	cJSON_AddStringToObject(obj, "NEWMSGAUDIOPATH", _newMsgAudioPath.data());
	cJSON_AddBoolToObject(obj, "NEWMSGTIPWINDOW", _newMsgTipWindowNotify);
	cJSON_AddStringToObject(obj, "SCREENSHOT", _screenshotKey.data());
	cJSON_AddStringToObject(obj, "WAKEWND", _wakeWndKey.data());
	cJSON_AddBoolToObject(obj, "OPENOAWITHAPPBROWSER", _openOaWithAppBrowser);
	cJSON_AddBoolToObject(obj, "SCREENTSHOTHIDEWNDFLAG", _screentShotHideWndFlag);
	cJSON_AddBoolToObject(obj, "SHOWSENDMESSAGEBTN", _showSendMessageBtn);
	cJSON_AddBoolToObject(obj, "STRONGWARN", _strongWarn);
	cJSON_AddBoolToObject(obj, "HOTCUTENABLE", _hotCutEnable);
    cJSON_AddStringToObject(obj, "SENDMESSAGEKEY", _sendMessageKey.data());
    cJSON_AddBoolToObject(obj, "SHOWMOODFLAG", _showMoodFlag);
    cJSON_AddBoolToObject(obj, "AUTOSTARTUP", _autoStartUp);
    cJSON_AddBoolToObject(obj, "LEAVECHECKENABLE", _leaveCheckEnable);
    cJSON_AddBoolToObject(obj, "AWAYSAUTOREPLY", _awaysAutoReply);
    cJSON_AddStringToObject(obj, "AUTOREPLYMSG", _autoReplyMsg.data());
//    cJSON_AddStringToObject(obj, "AUTOREPLYCUSMSG", _autoReplyCusMsg.data());
    cJSON_AddNumberToObject(obj, "LEAVEMINUTE", _leaveMinute);
    cJSON_AddBoolToObject(obj, "USENATIVEMESSAGEPROMPT", _useNativeMessagePrompt);
    cJSON_AddBoolToObject(obj, "AUTODELETESESSION", _autoDeleteSession);

    cJSON_AddNumberToObject(obj, "AUTOREPLYSTARTTIME", _autoReplyStartTime);
    cJSON_AddNumberToObject(obj, "AUTOREPLYENDTIME", _autoReplyEndTime);

	ret = QTalk::JSON::cJSON_to_string(obj);
	cJSON_Delete(obj);
    return ret;
}