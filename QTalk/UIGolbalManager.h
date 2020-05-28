#ifndef UIGOLBALMANAGER_H
#define UIGOLBALMANAGER_H
#include "../UICom/Config/configobject.h"
#include "pluginmanager.h"
#include "stylesheetmanager.h"
#include "../QtUtil/lib/ini/ConfigLoader.h"
#include "../Platform/AppSetting.h"

class UIGolbalManager : public ConfigObject
{
    Q_OBJECT
public:
    ~UIGolbalManager() override;

public:
    static UIGolbalManager *getUIGolbalManager();//获取单例对象指针的静态方法
    static QJsonDocument loadJsonConfig(const QString &path);

    QObject *getPluginInstanceQt(const QString &key)const;
    bool UnloadPluginQt(const QString &key);
    std::shared_ptr<QMap<QString, QObject *> > getAllPluginInstanceQt() const;
    void init();
    void InitPluginManager();
    void InitStyleManager();

    void setStyleSheetAll();
    void setStylesForApp();
    void setStyleSheetForPlugin(const QString& plgName);

    void initThemeConfig();

public slots:
    void saveSysConfig();

private:
    UIGolbalManager();

Q_SIGNALS:
    void sgMousePressGlobalPos(const QPoint & pos);

private:
    static UIGolbalManager *_pInstance; //对象指针为单例指针类保证了只有一个对象

    QString _ConfigDataDir; // 配置文件路径
    static const QString DEFAULT_PluginManagerPath;
    static const QString DEFAULT_StyleManagerPath;
    static const QString DEFAULT_PluginPath;

    PluginManager * _pluginManager;
    StyleSheetManager * _pstyleSheetManager;

private:
    QTalk::ConfigLoader *_pSystemConfig;

    int _theme = 1;
    std::string _font;
    int _font_level;

public:
    bool _check_updater = true;
    int  _updater_version = 0;
};

#endif // UIGOLBALMANAGER_H
