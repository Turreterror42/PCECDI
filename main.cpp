#include <QApplication>
#include <QSplashScreen>
#include <QPixmap>
#include <QProcess>
#include <QTranslator>
#include <QDir>
#include <QScreen>
#include <QMessageBox>
#include <QPainter>
#include <QStandardPaths>
#include <QDomElement>
#include <QStringList>
#include <QSize>
#include <QThread>
#include <QTimer>
#include <QFontDatabase>
#include <QGuiApplication>
#include "MainUI.h"
#include "SettingsManager.h"
#include "CheckSums.h"

const QString CSVName = "CSVFiles";
const QString SettingsName = "Settings";
const QString BackupName = "Sauvegarde";
const QString progVersion = "9.5";
const QString VersionInfoText = QString("PCECDI_v%1 - Codex").arg(progVersion);
const QString DefaultLang = "fr";
const bool DefaultBackup = true;
const QString DefaultTimeFormat = "%Y %m";
const QStringList DefaultChoices = {"Club", "Travailler sur les postes", "Dessiner", "Apprendre", "Lire", "Faire un exposé", "Voir des exposés", "Jouer au jeux de sociétés", "Jouer de la musique", "Faire ses devoirs", "Se connecter à l'ENT", "Jouer à des escape games", "Faire des recherches", "Aider des élèves", "Venir en tant que délégué CDI", "Faire du théâtre", "Aider Mme Noiret", "Exclusion", "Dormir", "S'informer", "Retard", "Rendez-vous médical", "Heure de colle"};
const QStringList DefaultGrades = {"6e", "5e", "4e", "3e"};
const QStringList DefaultGradesLetter = {"A", "B", "C", "D", "E", "F", "G", "H", "I"};
QSize screenSize;
SettingsManager SettingsRead("PCECDI Inc.", "PCECDI");

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    screenSize = QGuiApplication::primaryScreen()->size();

#ifdef Q_OS_WIN
    QApplication::setStyle("Vista");
#elif defined(Q_OS_LINUX)
    QApplication::setStyle("Fusion");
#elif defined(Q_OS_MAC)
    QApplication::setStyle("macOS");
#endif

    QString jsonFilePath = ":/Files/checksums.json";
    QJsonObject jsonChecksums = readChecksumsFromJson(jsonFilePath);
    if (jsonChecksums.isEmpty()) {
        qFatal("[PCECDI] [FATAL ERROR] Could not load checksums.");
        return 1;
    }

    int fileCount = 0;

    if (!compareChecksums(jsonChecksums, QApplication::applicationDirPath(), fileCount)) {
        qFatal("[PCECDI] [FATAL ERROR] Checksums do not match.");
        return 1;
    }

    qInfo() << "[Checksums] [INFO]" << fileCount << "checksums checked, OK!";
    qInfo() << "[Checksums] [INFO] All checksums match.";

    QString Language = SettingsRead.valueAsString("language", DefaultLang);

    QTranslator translator;
    if (translator.load(QLocale(Language), "PCECDI", "_", "translations") && Language != "en") QApplication::installTranslator(&translator);

    MainWindow MainUI;
    MainUI.settings.ResetComboBoxLanguage();

    QImage image(":/Images/SplashScreen.png");
    if (image.isNull()) {
        qFatal("[PCECDI] [FATAL ERROR] Could not load splash screen image.");
        return 1;
    }

    QPainter painter(&image);
    QFont font(QString(QFontDatabase::applicationFontFamilies(QFontDatabase::addApplicationFont(":/Fonts/Lobster-Regular.ttf")).at(0)), 20);
    font.setItalic(1);
    painter.setFont(font);
    painter.setPen(QPen(Qt::white));
    painter.drawText(10, 27, QApplication::tr("A Small Program for the CDI !"));
    painter.drawText(10, 310, QApplication::tr("By Turreterror42"));
    painter.drawText(600, 310, QString("v%1").arg(progVersion));

    QSplashScreen Splash;
    Splash.setPixmap(QPixmap::fromImage(image));
    Splash.show();

    QTimer::singleShot(3000, &Splash, [&]() {
        Splash.finish(&MainUI);
        MainUI.showMaximized();
        if (MainUI.New) MainUI.Welcome();
    });

    return app.exec();
}

