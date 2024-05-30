#ifndef FILELOGIC_H
#define FILELOGIC_H

#include <QFile>
#include <QList>

class FileLogic
{
public:
    QList<QString> readTextFile(const QString &filePath);
};

#endif // FILELOGIC_H
