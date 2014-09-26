#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "graphSettingsPopup.h"
#include "engMenuPopup.h"
#include <QDateTime>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

#ifdef PMD_HAMP
    ui->pbtn_StartStop->setChecked(true);
#endif

    m_alarmBtnNormalStyleSheet=ui->pbtn_ECG_Alarm->styleSheet();
    m_alarmBtnRedStyleSheet= "*{border: 0px;background-image: url(:/images/icnBellRed.png);} *:checked{background-image:url();}";

    m_graphWidget1 = new Widget(ui->graphECGWidget,this);
#ifndef ONLY_SHOW_ECG_GRAPH
    m_graphWidget2 = new Widget(ui->graphABPWidget,this);
    m_graphWidget3 = new Widget(ui->graphPLETHWidget,this);
#endif

    m_graphWidget1->initialized(GraphECG,3);
#ifndef ONLY_SHOW_ECG_GRAPH
    m_graphWidget2->initialized(GraphABP,1);
    m_graphWidget3->initialized(GraphPLETH,2);
#endif

    m_selectedGraphWidget = NULL;

    onSystemClockUpdate();

    // Setup timer to update UI
    m_timerSystemClock = new QTimer(this);
    connect(m_timerSystemClock, SIGNAL(timeout()), this, SLOT(onSystemClockUpdate()));
    m_timerSystemClock->start(1000);

    m_timerDataValues = new QTimer(this);
#ifndef DISABLE_RIGHT_PANEL_NUMERIC_VALUES
    connect(m_timerDataValues, SIGNAL(timeout()), this, SLOT(onDataValuesUpdate()));
    m_timerDataValues->setInterval(3000);
#ifndef PMD_HAMP
    m_timerDataValues->start();
#endif
#endif

    m_HB_Simulate_Data << 70 << 68 << 71 << 69<< 67<< 68;
    m_ABP_Higher_Simulate_Data << 100 << 110 << 107 << 124<< 137<< 119;
    m_ABP_Lower_Simulate_Data << 73 << 88 << 77 << 82 << 91 << 79;
    m_PLETH_Simulate_Data << 93 << 96 << 90 << 97 << 95 << 99;

    m_cstatus=true;

#ifdef PMD_MEHV
    m_nserver =new Server(this);
    m_cstatus=false;
#endif

#ifdef PMD_NUCLEUS
    m_dataSupplier=new DataSupplier(this);
#endif

#ifdef PMD_HAMP
    m_hampdataSupplier =new HAMPDataSupplier(this);
    m_cstatus=true;

    // Setup timer to update UI
    QTimer *hamptimer = new QTimer(this);
    connect(hamptimer, SIGNAL(timeout()), this, SLOT(takeScreenSnapshort()));
    hamptimer->start(3000);

#endif

    m_isPauseAll=false;
    m_isStart=true;
    m_isAlarmSilent=false;
    m_btnSilentPressed=true;
    on_pbtn_Silent_clicked();

    m_graphSettingsPopup =new GraphSettingsPopup(this);
    m_graphSettingsPopup->hide();

    m_engMenuPopup =new EngMenuPopup(this);
    m_engMenuPopup->hide();


    // Setup graph scrolling mode
#ifdef ENABLE_GRAPH_SCROLLING
    //on_pbtn_Scrolling_clicked(true);
    //ui->pbtn_Scrolling->setChecked(true);
    m_graphWidget1->setScrollingMode(true);
    m_graphWidget2->setScrollingMode(true);
    m_graphWidget3->setScrollingMode(true);

#else
    m_graphWidget1->setScrollingMode(false);
    m_graphWidget2->setScrollingMode(false);
    m_graphWidget3->setScrollingMode(false);
#endif
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onDataValuesUpdate()
{
    updatePulse();
    updateABP();
    updatePLETH();
}

void MainWindow::updatePulse()
{
    QString value="0";

    if(m_isPauseAll==true || m_isStart==false)
        return;

    if(m_cstatus)
    {
        value.setNum(this->getPulseValue());
    }

    ui->labelPulse->setText(value);
    ui->labelPulse->repaint();
}

void MainWindow::updateABP()
{

    if(m_isPauseAll==true || m_isStart==false)
        return;
    QString meanValue="(0)";
    if(m_cstatus)
    {
        ui->labelABP->setText(this->getABPValue(meanValue));
        ui->labelABP_2->setText(meanValue);
    }
    else
    {
        ui->labelABP->setText("0/0");
        ui->labelABP_2->setText(meanValue);
    }

    ui->labelABP->repaint();
}

void MainWindow::updatePLETH()
{
    if(m_isPauseAll==true || m_isStart==false)
        return;

    if(m_cstatus)
    {
        ui->labelSPO2->setText(this->getPLETHValue());
    }
    else
        ui->labelSPO2->setText("0");

    ui->labelABP->repaint();
}

void MainWindow::onSystemClockUpdate()
{
    QString dateTimeStr = QDateTime::currentDateTime().toString("hh:mm:ss AP");
    ui->labelTime->setText(dateTimeStr);
    ui->labelTime->repaint();
    m_statusMsgUpdateTime++;
    if(m_statusMsgUpdateTime>4 && ui->labelBulletText->isVisible())
    {
        ui->labelBulletText->setVisible(false);
        ui->lbstatusBullet->setVisible(false);
        m_statusMsgUpdateTime=0;
    }
}

int MainWindow::getPulseValue()
{
    static int index=-1;

    if(index==this->m_HB_Simulate_Data.count()-1)
        index=0;
    else
        index++;

    return m_HB_Simulate_Data[index];
}

QString MainWindow::getABPValue(QString &meanValue)
{
    static int index=-1;

    if(index==this->m_ABP_Higher_Simulate_Data.count()-1)
        index=0;
    else
        index++;

    QString str=QString("%1/%2").arg(m_ABP_Higher_Simulate_Data[index]).arg(m_ABP_Lower_Simulate_Data[index]);
    meanValue=QString("(%1)").arg((m_ABP_Higher_Simulate_Data[index]+m_ABP_Lower_Simulate_Data[index])/2);
    return str;
}

QString MainWindow::getPLETHValue()
{
    static int index=-1;

    if(index==this->m_PLETH_Simulate_Data.count()-1)
        index=0;
    else
        index++;

    QString str=QString("%1").arg(m_PLETH_Simulate_Data[index]);
    return str;
}

void MainWindow::updateStatusBarMessage(QString userMessage)
{
    QString message = QDateTime::currentDateTime().toString("hh:mm AP") + "  ";

    // Append user message
    message += userMessage;

    // Show it
    ui->labelBulletText->setText(message);
    ui->labelBulletText->setVisible(true);
    ui->lbstatusBullet->setVisible(true);
    m_statusMsgUpdateTime=0;
    ui->labelBulletText->repaint();
}

void MainWindow::on_pbtn_Silent_clicked()
{
    QString userMessage = "Audible alert turned ";

    m_btnSilentPressed = !m_btnSilentPressed;

    if(m_btnSilentPressed)
    {
        userMessage += "on";
        m_isAlarmSilent=true;
    }
    else
    {
        userMessage += "off";
        m_isAlarmSilent=false;
    }

    userMessage += ". ";

    // Update status bar
    updateStatusBarMessage(userMessage);
}

void MainWindow::on_pbtn_StartStop_clicked(bool checked)
{
    if(checked)
    {
        m_isStart=false;
        this->m_graphWidget1->clearWidget();
#ifndef ONLY_SHOW_ECG_GRAPH
        this->m_graphWidget2->clearWidget();
        this->m_graphWidget3->clearWidget();
#endif
        //ui->pbtn_StartStop->setText("Start");
    }
    else
    {
        m_isStart=true;
        //ui->pbtn_StartStop->setText("Stop");
    }

    QString userMessage = "Graph rendering is  ";
    if(checked)
        userMessage += "stopped";
    else
        userMessage += "started";
    userMessage += ". ";
    updateStatusBarMessage(userMessage);
}

void MainWindow::on_pbtn_PauseAll_clicked(bool checked)
{
    if(checked)
    {
        ui->pbtn_ECG_Alarm->setChecked(true);
        ui->pbtn_ABP_Alarm->setChecked(true);
        ui->pbtn_spo2_Alarm->setChecked(true);
    }
    else
    {
        ui->pbtn_ECG_Alarm->setChecked(false);
        ui->pbtn_ABP_Alarm->setChecked(false);
        ui->pbtn_spo2_Alarm->setChecked(false);
    }

    QString userMessage = "All alarms are ";
    if(checked)
        userMessage += "stopped";
    else
        userMessage += "activated";
    userMessage += ". ";
    updateStatusBarMessage(userMessage);
}

void MainWindow::updateTimer()
{
#ifdef PMD_MEHV
    pm_data_struct pm={0,0,0,0};
    dataReceived (&pm);
#endif
}

void MainWindow::on_pbtn_ECG_Alarm_clicked(bool checked)
{
    QString userMessage = "ECG Alarm is ";
    if(checked)
        userMessage += "stopped";
    else
        userMessage += "activated";
    userMessage += ". ";
    updateStatusBarMessage(userMessage);
}

void MainWindow::on_pbtn_spo2_Alarm_clicked(bool checked)
{
    QString userMessage = "SPO2 Alarm is ";
    if(checked)
        userMessage += "stopped";
    else
        userMessage += "activated";
    userMessage += ". ";
    updateStatusBarMessage(userMessage);
}

void MainWindow::on_pbtn_ABP_Alarm_clicked(bool checked)
{
    QString userMessage = "ABP Alarm is ";
    if(checked)
        userMessage += "stopped";
    else
        userMessage += "activated";
    userMessage += ". ";
    updateStatusBarMessage(userMessage);
}

void MainWindow::on_pbtn_settings_clicked(bool checked)
{
    (void)checked;

    if(m_graphSettingsPopup->m_isVisible==true)
        return;

    if(m_engMenuPopup->m_isVisible==false)
    {
        m_engMenuPopup->initialized(m_graphWidget1->getScrollingMode());
        m_engMenuPopup->show();
    }
}

void MainWindow::dataReceived(pm_data_struct *current)
{
    if (m_isPauseAll==false && m_isStart==true)
    {
        switch(m_graphWidget1->getGraphType())
        {
        case GraphECG:
            m_graphWidget1->animate(current->ecgValue);
            break;

        case GraphABP:
            m_graphWidget1->animate(current->abpValue);
            break;

        case GraphPLETH:
            m_graphWidget1->animate(current->plethValue);
            break;

        case GraphCO2:
            //TODO
            break;
        }

#ifndef ONLY_SHOW_ECG_GRAPH
        switch(m_graphWidget2->getGraphType())
        {
        case GraphECG:
            m_graphWidget2->animate(current->ecgValue);
            break;

        case GraphABP:
            m_graphWidget2->animate(current->abpValue);
            break;

        case GraphPLETH:
            m_graphWidget2->animate(current->plethValue);
            break;

        case GraphCO2:
            //TODO
            break;
        }

        switch(m_graphWidget3->getGraphType())
        {
        case GraphECG:
            m_graphWidget3->animate(current->ecgValue);
            break;

        case GraphABP:
            m_graphWidget3->animate(current->abpValue);
            break;

        case GraphPLETH:
            m_graphWidget3->animate(current->plethValue);
            break;

        case GraphCO2:
            //TODO
            break;
        }
#endif
    }
}

#ifndef PMD_NUCLEUS
void MainWindow::connectionStatus(bool status)
{
    QDateTime dateTime = QDateTime::currentDateTime();
    QString dateTimeString = dateTime.toString("hh:mmap");
    m_cstatus=status;

    if(status)
    {
        ui->pbtn_ECG_Alarm->setStyleSheet(m_alarmBtnNormalStyleSheet);
        ui->pbtn_ABP_Alarm->setStyleSheet(m_alarmBtnNormalStyleSheet);
        ui->pbtn_spo2_Alarm->setStyleSheet(m_alarmBtnNormalStyleSheet);
        ui->pbtn_ABP_Alarm->setDisabled(false);
        ui->pbtn_ECG_Alarm->setDisabled(false);
        ui->pbtn_spo2_Alarm->setDisabled(false);
        m_timerAlarm->stop();

    }
    else
    {
        ui->pbtn_ECG_Alarm->setStyleSheet(m_alarmBtnRedStyleSheet);
        ui->pbtn_ABP_Alarm->setStyleSheet(m_alarmBtnRedStyleSheet);
        ui->pbtn_spo2_Alarm->setStyleSheet(m_alarmBtnRedStyleSheet);
        ui->pbtn_ABP_Alarm->setDisabled(true);
        ui->pbtn_ECG_Alarm->setDisabled(true);
        ui->pbtn_spo2_Alarm->setDisabled(true);

        m_timerAlarm->start();
        updatePLETH();
        updateABP();
        updatePulse();
    }
}
#endif

void MainWindow::launchGraphMenuPopup(Widget *widget)
{
    if(m_engMenuPopup->m_isVisible==true)
        return;
    if(m_graphSettingsPopup->m_isVisible==false)
    {
        m_selectedGraphWidget=widget;
        switch(m_selectedGraphWidget->getGraphType())
        {
        case GraphECG:
            m_graphSettingsPopup->initialized(m_selectedGraphWidget->getGraphWaveSize(),GraphECG);
            break;

        case GraphABP:
            m_graphSettingsPopup->initialized(m_selectedGraphWidget->getGraphWaveSize(),GraphABP);
            break;

        case GraphPLETH:
            m_graphSettingsPopup->initialized(m_selectedGraphWidget->getGraphWaveSize(),GraphPLETH);
            break;

        case GraphCO2:
            //TODO
            break;
        }

        m_graphSettingsPopup->show();
    }
}

void MainWindow::onGraphMenuPopupOk()
{
    if(m_selectedGraphWidget->getGraphWaveSize()!=m_graphSettingsPopup->m_graphWaveSize)
    {
        switch(m_selectedGraphWidget->getGraphType())
        {
        case GraphECG:

        	m_selectedGraphWidget->initialized(GraphECG,m_graphSettingsPopup->m_graphWaveSize);
            m_selectedGraphWidget->repaint();
            break;

        case GraphABP:

        	m_selectedGraphWidget->initialized(GraphABP,m_graphSettingsPopup->m_graphWaveSize);
            m_selectedGraphWidget->repaint();

            break;

        case GraphPLETH:
        	m_selectedGraphWidget->initialized(GraphPLETH,m_graphSettingsPopup->m_graphWaveSize);
            m_selectedGraphWidget->repaint();

            break;

        case GraphCO2:
            //TODO
            break;
        }
    }
    ui->pbtn_ECG_GraphPopup->setChecked(false);
    ui->pbtn_ABP_GraphPopup->setChecked(false);
    ui->pbtn_spo2_GraphPopup->setChecked(false);
    m_graphSettingsPopup->hide();
    m_graphSettingsPopup->close();
    m_graphSettingsPopup->m_isVisible=false;
    m_selectedGraphWidget=NULL;
}

void MainWindow::onGraphMenuPopupCancel()
{
    ui->pbtn_ECG_GraphPopup->setChecked(false);
    ui->pbtn_ABP_GraphPopup->setChecked(false);
    ui->pbtn_spo2_GraphPopup->setChecked(false);
    m_graphSettingsPopup->hide();
    m_graphSettingsPopup->close();
    m_graphSettingsPopup->m_isVisible=false;
    m_selectedGraphWidget=NULL;
}

void MainWindow::onEngMenuPopupOk()
{
    bool stateChanged=false;

    if(m_engMenuPopup->isScrollingStateChanged(stateChanged))
    {
#ifndef PMD_HAMP
        m_graphWidget1->setScrollingMode(stateChanged);
#ifndef ONLY_SHOW_ECG_GRAPH
        m_graphWidget2->setScrollingMode(stateChanged);
        m_graphWidget3->setScrollingMode(stateChanged);
#endif
#endif
    }
    if(m_engMenuPopup->isNucleusStateChanged(stateChanged))
    {
#ifdef PMD_HAMP

        if(checked)
        {
            m_isStart=false;
            m_timerDataValues->stop();
            //ui->pbtn_StartStop->setText("Start");
            m_hampdataSupplier->startStopNucleus(m_isStart);
            m_cstatus=false;
        }
        else
        {
            m_isStart=true;
            //ui->pbtn_StartStop->setText("Stop");
            m_timerDataValues->start();
            m_hampdataSupplier->startStopNucleus(m_isStart);
            m_cstatus=true;
        }
        //fflush(stdout);
#endif
    }

    m_engMenuPopup->hide();
    m_engMenuPopup->close();
    m_engMenuPopup->m_isVisible=false;
    ui->pbtn_settings->setChecked(false);
}

void MainWindow::onEngMenuPopupCancel()
{
    m_engMenuPopup->hide();
    m_engMenuPopup->close();
    m_engMenuPopup->m_isVisible=false;
    ui->pbtn_settings->setChecked(false);
}

#ifdef PMD_HAMP
void  MainWindow::takeScreenSnapshort()
{
    QRect r= this->rect();
    QPixmap pixmap=this->grab(r);
    QString fileName = m_hampdataSupplier->getScreenShortPath();

    if(pixmap.isNull()==false)
        pixmap.save(fileName,"PNG");
    else
        printf("\npixmap is NULL");
}

#endif

void MainWindow::on_pbtn_ECG_GraphPopup_clicked(bool checked)
{
    (void)checked;
    launchGraphMenuPopup(m_graphWidget1);
}

void MainWindow::on_pbtn_ABP_GraphPopup_clicked(bool checked)
{
    (void)checked;
    launchGraphMenuPopup(m_graphWidget2);
}

void MainWindow::on_pbtn_spo2_GraphPopup_clicked(bool checked)
{
    (void)checked;
    launchGraphMenuPopup(m_graphWidget3);
}
