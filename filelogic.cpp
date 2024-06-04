#include "filelogic.h"
#include <QDebug>
#include <QTextStream>

QList<QString> FileLogic::readTextFile(const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "file didn't open";
        return {};
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
