#ifndef SETTINGS_MANAGER_H
#define SETTINGS_MANAGER_H

#include <QSettings>
#include <QString>
#include <QDir>
#include <QDebug>
#include <QFile>

extern const QString DefaultLang;
extern const bool DefaultBackup;
extern const QString DefaultTimeFormat;
extern const QStringList DefaultChoices;
extern const QStringList DefaultGrades;
extern const QStringList DefaultGradesLetter;
extern const QString progVersion;

class SettingsManager
{
public:
    SettingsManager(const QString& organization, const QString& application) : m_settings(QSettings::IniFormat, QSettings::UserScope, organization, application)
    {
        initializeDefaultSettings();
        qInfo() << "[Settings] [INFO] SettingsManager initialized.";
    }

    void setValue(const QString& key, const QVariant& value) {
        m_settings.setValue(key, value);

        QString displayValue;
        if (value.typeId() == QMetaType::QStringList) displayValue = value.toStringList().join(", ");
        else displayValue = value.toString();

        qInfo() << "[Settings] [INFO] Set value for key :" << key << "to" << displayValue;
    }


    QString valueAsString(const QString& key, const QString& defaultValue = QString()) const {
        return m_settings.value(key, defaultValue).toString();
    }

    QStringList valueAsStringList(const QString& key, const QStringList& defaultValue = QStringList()) const {
        return m_settings.value(key, defaultValue).toStringList();
    }

    void addChoice(const QString& newChoice, const QString& valueName) {
        QStringList choices = valueAsStringList(valueName);
        if (!choices.contains(newChoice)) {
            choices.append(newChoice);
            setValue(valueName, choices);
            qInfo() << "[Settings] [INFO] Added choice :" << newChoice;
        } else {
            qWarning() << "[Settings] [WARNING] Choice already exists :" << newChoice;
        }
    }

    bool valueAsBool(const QString& key, bool defaultValue) const {
        return m_settings.value(key, defaultValue).toBool();
    }

    bool tagExists(const QString& tag) {
        return m_settings.contains(tag);
    }

    bool isFirstLaunch() {
        if(!QFile(m_settings.fileName()).exists()) return true;
        else return false;
    }

    void removeValue(const QString& key) {
        m_settings.remove(key);
        qInfo() << "[Settings] [INFO] Removed value for key :" << key;
    }

    void clear() {
        m_settings.clear();
        qInfo() << "[Settings] [INFO] All settings cleared.";
    }

    bool createDirIfDontExist(const QString& folder) {
        QDir dir;
        if(dir.mkpath(folder)) return true;
        else return false;
    }

    ~SettingsManager() {
        qInfo() << "[Settings] [INFO] SettingsManager destroyed.";
    }

private:
    void initializeDefaultSettings() {
        if (valueAsString("version", progVersion) != progVersion) {
            qInfo() << "[Settings] [INFO] Initializing the settings file ...";

            clear();
            setValue("version", progVersion);
            setValue("language", DefaultLang);
            setValue("backup", DefaultBackup);
            setValue("timeFormat", DefaultTimeFormat);
            setValue("choices", DefaultChoices);
            setValue("grades", DefaultGrades);
            setValue("gradesLetter", DefaultGradesLetter);

            qInfo() << "[Settings] [INFO] Default settings initialized.";
        }
    }

    QSettings m_settings;
};

#endif // SETTINGS_MANAGER_H
