#ifndef CONSTANTS
#define CONSTANTS

#include <QString>
#include <QStringList>
#include <QSize>

const QString CSVName = "CSVFiles";
const QString SettingsName = "Settings";
const QString BackupName = "Sauvegarde";
const QString VersionInfoText = "PCECDI_v9.5 - Codex";
const QString progName = QString::fromUtf8(PROJECT_NAME);
const QString progVersion = "9.5";
const QString DefaultLang = "fr";
const bool DefaultBackup = true;
const QString DefaultTimeFormat = "%Y %m";
const QStringList DefaultChoices = {"Club", "Travailler sur les postes", "Dessiner", "Apprendre", "Lire", "Faire un exposé", "Voir des exposés", "Jouer au jeux de sociétés", "Jouer de la musique", "Faire ses devoirs", "Se connecter à l'ENT", "Jouer à des escape games", "Faire des recherches", "Aider des élèves", "Venir en tant que délégué CDI", "Faire du théâtre", "Aider Mme Noiret", "Exclusion", "Dormir", "S'informer", "Retard", "Rendez-vous médical", "Heure de colle"};
const QStringList DefaultGrades = {"6e", "5e", "4e", "3e"};
const QStringList DefaultGradesLetter = {"A", "B", "C", "D", "E", "F", "G", "H", "I"};
QSize screenSize;

#endif CONSTANTS
