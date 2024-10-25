#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QApplication>
#include <QTextEdit>
#include <QMessageBox>
#include <QTextStream>
#include <QFile>
#include <QSystemTrayIcon>
#include <QTimeZone>
#include <QDir>
#include <QDate>
#include <QInputDialog>
#include <QIcon>
#include <QTime>
#include <QDateTime>
#include <QPalette>
#include <QShortcut>
#include <QStatusBar>
#include <QByteArray>
#include <QCheckBox>
#include <QStringList>
#include <QComboBox>
#include <QPainter>
#include "SettingsUI.h"
#include "SettingsManager.h"

extern SettingsManager SettingsRead;
extern QSize screenSize;
extern const QString CSVName;
extern const QString SettingsName;
extern const QString BackupName;
extern const QString DefaultLang;
extern const bool DefaultBackup;
extern const QString DefaultTimeFormat;
extern const QStringList DefaultChoices;
extern const QStringList DefaultGrades;
extern const QStringList DefaultGradesLetter;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow()
    {
        qInfo() << "[PCECDI] [INFO] The Program is starting!";
        setupUI();
        connectButtons();
        qInfo() << "[PCECDI] [INFO] The program has started!";
    }
    ~MainWindow()
    {
        if(SettingsRead.valueAsBool("backup", DefaultBackup))
        {
            QString hel = getCSVFileName();
            copierFichier(true, QString("%1/%2").arg(CSVName, hel), QString("%1/%2").arg(BackupName, hel));
        }

        qInfo() << "[PCECDI] [INFO] Application quitting.";
        qInfo() << "[PCECDI] [INFO] The program has terminated !";
    }

    void Welcome()
    {
        QMessageBox *message = new QMessageBox;
        message->setIcon(QMessageBox::Information);
        message->setWindowTitle(tr("Welcome"));
        message->setWindowIcon(QIcon(":/Images/ICON.ico"));
        message->setText(tr("Welcome to PCECDI!\nPCECDI has detected that you are turning it on for the first time.\nThis program is used to classify students coming to the Cdi.\nTo register you must enter a name, a first name, a class and a reason. \nTo add reasons please consult the \"Choises.txt\" file contained in the \"Settings\" folder."));
        message->show();
        qInfo() << "[PCECDI] [INFO] Successfully showed the welcome screen!";
    }

    SettingsUI settings;
    bool New;

private:
    QLineEdit *NomLineEdit;
    QLineEdit *PrenomLineEdit;
    QComboBox *ClasseCombo1;
    QComboBox *ClasseCombo2;
    QCheckBox *ProfAbsentCheckBox;
    QShortcut *shortcutEasterEgg;
    QShortcut *enterShortcut;
    QComboBox *ComboBox1;
    QSystemTrayIcon trayIcon;
    QTextEdit *TextEdit1;
    QPushButton *QuitButton;
    QPushButton *ApplyButton;
    QPushButton *ToolsButton;
    QLabel *Spacer;
    QLabel *Title;

    void setupUI()
    {
        if(SettingsRead.isFirstLaunch()) New = true;
        else New = false;

        QWidget *window = new QWidget;
        QIcon progIcon(":/Images/ICON.ico");
        trayIcon.setIcon(progIcon);
        NomLineEdit = new QLineEdit(this);
        PrenomLineEdit = new QLineEdit(this);
        ClasseCombo1 = new QComboBox(this);
        ClasseCombo2 = new QComboBox(this);
        ProfAbsentCheckBox = new QCheckBox(tr("Absent teacher ?"), this);
        TextEdit1 = new QTextEdit(this);
        QuitButton = new QPushButton(tr("Quit"), this);
        ApplyButton = new QPushButton(tr("Save"), this);
        ToolsButton = new QPushButton(tr("Tools"), this);
        enterShortcut = new QShortcut(QKeySequence(Qt::Key_Return), this);
        ComboBox1 = new QComboBox(this);
        Spacer = new QLabel(this);
        Title = new QLabel(this);
        QPixmap SpacerImage(":/Images/Spacer.png");
        QPixmap TitleImage(":/Images/Title.png");
        Spacer->setPixmap(SpacerImage);
        Title->setPixmap(TitleImage);
        NomLineEdit->setFixedWidth(screenSize.width()/6.4);
        PrenomLineEdit->setFixedWidth(screenSize.width()/6.4);
        ClasseCombo1->setFixedWidth(screenSize.width()/25.6);
        ClasseCombo2->setFixedWidth(screenSize.width()/6.4/6);
        ComboBox1->setFixedWidth(screenSize.width()/6.4);
        ToolsButton->setFixedWidth(screenSize.width()/6.4);
        Spacer->setFixedSize(screenSize.width()/6.4, 2);
        Title->setFixedSize(TitleImage.width(), TitleImage.height());
        TextEdit1->setFixedSize(screenSize.width()/1.92, screenSize.height()/8);

        ClasseCombo1->setPlaceholderText(tr("Class :"));
        ClasseCombo2->setPlaceholderText("...");
        QStringList ClassChoicesList = SettingsRead.valueAsStringList("grades", DefaultGrades);
        for(int i = 0; i < ClassChoicesList.size(); i++) ClasseCombo1->addItem(ClassChoicesList.at(i));
        QStringList LetterClassChoicesList = SettingsRead.valueAsStringList("gradesLetter", DefaultGradesLetter);
        for(int i = 0; i < LetterClassChoicesList.size(); i++) ClasseCombo2->addItem(LetterClassChoicesList.at(i));
        ComboBox1->setPlaceholderText(tr("I come to the CDI to..."));
        NomLineEdit->setPlaceholderText(tr("Name :"));
        PrenomLineEdit->setPlaceholderText(tr("First name :"));
        QStringList ChoicesList = SettingsRead.valueAsStringList("choices", DefaultChoices);
        for(int i = 0; i < ChoicesList.size(); i++) ComboBox1->addItem(ChoicesList.at(i));

        TextEdit1->setReadOnly(true);
        TextEdit1->setStyleSheet("background-color: solid;");
        ComboBox1->setStyleSheet("background-color: solid;");
        TextEdit1->setAlignment(Qt::AlignCenter);
        TextEdit1->append(tr("Welcome to PCECDI!\nEnter your name, first name, class and finally a reason to be registered.\n|-------------------------------------------------------------------------------------------------------------|\n"));
        TextEdit1->setAlignment(Qt::AlignLeft);

        QVBoxLayout *layout = new QVBoxLayout(window);
        QVBoxLayout *Vlayout = new QVBoxLayout;
        QHBoxLayout *Hlayout = new QHBoxLayout;
        QHBoxLayout *Hlayout2 = new QHBoxLayout;
        QHBoxLayout *Hlayout3 = new QHBoxLayout;
        QHBoxLayout *Hlayout4 = new QHBoxLayout;
        QHBoxLayout *Hlayout5 = new QHBoxLayout;

        layout->addLayout(Hlayout3);
        layout->addLayout(Hlayout);
        layout->addLayout(Hlayout4);

        Hlayout4->addWidget(TextEdit1);
        Hlayout3->addWidget(Title);

        Hlayout->addLayout(Vlayout);
        Vlayout->addWidget(NomLineEdit);
        Vlayout->addWidget(PrenomLineEdit);
        Vlayout->addLayout(Hlayout5);
        Hlayout5->addWidget(ClasseCombo1);
        Hlayout5->addWidget(ClasseCombo2);
        Hlayout5->addWidget(ProfAbsentCheckBox);
        Vlayout->addWidget(ComboBox1);
        Vlayout->addLayout(Hlayout2);
        Hlayout2->addWidget(ApplyButton);
        Hlayout2->addWidget(QuitButton);
        Vlayout->addWidget(Spacer);
        Vlayout->addWidget(ToolsButton);
        setCentralWidget(window);
        setWindowIcon(progIcon);
        setWindowTitle("PCECDI");
        setMinimumSize(sizeHint());

        QPixmap bkgnd(":/Images/WallPaper.jpg");
        bkgnd = bkgnd.scaled(screenSize, Qt::IgnoreAspectRatio);
        QPalette palette;
        palette.setBrush(QPalette::Window, bkgnd);
        this->setPalette(palette);
        trayIcon.show();

        qInfo() << "[PCECDI] [INFO] Successfully initialized the Main UI!";
    }

    void connectButtons()
    {
        QObject::connect(QuitButton, &QPushButton::clicked, qApp, &QApplication::quit);
        QObject::connect(ApplyButton, &QPushButton::clicked, this, &MainWindow::Register);
        QObject::connect(ToolsButton, &QPushButton::clicked, this, &MainWindow::LaunchSettingsUI);
        QObject::connect(enterShortcut, &QShortcut::activated, ApplyButton, &QPushButton::click);
        qInfo() << "[PCECDI] [INFO] Successfully connected all components in Main UI!";
    }

    void ResetFields()
    {
        NomLineEdit->setText("");
        PrenomLineEdit->setText("");
        ClasseCombo1->setCurrentIndex(-1);
        ClasseCombo2->setCurrentIndex(-1);
        ComboBox1->setCurrentIndex(-1);
        ProfAbsentCheckBox->setChecked(false);

        ComboBox1->setPlaceholderText(tr("I come to the CDI to..."));
        NomLineEdit->setPlaceholderText(tr("Name :"));
        PrenomLineEdit->setPlaceholderText(tr("First name :"));
        ClasseCombo1->setPlaceholderText(tr("Class :"));
        ClasseCombo2->setPlaceholderText("...");

        qInfo() << "[PCECDI] [INFO] Fields reset successfully.";
    }

    void copierFichier(bool overwrite, const QString &cheminSource, const QString &cheminDestination)
    {
        QFile fichierSource(cheminSource);
        if (!fichierSource.exists()) {
            qWarning() << "[BACKUP] [WARNING] Source file does not exist :" << cheminSource;
            return;
        }
        QFileInfo infoFichierDestination(cheminDestination);
        QDir dir = infoFichierDestination.dir();
        if (!dir.exists()) {
            if (!dir.mkpath(dir.absolutePath())) {
                qWarning() << "[BACKUP] [WARNING] Unable to create destination directory :" << dir.absolutePath();
                return;
            }
        }
        if(QFile(cheminDestination).exists() && overwrite) QFile(cheminDestination).remove();
        if (fichierSource.copy(cheminDestination)) qInfo() << "[BACKUP] [INFO] File copied successfully to :" << cheminDestination;
        else qWarning() << "[BACKUP] [WARNING] Failed to copy file to :" << cheminDestination;
    }

    QString getCSVFileName() {
        QDateTime currentDateTime = QDateTime::currentDateTime();
        QTimeZone timeZone = currentDateTime.timeZone();
        return QString(currentDateTime.toString(SettingsRead.valueAsString("timeFormat", DefaultTimeFormat).replace("%Y", "yyyy").replace("%y", "yy").replace("%m", "MM").replace("%b", "MMM").replace("%B", "MMMM").replace("%d", "dd").replace("%H", "HH").replace("%I", "hh").replace("%M", "mm").replace("%S", "ss").replace("%p", "AP").replace("%A", "dddd").replace("%a", "ddd").replace("%j", QString::number(currentDateTime.date().dayOfYear()).rightJustified(3, '0')).replace("%U", QString::number(currentDateTime.date().weekNumber()).rightJustified(2, '0')).replace("%c", "ddd MMM dd HH:mm:ss yyyy").replace("%x", "MM-dd-yy").replace("%X", "HH:mm:ss").replace("%Z", timeZone.abbreviation(currentDateTime)).replace("%z", QString("%1%2").arg(currentDateTime.offsetFromUtc() >= 0 ? "+" : "", QString::number(qAbs(currentDateTime.offsetFromUtc() / 3600)).rightJustified(2, '0') + QString::number(qAbs((currentDateTime.offsetFromUtc() % 3600) / 60)).rightJustified(2, '0')))) + ".csv");
    }

private slots:
    void LaunchSettingsUI()
    {
        settings.move(screenSize.width()/2 - settings.width()/2, screenSize.height()/2.5 - settings.height()/2);
        settings.ResetFieldsSettings();
        settings.show();
        qInfo() << "[PCECDI] [INFO] Settings UI launched successfully.";
    }

    void Register()
    {
        bool ok;

        QDateTime *DateWithTime = new QDateTime(QDateTime::currentDateTime());
        QTime ProgTime = DateWithTime->time();
        QDate ProgDate = DateWithTime->date();
        QString ProfAbsentYN;
        if(ProfAbsentCheckBox->isChecked()) ProfAbsentYN = "Oui";
        else ProfAbsentYN = "Non";

        QString CSVFileName = getCSVFileName();
        QString ProgDate2 = QString("%1/%2/%3").arg(ProgDate.day(), 2, 10, QChar('0')).arg(ProgDate.month(), 2, 10, QChar('0')).arg(ProgDate.year(), 4, 10, QChar('0'));
        QString ProgTime2;

        int compsec = ProgTime.second();
        int compheure = ProgTime.hour();
        int compmin = ProgTime.minute();

        const QStringList timeIntervals = {
            "7:20 - 8:20",
            "8:20 - 9:20",
            "9:20 - 10:20",
            "10:20 - 11:35",
            "11:35 - 12:35",
            "12:35 - 13:00"
        };

        if (compheure >= 7 && compheure <= 12) {
            int index = compheure - 7;

            if (compheure >= 8 && compheure <= 12) {
                if ((compheure == 10 && compmin < 35) ||
                    (compheure == 11 && compmin < 35) ||
                    (compheure == 12 && compmin < 35)) {
                    ProgTime2 = timeIntervals[index];
                } else {
                    ProgTime2 = timeIntervals[index + 1];
                }
            } else {
                ProgTime2 = timeIntervals[index];
            }
        } else {
            ProgTime2 = QString("%1:00 - %2:00").arg(compheure).arg(compheure + 1);
        }

        QString nom = NomLineEdit->text();
        QString prenom = PrenomLineEdit->text();
        QString classe = QString(ClasseCombo1->currentText() + ClasseCombo2->currentText());
        QString raison = ComboBox1->currentText();

        nom.remove(',').remove(';').remove('\n');
        prenom.remove(',').remove(';').remove('\n');
        classe.remove('e');

        QFile CSVFile(QString("%1/%2").arg(CSVName, CSVFileName));
        QTextStream textStream(&CSVFile);
        if(!CSVFile.exists())
        {
            qDebug() << "[Registration] [INFO] File " << CSVFile.fileName() << " doesn't works.\n[Registration] [INFO] Creating file" << CSVFile.fileName() << "...";
            SettingsRead.createDirIfDontExist(CSVName);
            CSVFile.open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::Append);
            QString line = "Dates :,Heures :,Noms :,Prénoms :,Classes :,Raisons :,Professeur Absent ?\n";
            textStream << line;
            CSVFile.close();
            Register();
        }
        else
        {
            if(nom == "" || prenom == "" || classe == "" || raison == "")
            {
                if(nom == "") NomLineEdit->setPlaceholderText(tr("Empty ..."));
                if(prenom == "") PrenomLineEdit->setPlaceholderText(tr("Empty ..."));
                if(classe == "") { ClasseCombo1->setPlaceholderText(tr("Empty ...")); ClasseCombo2->setPlaceholderText(tr("Empty ...")); }
                if(raison == "") ComboBox1->setPlaceholderText(tr("Empty ..."));
                qWarning() << "[Registration] [WARNING] Registration failed: One or more fields are empty.";
            }
            else
            {
                if(raison == "Travailler sur les postes" || raison == "Se connecter à l'ENT")
                {
                    int numposte = QInputDialog::getInt(this, tr("Computer number"), tr("Enter your computer number:"), 1, 1, 9, 1, &ok);
                    if (ok && !numposte == 0) raison = QString("%1 - poste : %2").arg(raison).arg(numposte);
                    else return;
                }
                CSVFile.open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::Append);
                textStream << ProgDate2 << "," << ProgTime2 << "," << nom << "," << prenom << "," << classe << "," << raison << "," << ProfAbsentYN << "\n";
                CSVFile.close();
                TextEdit1->append(QString(tr("[PCECDI] [%1] Student %2 %3 in %4 to %5, on %6. Who came for %7 successfully registered !")).arg(QString("%1:%2:%3").arg(compheure, 2, 10, QChar('0')).arg(compmin, 2, 10, QChar('0')).arg(compsec, 2, 10, QChar('0')), nom, prenom, classe, ProgTime2, ProgDate2, raison));
                ResetFields();
                qInfo() << "[Registration] [INFO] Student registered successfully :" << nom << prenom << classe << raison;
            }
        }
    }
};
#endif // MAINWINDOW_H
