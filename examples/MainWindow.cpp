#include "MainWindow.h"
#include "./ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    emit ui->view_InputCBox->toggled(false);

    ui->input_TB->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->input_TB->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->input_TB->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    ui->input_TB->horizontalHeader()->resizeSection(3, 220);
    ui->input_TB->horizontalHeader()->resizeSection(4, 220);
    ui->input_TB->horizontalHeader()->resizeSection(5, 220);

    ui->input_TB->verticalHeader()->setVisible(false);

    connect(this, &MainWindow::needDebugInfo, this, &MainWindow::on_needneedDebugInfo);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_input_openBtn_clicked()
{
    ui->input_openBtn->setEnabled(false);

    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open txt"), QFileHelper::getGlobalFileOpenDir(), tr("Plain Text Files (*.* *.txt)"));
    QString textData;
    if(QFileHelper::readAllText(fileName, textData)){
        ui->input_TED->setPlainText(textData);

        // init table
        ui->input_TB->clearContents();
        m_formulas = textData.split("\n", QString::SkipEmptyParts);
        ui->input_TB->setRowCount(m_formulas.length());
        for (int i = 0; i < m_formulas.length(); ++i) {
            ui->input_TB->setItem(i, 0, new QTableWidgetItem(QString::number(i+1)));
            ui->input_TB->setItem(i, 1, new QTableWidgetItem(m_formulas.at(i)));
            ui->input_TB->setItem(i, 2, new QTableWidgetItem("Calc"));

            ui->input_TB->item(i,0)->setTextAlignment(Qt::AlignCenter);
            ui->input_TB->item(i,1)->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
            ui->input_TB->item(i,2)->setTextAlignment(Qt::AlignCenter);

            ui->input_TB->item(i,0)->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
            // ui->input_TB->item(i,1)->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
            ui->input_TB->item(i,2)->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        }

        emit needDebugInfo(tr("%1 open sucess.").arg(fileName));
    }else {
        emit needDebugInfo(tr("%1 open error.").arg(fileName));
    }

    ui->input_openBtn->setEnabled(true);
}

void MainWindow::on_needneedDebugInfo(QString info)
{
    ui->log_TED->appendPlainText("[INFO] :" + info);
}

void MainWindow::on_startBtn_clicked()
{
    ui->startBtn->setEnabled(false);
    ui->stopBtn->setEnabled(true);

    int zhy_ms = 0;
    int mp_ms = 0;
    int zhy_std_ms = 0;

    int i = 0;
    for(; i < ui->input_TB->rowCount(); i++){
        if(m_stop) {
            m_stop = false;
            break;
        }

        ERRORINFO_ST info = EasyParserQt::E000;
        ZHYSTD::ERRORINFO_ST zhy_info = ZHYSTD::EasyParserStd::E000;

        QTime zhy_before = QTime::currentTime();
        double zhy_res = EasyParserQt::eval(m_formulas.at(i), info);
        QTime zhy_after = QTime::currentTime();

        QTime zhy_std_before = QTime::currentTime();
        double zhy_std_res = ZHYSTD::EasyParserStd::eval(m_formulas.at(i).toStdString(), zhy_info);
        QTime zhy_std_after = QTime::currentTime();

        QTime mp_before = QTime::currentTime();
        double mp_res = OperaionByMuParser::ComputeExpressionValue(m_formulas.at(i));
        QTime mp_after = QTime::currentTime();

        zhy_ms += zhy_before.msecsTo(zhy_after);
        mp_ms += mp_before.msecsTo(mp_after);
        zhy_std_ms += zhy_std_before.msecsTo(zhy_std_after);

        emit needDebugInfo(tr("Result \n\tCode: %1 Info: %2 \n\tDetail: %3").arg(info.code).arg(info.info).arg(info.detail));

        ui->record_SPBox->setValue(1);

        ui->input_TB->setItem(i, 3, new QTableWidgetItem(QString::number(zhy_res, 'g', PRESCION)));
        ui->input_TB->setItem(i, 4, new QTableWidgetItem(QString::number(mp_res, 'g', PRESCION)));
        ui->input_TB->setItem(i, 5, new QTableWidgetItem(QString::number(zhy_std_res, 'g', PRESCION)));

        ui->input_TB->item(i, 3)->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        ui->input_TB->item(i, 3)->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        ui->input_TB->item(i, 4)->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        ui->input_TB->item(i, 4)->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        ui->input_TB->item(i, 5)->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        ui->input_TB->item(i, 5)->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    }

    ui->cost_time_ZHY_SPBox->setValue(zhy_ms);
    ui->cost_time_MP_SPBox->setValue(mp_ms);
    ui->cost_time_ZHY_STD_SPBox->setValue(zhy_std_ms);

    ui->record_SPBox->setValue(i+1);

    ui->startBtn->setEnabled(true);
    ui->stopBtn->setEnabled(false);
}


void MainWindow::on_stopBtn_clicked()
{
    ui->stopBtn->setEnabled(false);

    m_stop = !m_stop;

    ui->stopBtn->setEnabled(true);
}


void MainWindow::on_input_TB_cellDoubleClicked(int row, int column)
{
    if(row > m_formulas.length() -1)    return;

    if(column == 2){
        ERRORINFO_ST info = EasyParserQt::E000;
        ZHYSTD::ERRORINFO_ST zhy_info = ZHYSTD::EasyParserStd::E000;

        QTime zhy_before = QTime::currentTime();
        double zhy_res = EasyParserQt::eval(m_formulas.at(row), info);
        QTime zhy_after = QTime::currentTime();

        QTime mp_before = QTime::currentTime();
        double mp_res = OperaionByMuParser::ComputeExpressionValue(m_formulas.at(row));
        QTime mp_after = QTime::currentTime();

        QTime zhy_std_before = QTime::currentTime();
        double zhy_std_res = ZHYSTD::EasyParserStd::eval(m_formulas.at(row).toStdString(), zhy_info);
        QTime zhy_std_after = QTime::currentTime();

        ui->cost_time_MP_SPBox->setValue(zhy_before.msecsTo(zhy_after));
        ui->cost_time_MP_SPBox->setValue(mp_before.msecsTo(mp_after));
        ui->cost_time_ZHY_STD_SPBox->setValue(zhy_std_before.msecsTo(zhy_std_after));

        emit needDebugInfo(tr("ZHY Result \n\tCode: %1 Info: %2 \n\tDetail: %3").arg(info.code).arg(info.info).arg(info.detail));
        emit needDebugInfo(tr("ZHY-STD Result \n\tCode: %1 Info: %2 \n\tDetail: %3").arg(zhy_info.code).arg(QString::fromStdString(zhy_info.info)).arg(QString::fromStdString(zhy_info.detail)));

        ui->record_SPBox->setValue(1);

        ui->input_TB->setItem(row, 3, new QTableWidgetItem(QString::number(zhy_res, 'g', PRESCION)));
        ui->input_TB->setItem(row, 4, new QTableWidgetItem(QString::number(mp_res, 'g', PRESCION)));
        ui->input_TB->setItem(row, 5, new QTableWidgetItem(QString::number(zhy_std_res, 'g', PRESCION)));

        ui->input_TB->item(row, 3)->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        ui->input_TB->item(row, 3)->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        ui->input_TB->item(row, 4)->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        ui->input_TB->item(row, 4)->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        ui->input_TB->item(row, 5)->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        ui->input_TB->item(row, 5)->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    }
}


void MainWindow::on_input_TB_cellChanged(int row, int column)
{
    if(row > m_formulas.length() -1)    return;

    if(column != 1 ) return;

    if(ui->input_TB->item(row, 1) == nullptr) return;

    m_formulas[row] = ui->input_TB->item(row, 1)->text();
}


void MainWindow::on_add_recordBtn_clicked()
{
    ui->input_TB->setRowCount(ui->input_TB->rowCount() + 1);

    ui->input_TB->setItem(ui->input_TB->rowCount() - 1, 0, new QTableWidgetItem(QString::number(ui->input_TB->rowCount())));
    ui->input_TB->setItem(ui->input_TB->rowCount() - 1, 1, new QTableWidgetItem("1+1"));
    ui->input_TB->setItem(ui->input_TB->rowCount() - 1, 2, new QTableWidgetItem("Calc"));

    if(ui->input_TB->item(ui->input_TB->rowCount() - 1,0) == nullptr ||
        ui->input_TB->item(ui->input_TB->rowCount() - 1,1) == nullptr ||
        ui->input_TB->item(ui->input_TB->rowCount() - 1,2) == nullptr) return;

    ui->input_TB->item(ui->input_TB->rowCount() - 1,0)->setTextAlignment(Qt::AlignCenter);
    ui->input_TB->item(ui->input_TB->rowCount() - 1,1)->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    ui->input_TB->item(ui->input_TB->rowCount() - 1,2)->setTextAlignment(Qt::AlignCenter);

    ui->input_TB->item(ui->input_TB->rowCount() - 1,0)->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    ui->input_TB->item(ui->input_TB->rowCount() - 1,2)->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    m_formulas.append("1+1");
}


void MainWindow::on_delete_recordBtn_clicked()
{
    if(ui->input_TB->rowCount() <= 0)   return;

    ui->input_TB->removeRow(ui->input_TB->rowCount() - 1);

    m_formulas.pop_back();
}

