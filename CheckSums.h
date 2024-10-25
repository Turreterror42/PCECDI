#ifndef CHECKSUMS
#define CHECKSUMS

#include <QCoreApplication>
#include <QDir>
#include <QDebug>
#include <QFile>
#include <QCryptographicHash>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QStringList>
#include <QDebug>

QString calculateMD5Checksum(const QString &filePath) {
    QFile file(filePath);

    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "[Checksums] [WARNING] Could not open file :" << filePath;
        return QString();
    }

    QCryptographicHash hash(QCryptographicHash::Md5);
    if (!hash.addData(&file)) {
        qWarning() << "[Checksums] [WARNING] Error reading file for checksum :" << filePath;
        return QString();
    }
    return QString(hash.result().toHex());
}

QJsonObject readChecksumsFromJson(const QString &jsonFilePath) {
    QFile file(jsonFilePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "[Checksums] [WARNING] Could not open JSON file :" << jsonFilePath;
        return QJsonObject();
    }

    QByteArray jsonData = file.readAll();
    QJsonDocument jsonDoc(QJsonDocument::fromJson(jsonData));
    return jsonDoc.object();
}

bool compareChecksums(const QJsonObject &jsonChecksums, const QString &directoryPath, int &fileCount) {
    QDir dir(directoryPath);
    QStringList filters;
    filters << "*.dll";

    QStringList fileList = dir.entryList(filters, QDir::Files | QDir::NoSymLinks | QDir::Readable | QDir::NoDotAndDotDot);

    for (const QString &filePath : fileList) {
        QString fullFilePath = dir.absoluteFilePath(filePath);
        QString calculatedChecksum = calculateMD5Checksum(fullFilePath);
        QString jsonChecksum = jsonChecksums.value(filePath).toString();

        qInfo() << "[Checksums] [INFO] File :" << filePath.leftJustified(28) << "| Calculated Checksum :" << calculatedChecksum << "| Json Checksum :" << jsonChecksum << "| " << (calculatedChecksum == jsonChecksum ? "MATCH !" : "FAILED !");

        fileCount++;

        if (calculatedChecksum != jsonChecksum) {
            qWarning() << "[Checksums] [WARNING] Checksum mismatch for file :" << fullFilePath;
            return false;
        }
    }

    foreach (const QString &subDirName, dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot)) {
        QString subDirPath = dir.absoluteFilePath(subDirName);
        if (!compareChecksums(jsonChecksums, subDirPath, fileCount)) return false;
    }
    return true;
}
#endif //CHECKSUMS
