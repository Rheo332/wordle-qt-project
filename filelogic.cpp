#include "filelogic.h"

QList<QString> FileLogic::readTextFile(const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        throw file.errorString();
    }
    QList<QString> outList;
    QTextStream input(&file);
    while (!input.atEnd()) {
        QString line = input.readLine();
        outList << line;
    }
    file.close();
    return outList;
}

QList<SaveFileRow> FileLogic::readCsvFile(const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        throw file.errorString();
    }
    QList<SaveFileRow> outList;
    QTextStream input(&file);
    while (!input.atEnd()) {
        QString rowString = input.readLine();
        QList<QString> fields = rowString.split(",");
        if (fields.size() != 4) {
            throw "Invalid csv format";
        }
        SaveFileRow row;
        row.word = fields[0];
        row.games = fields[1].toInt();
        row.wins = fields[2].toInt();
        row.streak = fields[3].toInt();
        outList.append(row);
    }
    file.close();
    return outList;
}

void FileLogic::writeCsvFile(const QString &filePath, const SaveFileRow row)
{
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append)) {
        throw file.errorString();
    }
    QTextStream out(&file);
    out << row.word << "," << row.games << "," << row.wins << "," << row.streak << "\n";
    file.close();
}

void FileLogic::deleteCsvFile(const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        throw file.errorString();
    }
    QTextStream out(&file);
    out << " "
        << "," << 0 << "," << 0 << "," << 0 << "\n";
    file.close();
}
