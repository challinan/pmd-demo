#include "graphSettingsPopup.h"
#include "mainwindow.h"
#include "ui_graphSettingsPopup.h"

GraphSettingsPopup::GraphSettingsPopup(MainWindow *parent):
    ui(new Ui::GraphSettingsPopup),
    m_mainWindow(parent)
{
    setWindowFlags(Qt::FramelessWindowHint| Qt::WindowStaysOnTopHint);

    ui->setupUi(this);
    m_isVisible=false;
    connect(ui->okButton, SIGNAL(clicked()), m_mainWindow, SLOT(onGraphMenuPopupOk()));
    connect(ui->cancelButton, SIGNAL(clicked()), m_mainWindow, SLOT(onGraphMenuPopupCancel()));
}

GraphSettingsPopup::~GraphSettingsPopup()
{
    delete ui;
}

void GraphSettingsPopup::on_pbtn_SizeMinus_clicked()
{
    QString val;
    if(m_graphWaveSize>1)
    {
        m_graphWaveSize--;
        val.setNum(m_graphWaveSize);
        ui->lbwavesize->setText(val);
        ui->lbwavesize->update();
    }
}

void GraphSettingsPopup::on_pbtn_SizeAuto_clicked()
{
    QString val;
    m_graphWaveSize=2;
    val.setNum(m_graphWaveSize);
    ui->lbwavesize->setText(val);
    ui->lbwavesize->update();
}

void GraphSettingsPopup::on_pbtn_SizePlus_clicked()
{
    QString val;
    if(m_graphWaveSize<3)
    {
        m_graphWaveSize++;
        val.setNum(m_graphWaveSize);
        ui->lbwavesize->setText(val);
        ui->lbwavesize->update();
    }
}

void GraphSettingsPopup::on_pbtn_wave_co2_clicked()
{
    m_graphType=GraphCO2;
}

void GraphSettingsPopup::on_pbtn_wave_abp_clicked()
{
    m_graphType=GraphABP;
}

void GraphSettingsPopup::on_pbtn_wave_ecg_clicked()
{
    m_graphType=GraphECG;
}

void GraphSettingsPopup::on_pbtn_wave_pleth_clicked()
{
    m_graphType=GraphPLETH;
}

void GraphSettingsPopup::initialized(int graphSize,TGraphType graphType )
{
    QString val;

    m_graphWaveSize = graphSize;
    m_graphType = graphType;

    val.setNum(m_graphWaveSize);
    ui->lbwavesize->setText(val);

    switch(graphType)
    {
        case GraphECG:
            ui->msgTitle->setText("ECG Menu");
        break;

        case GraphABP:
            ui->msgTitle->setText("ABP Menu");
        break;

        case GraphPLETH:
            ui->msgTitle->setText("PLETH Menu");
        break;

        case GraphCO2:
            ui->msgTitle->setText("CO2 Menu");
        break;
    }
    m_isVisible=true;
}
