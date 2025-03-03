#ifndef QFILEHELPER_H
#define QFILEHELPER_H

#include <QObject>
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QFileDialog>
#include <QDebug>

class QFileHelper : QObject
{
    Q_OBJECT

public:
    QFileHelper();

    static bool readAllText(const QString &fileName, QString &text);
    static bool writeAllText(const QString &fileName, QString &text);

    static QString getGlobalFileOpenDir();
private:
    static QString lastDir;
};

#endif // QFILEHELPER_H
