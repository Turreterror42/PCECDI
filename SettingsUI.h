#ifndef MAINSETTINGS_H
#define MAINSETTINGS_H

#include <QWidget>
#include <QApplication>
#include <QTabWidget>
#include <QPushButton>
#include <QLabel>
#include <QSize>
#include <QPixmap>
#include <QTextEdit>
#include <QGroupBox>
#include <QProcess>
#include <QInputDialog>
#include <QTranslator>
#include <QDesktopServices>
#include <QCheckBox>
#include <QLineEdit>
#include <QSettings>
#include <QMessageBox>
#include <QStringList>
#include <QVBoxLayout>
#include <QScreen>
#include <QHBoxLayout>
#include <QDir>
#include <QComboBox>
#include <QRect>
#include "SettingsManager.h"

extern SettingsManager SettingsRead;
extern const QString CSVName;
extern const QString SettingsName;
extern const QString BackupName;
extern const QString VersionInfoText;
extern const QString DefaultLang;
extern const bool DefaultBackup;
extern const QString DefaultTimeFormat;
extern const QStringList DefaultChoices;
extern const QStringList DefaultGrades;
extern const QStringList DefaultGradesLetter;

class SettingsUI : public QWidget
{
    Q_OBJECT

public:
    SettingsUI()
    {
        setupUI();
        connectButtons();
        qInfo() << "[SettingsUI] [INFO] SettingsUI initialized successfully.";
    }
    ~SettingsUI()
    {
        qInfo() << "[SettingsUI] [INFO] SettingsUI destroyed.";
    }

    void ResetFieldsSettings()
    {
        TextEdit2->setText(QDir::currentPath() + "/" + CSVName);
        tabWidget->setCurrentIndex(0);
        fileNameTime->setText(SettingsRead.valueAsString("timeFormat", DefaultTimeFormat));
        qInfo() << "[SettingsUI] [INFO] Fields in SettingsUI reset.";
    }

    void ResetComboBoxLanguage()
    {
        LanguageCombo->clear();
        LanguageCombo->addItem("Français");
        LanguageCombo->addItem("English");
        LanguageCombo->addItem("Deutsch");
        LanguageCombo->addItem("Русский");
        qInfo() << "[SettingsUI] [INFO] Language combo box reset with available languages.";

        QString Lang = SettingsRead.valueAsString("language", DefaultLang);
        QStringList languages = {"fr", "en", "de", "ru"};
        int index = languages.indexOf(Lang);
        if (index != -1) LanguageCombo->setCurrentIndex(index);
        qInfo() << "[SettingsUI] [INFO] Language Combo Box set to :" << Lang;
    }

    QComboBox *LanguageCombo;

private:
    QWidget *GeneralTab;
    QWidget *AboutTab;
    QWidget *SettingsTab;
    QTabWidget *tabWidget;
    QLabel *CreditsShort;
    QLabel *CSVPath;
    QLabel *TitleandIcon;
    QLabel *Credits;
    QLabel *Language;
    QCheckBox *BackupAdd;
    QPushButton *OKButton;
    QPushButton *RestartButton;
    QPushButton *AboutQt;
    QTextEdit *TextEdit2;
    QLabel *VersionInfo;
    QLabel *timeFormatLabel;
    QPushButton *OpenDirCSVButton;
    QPushButton *OpenDirExeButton;
    QPushButton *AddChoiceButton;
    QPushButton *OpenDirBackupButton;
    QPushButton *ApplyfileTime;
    QLineEdit *fileNameTime;

    void setupUI()
    {
        QVBoxLayout *settingsLayout = new QVBoxLayout;
        QVBoxLayout *settingsGeneralLayout = new QVBoxLayout;
        QVBoxLayout *settingsAboutLayout = new QVBoxLayout;
        QHBoxLayout *settingsLayoutH = new QHBoxLayout;
        QHBoxLayout *settingsGeneralLayoutH = new QHBoxLayout;
        QHBoxLayout *settingsGeneralLayoutH2 = new QHBoxLayout;
        QHBoxLayout *settingsGeneralLayoutH3 = new QHBoxLayout;
        QHBoxLayout *settingsGeneralLayoutH4 = new QHBoxLayout;

        TextEdit2 = new QTextEdit(this);
        CSVPath = new QLabel(tr("CSV files folder\npath:"), this);
        Language = new QLabel(tr("Language :"), this);
        BackupAdd = new QCheckBox(tr("Backup"), this);
        timeFormatLabel = new QLabel(tr("CSV file name :"), this);

        if(SettingsRead.valueAsBool("settings", DefaultBackup)) BackupAdd->setChecked(1);
        else BackupAdd->setChecked(0);

        LanguageCombo = new QComboBox(this);
        LanguageCombo->setFixedWidth(width()/2.5);
        TextEdit2->setFixedSize(width()/2.5, height()/4);
        ResetComboBoxLanguage();

        OpenDirCSVButton = new QPushButton(tr("Open CSV files folder"), this);
        OpenDirBackupButton = new QPushButton(tr("Open backups folder"), this);
        AddChoiceButton = new QPushButton(tr("Add a choice"), this);
        OpenDirExeButton = new QPushButton(tr("Open the executable folder"), this);
        ApplyfileTime = new QPushButton(tr("Apply"), this);
        VersionInfo = new QLabel(VersionInfoText, this);
        fileNameTime = new QLineEdit(this);

        AboutQt = new QPushButton(tr("About Qt"), this);
        TitleandIcon = new QLabel(this);
        Credits = new QLabel(tr("This program was specifically developed for the\nDocumentation and Information Center (CDI) by Turreterror42.\nThe background image is sourced from the royalty-free\nplatform: https://pxhere.com/en/photo/1593135.\n\nThis software was built using the Qt framework,\nan open-source technology for creating\ncross-platform graphical user interfaces.\n"), this);
        Credits->setAlignment(Qt::AlignCenter);
        TitleandIcon->setPixmap(QPixmap(":/Images/TitleWithIcon.png"));
        TitleandIcon->setFixedSize(322, 62);
        AboutTab = new QWidget;
        GeneralTab = new QWidget;
        OKButton = new QPushButton("OK", this);
        OKButton->setFixedWidth(75);
        RestartButton = new QPushButton(tr("Cancel"), this);
        RestartButton->setFixedWidth(75);
        CreditsShort = new QLabel(tr("Turreterror42"), this);
        tabWidget = new QTabWidget(this);
        tabWidget->addTab(GeneralTab, tr("General"));
        tabWidget->addTab(AboutTab, tr("About"));
        TextEdit2->setReadOnly(1);
        setLayout(settingsLayout);
        settingsLayout->addWidget(tabWidget);
        settingsLayout->addLayout(settingsLayoutH);
        settingsLayoutH->addWidget(CreditsShort);
        settingsLayoutH->addWidget(OKButton);
        settingsLayoutH->addWidget(RestartButton);

        GeneralTab->setLayout(settingsGeneralLayout);
        AboutTab->setLayout(settingsAboutLayout);
        settingsAboutLayout->addWidget(TitleandIcon, 0, Qt::AlignHCenter);
        settingsAboutLayout->addWidget(Credits);
        settingsAboutLayout->addWidget(AboutQt);
        settingsGeneralLayout->addWidget(VersionInfo, 0, Qt::AlignHCenter);
        settingsGeneralLayout->addLayout(settingsGeneralLayoutH);
        settingsGeneralLayout->addLayout(settingsGeneralLayoutH2);
        settingsGeneralLayout->addWidget(OpenDirCSVButton);
        settingsGeneralLayout->addWidget(OpenDirExeButton);
        settingsGeneralLayout->addWidget(AddChoiceButton);
        settingsGeneralLayout->addLayout(settingsGeneralLayoutH3);
        settingsGeneralLayout->addLayout(settingsGeneralLayoutH4);
        settingsGeneralLayoutH3->addWidget(timeFormatLabel);
        settingsGeneralLayoutH3->addWidget(fileNameTime);
        settingsGeneralLayoutH3->addWidget(ApplyfileTime);
        settingsGeneralLayoutH4->addWidget(BackupAdd);
        settingsGeneralLayoutH4->addWidget(OpenDirBackupButton);
        settingsGeneralLayoutH->addWidget(CSVPath);
        settingsGeneralLayoutH->addWidget(TextEdit2);
        settingsGeneralLayoutH2->addWidget(Language);
        settingsGeneralLayoutH2->addWidget(LanguageCombo);

        setWindowIcon(QIcon(":/Images/ICON.ico"));
        setWindowTitle(tr("Tools"));
        setFixedSize(sizeHint());
    }

    void connectButtons()
    {
        QObject::connect(OKButton, &QPushButton::clicked, this, &QWidget::close);
        QObject::connect(RestartButton, &QPushButton::clicked, this, &QWidget::close);
        QObject::connect(OpenDirCSVButton, &QPushButton::clicked, this, [this](){ OpenFolder(QString("%1").arg(CSVName)); });
        QObject::connect(OpenDirExeButton, &QPushButton::clicked, this, [this](){ OpenFolder(QDir::currentPath()); });
        QObject::connect(OpenDirBackupButton, &QPushButton::clicked, this, [this](){ OpenFolder(QString("%1").arg(BackupName)); });
        QObject::connect(AddChoiceButton, &QPushButton::clicked, this, &SettingsUI::AddNewChoice);
        QObject::connect(AboutQt, &QPushButton::clicked, qApp, &QApplication::aboutQt);
        QObject::connect(LanguageCombo, QOverload<int>::of(&QComboBox::activated), this, &SettingsUI::ChangeLanguage);
        QObject::connect(BackupAdd, &QCheckBox::checkStateChanged, this, &SettingsUI::checkStateChanged);
        QObject::connect(ApplyfileTime, &QPushButton::clicked, this, &SettingsUI::setTimeFormat);
    }

private slots:
    void setTimeFormat() {
        QString text = fileNameTime->text();
        if(SettingsRead.valueAsString("timeFormat", DefaultTimeFormat) != text) {
            SettingsRead.setValue("timeFormat", text);
            askForRestart();
        }
        qInfo() << "[SettingsUI] [INFO] Set time format to :" << text;
    }

    void ChangeLanguage() {
        QStringList languages = {"fr", "en", "de", "ru"};
        int index = LanguageCombo->currentIndex();
        if (index >= 0 && index < languages.size()) SettingsRead.setValue("language", languages[index]);
        askForRestart();
    }

    void checkStateChanged()
    {
        if(BackupAdd->isChecked()) {
            SettingsRead.setValue("backup", true);
            qInfo() << "[SettingsUI] [INFO] Backup option enabled.";
        }
        else {
            SettingsRead.setValue("backup", false);
            qInfo() << "[SettingsUI] [INFO] Backup option disabled.";
        }

        askForRestart();
    }

    void AddNewChoice()
    {
        bool ok;
        SettingsRead.addChoice(QString("%1").arg(QInputDialog::getText(this, tr("Add a choice"), tr("Enter the choice you want to add :"), QLineEdit::Normal, "", &ok).remove('\n').remove(',').remove(';')), "choices");
        if(ok) askForRestart();
    }

    void askForRestart()
    {
        QProcess process;
        QMessageBox msgBox(this);
        msgBox.setWindowTitle("PCECDI");
        msgBox.setText(tr("You have changed the PCECDI settings!\nDo you want to restart the program to apply the changes ?"));
        msgBox.setIcon(QMessageBox::Question);
        QPushButton *AcceptButton = msgBox.addButton("OK", QMessageBox::AcceptRole);
        msgBox.addButton(tr("Cancel"), QMessageBox::RejectRole);

        msgBox.exec();

        if (msgBox.clickedButton() == AcceptButton)
        {
            qApp->quit();
#ifdef Q_OS_WIN
            process.startDetached(QFileInfo(QApplication::applicationFilePath()).fileName());
#elif defined(Q_OS_LINUX)
            process.startDetached("./" + QFileInfo(QApplication::applicationFilePath()).fileName());
#endif
            process.waitForFinished(-1);
            qInfo() << "[PCECDI] [INFO] Program restarted to apply settings.";
        }
        else
        {
            qInfo() << "[PCECDI] [INFO] Restart canceled by user.";
        }
    }

    void OpenFolder(const QString& folderName)
    {
        QString folderPath = folderName;
        if (!QDesktopServices::openUrl(QUrl::fromLocalFile(folderPath)))
        {
            SettingsRead.createDirIfDontExist(folderName);
            QDesktopServices::openUrl(QUrl::fromLocalFile(folderPath));
        }
        qInfo() << "[SettingsUI] [INFO] Folder opened :" << folderName;
    }
};
#endif // MAINSETTINGS_H
