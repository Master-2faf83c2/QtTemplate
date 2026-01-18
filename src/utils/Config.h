#ifndef _UTIL_CONFIG_H
#define _UTIL_CONFIG_H

#include "Singleton.h"

#include <QString>
#include <QVector>
#include <QJsonObject>
#include <QMap>
#include <QApplication>

struct ParamBinding{
    QString jsonKey;                    // JSON里的key
    QWidget* widget;                    // 对应控件
    std::function<QVariant()> get;      // 从UI取值
    std::function<void(QVariant)> set;  // 设置UI
};

class CoreConfig : public Singleton<CoreConfig>{
public:
    CoreConfig();
    ~CoreConfig();
    //目录创建
    void directoryInit();
    //写入当前温度数据
    void writeCurentTemperatureValue(double value);
    //写Info的JSON文件
    void writeInfoJson();
    //加载Info的JSON文件
    void loadFromInfoJson();
    //写Base的JSON文件
    void writeBaseJson();
    //加载Base的JSON文件
    void loadFromBaseJson();
    //加载翻译文件
    void loadTranslator(QApplication &a);
    //写入警告数据
    bool writeAlarmData(double value, double max, double min);
    //获取帮助手册路径
    QString getHelpManualPath();
    //获取温度文件夹路径
    QString getTemperatureRecordsPath();
    //获取临时文件夹路径
    QString getTempFilesPath();
    //获取警告路径
    QString getAlarmFolderPath();
    //获取设备id
    QString getDeviceId();
    //获取Info ParamTable
    QMap<QString, ParamBinding>& getInfoParamTable();
    //获取Base ParamTable
    QMap<QString, ParamBinding>& getBaseParamTable();
protected:
    void writeJson(const QString &key, QMap<QString, ParamBinding> &param);
    void loadFromJson(const QString &key, QMap<QString, ParamBinding> &param);

private:
    bool m_is_current_file_init;

    QString m_file_path;
    QString m_help_path;

    QMap<QString, QString> m_dirs;
    QMap<QString, ParamBinding> m_info_param_table;
    QMap<QString, ParamBinding> m_base_param_table;
};

#define G_Config    CoreConfig::getInstance()

#endif
