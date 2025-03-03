#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <common/QFileHelper.h>
#include <parser_qt/easy_parser_qt.h>
#include <parser_std/easy_parser_std.h>
#include <muparser_engine/OperaionByMuParser.h>
#include <QTime>
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void needDebugInfo(QString info);

private slots:
    void on_input_openBtn_clicked();

    void on_needneedDebugInfo(QString info);

    void on_startBtn_clicked();

    void on_stopBtn_clicked();

    void on_input_TB_cellDoubleClicked(int row, int column);

    void on_input_TB_cellChanged(int row, int column);

    void on_add_recordBtn_clicked();

    void on_delete_recordBtn_clicked();

private:
    Ui::MainWindow *ui;

    bool m_stop = false;

    QStringList m_formulas;
};
#endif // MAINWINDOW_H
