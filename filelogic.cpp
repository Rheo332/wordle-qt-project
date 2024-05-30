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
    for (int i = 1; !input.atEnd(); ++i) {
        QString line = input.readLine();
        outList << line;
        //qDebug() << i << "|" << line;
    }

    file.close();

    return {};
}
