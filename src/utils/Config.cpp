#include "Config.h"

#include "Utils.h"

#include <QFile>
#include <QDir>
#include <QJsonDocument>
#include <QJsonArray>
#include <QSettings>
#include <QCoreApplication>
#include <QTranslator>

CoreConfig::CoreConfig()
    : m_is_current_file_init(true)
{
    QDir exe_path(QCoreApplication::applicationDirPath());
    m_file_path = exe_path.filePath("config/info.json");
    m_help_path = exe_path.filePath("config/help");

    qInfo() << "配置文件路径: " << m_file_path;
    qInfo() << "帮助文件路径: " << m_help_path;
}

CoreConfig::~CoreConfig(){

}

void CoreConfig::directoryInit(){
    auto obj = loadJsonParameter(m_file_path);
    auto info_dir = obj["Dir"].toObject();
    
    m_dirs["Data"] = info_dir["Data"].toString();
    m_dirs["TemperatureRecords"] = info_dir["TemperatureRecords"].toString();
    m_dirs["TempFiles"] = info_dir["TempFiles"].toString();
    m_dirs["Alarm"] = info_dir["Alarm"].toString();

    for (auto key : m_dirs.keys()) {
        if (key != "Data"){
            QDir dir(m_dirs["Data"]);
            m_dirs[key] = dir.filePath(m_dirs[key]);
        }
        QDir dir(m_dirs[key]);
        if (!dir.exists()){
            dir.mkpath(".");
            qDebug() << m_dirs[key] << "目录不存在，创建目录";
        }
    }
}

void CoreConfig::writeCurentTemperatureValue(double value){
    QString current_path = QDir(getTempFilesPath()).filePath("current_temperature.csv");
    QFile current_file(current_path);

    QIODevice::OpenMode open_mode = QIODevice::WriteOnly | QIODevice::Text;
    if (!m_is_current_file_init){
        open_mode |= QIODevice::Append;
    }else {
        m_is_current_file_init = false;
    }

    if (!current_file.open(open_mode)) {
        qDebug() << "无法打开文件:" << current_file.errorString();
        return;
    }

    auto current_time = getCurrentTimeMsString(":");

    QTextStream out(&current_file);

    out << current_time << "," << value << "\n";

    current_file.close();
}

void CoreConfig::writeInfoJson(){
    writeJson("Info", m_info_param_table);
}

void CoreConfig::loadFromInfoJson(){
    loadFromJson("Info", m_info_param_table);
}

void CoreConfig::writeBaseJson(){
    writeJson("Base", m_base_param_table);
}

void CoreConfig::loadFromBaseJson(){
    loadFromJson("Base", m_base_param_table);
}

void CoreConfig::loadTranslator(QApplication &a){
    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "app_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }
}

bool CoreConfig::writeAlarmData(double value, double max, double min){
    QString c_time = getCurrentDateString();
    QString c_path = QDir(getAlarmFolderPath()).filePath("alarm-" + c_time + ".csv");
    QFile c_file(c_path);
    QString alarm_str = "";
    bool res = false;

    if (value >= max){
        alarm_str = "高温警告";
        res = true;
        
    }else {
        alarm_str = "低温警告";
        res = false;
    }

    if (!c_file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append)) {
        qDebug() << "无法打开文件:" << c_file.errorString();
        return res;
    }

    QTextStream out(&c_file);

    out << c_time << "," << value <<  "," << alarm_str << "," << max << "," << min << "\n";

    c_file.close();

    return res;
}

QMap<QString, ParamBinding>& CoreConfig::getInfoParamTable(){
    return m_info_param_table;
}

QMap<QString, ParamBinding>& CoreConfig::getBaseParamTable(){
    return m_base_param_table;
}

QString CoreConfig::getHelpManualPath(){
    return m_help_path;
}

QString CoreConfig::getTemperatureRecordsPath(){
    return m_dirs["TemperatureRecords"];
}

QString CoreConfig::getTempFilesPath(){
    return m_dirs["TempFiles"];
}

QString CoreConfig::getAlarmFolderPath(){
    return m_dirs["Alarm"];
}

QString CoreConfig::getDeviceId(){
    return m_info_param_table["DeviceId"].get().toString();
}

void CoreConfig::writeJson(const QString &key, QMap<QString, ParamBinding> &param){
    auto obj = loadJsonParameter(m_file_path);
    auto key_obj = obj[key].toObject();

    for (auto it = param.begin(); it != param.end(); ++it) { 
        QVariant v = it.value().get();
        key_obj[it.value().jsonKey] = QJsonValue::fromVariant(v);
    }

    obj[key] = key_obj;
    
    QJsonDocument doc(obj);

    QFile file(m_file_path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Cannot open file for writing:" << m_file_path;
        return;
    }

    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();

    qDebug() << "配置已写入:" << m_file_path;
}

void CoreConfig::loadFromJson(const QString &key, QMap<QString, ParamBinding> &param){
    auto obj = loadJsonParameter(m_file_path);
    auto key_obj = obj[key].toObject();

    qDebug() << "导入json配置文件" << m_file_path << ":";
    for (auto it = param.begin(); it != param.end(); ++it) { 
        if (key_obj.contains(it.value().jsonKey)){
            it.value().set(key_obj[it.value().jsonKey].toVariant());
            qDebug() << it.value().jsonKey << ": " << key_obj[it.value().jsonKey].toVariant();
        }
    }
}

