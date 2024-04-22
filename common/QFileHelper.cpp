#include "QFileHelper.h"

QString QFileHelper::lastDir = "";

QFileHelper::QFileHelper() {

}

bool QFileHelper::readAllText(const QString &fileName, QString &text)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    QTextStream in(&file);
    in.setCodec("utf-8");

    text = in.readAll();

    lastDir = QFileInfo(fileName).dir().absolutePath();

    file.close();
    return true;
}

bool QFileHelper::writeAllText(const QString &fileName, QString &text)
{
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return false;

    QTextStream in(&file);
    in.setCodec("utf-8");

    in<<text;

    lastDir = QFileInfo(fileName).dir().absolutePath();

    file.close();
    return true;
}

QString QFileHelper::getGlobalFileOpenDir()
{
    if(!QDir(lastDir).exists()){
        lastDir = QDir::homePath();
    }

    return lastDir;
}
