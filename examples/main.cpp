#include <MainWindow.h>
#include <QStyleFactory>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setStyle(QStyleFactory::create("fusion"));

    MainWindow w;
    w.setMinimumSize(QSize(1200, 800));
    w.show();
    return a.exec();
}
