#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "graphSettingsPopup.h"
#include <QDateTime>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

#ifdef PMD_HAMP
    ui->pbtn_StartStop->setChecked(true);
#endif
	qDebug("This is MainWindow via QDebug()\n");

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

    // Set time
    updateTimeString();

    // Setup timer to update UI
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateTimeString()));
    timer->start(1000);

    m_timerAlarm =new QTimer (this);
    connect(m_timerAlarm, SIGNAL(timeout()), this, SLOT(animateAlarm()));
    m_timerAlarm->setInterval(500);
    m_timerAlarm->stop();

    m_timerDataValues = new QTimer(this);
#ifndef DISABLE_RIGHT_PANEL_NUMERIC_VALUES
    connect(m_timerDataValues, SIGNAL(timeout()), this, SLOT(updatePulse()));
    connect(m_timerDataValues, SIGNAL(timeout()), this, SLOT(updateABP()));
    connect(m_timerDataValues, SIGNAL(timeout()), this, SLOT(updatePLETH()));
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
	printf("Creating HAMPDataSupplier\n");
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
    m_btnSilentPressed=false;

    m_graphSettingsPop =new GraphSettingsPopup(this);
    m_graphSettingsPop->hide();

    // Setup graph scrolling mode
#ifdef ENABLE_GRAPH_SCROLLING
    on_pbtn_Scrolling_clicked(true);
    ui->pbtn_Scrolling->setChecked(true);
#else
    on_pbtn_Scrolling_clicked(false);
    ui->pbtn_Scrolling->setChecked(false);
#endif
}

MainWindow::~MainWindow()
{
    delete ui;
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

    if(m_cstatus)
    {
      ui->labelABP->setText(this->getABPValue());
    }
    else
        ui->labelABP->setText("0/0");

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

void MainWindow::updateTimeString()
{
    //get current date and time
    QDateTime dateTime = QDateTime::currentDateTime();
    QString dateTimeString = dateTime.toString("hh:mm:ss AP");
    ui->labelTime->setText(dateTimeString);
    ui->labelTime->repaint();
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

QString MainWindow::getABPValue()
{
    static int index=-1;

    if(index==this->m_ABP_Higher_Simulate_Data.count()-1)
        index=0;
    else
        index++;

    QString str=QString("%1/%2").arg(m_ABP_Higher_Simulate_Data[index]).arg(m_ABP_Lower_Simulate_Data[index]);
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

void MainWindow::on_pbtn_Silent_clicked()
{
   m_btnSilentPressed=!m_btnSilentPressed;
    if(m_btnSilentPressed)
    {
        ui->pbtn_ECG_Alarm->setChecked(true);
        ui->pbtn_ABP_Alarm->setChecked(true);
        ui->pbtn_spo2_Alarm->setChecked(true);
        m_isAlarmSilent=true;
       // ui->pbtn_Silent->setStyleSheet("*{border: 0px;background-image: url(:/images/btn_pressed.png);}");
    }
    else
    {
        ui->pbtn_ECG_Alarm->setChecked(false);
        ui->pbtn_ABP_Alarm->setChecked(false);
        ui->pbtn_spo2_Alarm->setChecked(false);
        m_isAlarmSilent=false;
        //ui->pbtn_Silent->setStyleSheet("");
    }
}

void MainWindow::on_pbtn_StartStop_clicked(bool checked)
{

#ifdef PMD_HAMP

    if(checked)
    {
		qDebug("MainWindow::on_pbtn_StartStop_clicked button pushed - %d\n", checked);
        m_isStart=false;
        m_timerDataValues->stop();
        ui->pbtn_StartStop->setText("Start");
        m_hampdataSupplier->startStopNucleus(m_isStart);
        m_cstatus=false;
    }
    else
    {
		qDebug("MainWindow::on_pbtn_StartStop_clicked button pushed - %d\n", checked);
        m_isStart=true;
        ui->pbtn_StartStop->setText("Stop");
        m_timerDataValues->start();
        m_hampdataSupplier->startStopNucleus(m_isStart);
        m_cstatus=true;

    }
   fflush(stdout);
#else

    if(checked)
    {
        m_isStart=false;
        this->m_graphWidget1->clearWidget();
#ifndef ONLY_SHOW_ECG_GRAPH
        this->m_graphWidget2->clearWidget();
        this->m_graphWidget3->clearWidget();
#endif
        ui->pbtn_StartStop->setText("Start");
    }
    else
    {
        m_isStart=true;
        ui->pbtn_StartStop->setText("Stop");
    }

#endif

}

void MainWindow::on_pbtn_PauseAll_clicked(bool checked)
{
    if(!checked)
    {
        m_isPauseAll=false;
        ui->pbtn_PauseAll->setText("Pause All");
    }
    else
    {
        m_isPauseAll=true;
        this->m_graphWidget1->clearWidget();
#ifndef ONLY_SHOW_ECG_GRAPH
        this->m_graphWidget2->clearWidget();
        this->m_graphWidget3->clearWidget();
#endif
        ui->pbtn_PauseAll->setText("Start All");
    }
}

void MainWindow::on_pbtn_Scrolling_clicked(bool checked)
{
 (void)checked;
#ifndef PMD_HAMP
    m_graphWidget1->setScrollingMode(checked);
#ifndef ONLY_SHOW_ECG_GRAPH
    m_graphWidget2->setScrollingMode(checked);
    m_graphWidget3->setScrollingMode(checked);
#endif
#endif
}

void MainWindow::updateTimer()
{
#ifdef PMD_MEHV
    pm_data_struct pm={0,0,0,0};
    dataReceived (&pm);
#endif
}

void MainWindow::animateAlarm()
{
    ui->pbtn_ABP_Alarm->toggle();
    ui->pbtn_ECG_Alarm->toggle();
    ui->pbtn_spo2_Alarm->toggle();
}

void MainWindow::on_pbtn_ECG_Alarm_clicked(bool checked)
{
    (void)checked;
}

void MainWindow::on_pbtn_spo2_Alarm_clicked(bool checked)
{
    (void)checked;
}

void MainWindow::on_pbtn_ABP_Alarm_clicked(bool checked)
{
    (void)checked;
}

void MainWindow::dataReceived(pm_data_struct *current)
{
	qDebug("MainWindow::dataReceived: Entered\n");
	if (m_isPauseAll==false)
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
        ui->labelBulletText->setText("External system connected at "+dateTimeString);
        ui->labelBulletText->repaint();
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
        ui->labelBulletText->setText("External system disconnected at "+dateTimeString);
        ui->labelBulletText->repaint();
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
   if(m_graphSettingsPop->m_isVisible==false)
   {
       m_selectedGraphWidget=widget;
       switch(m_selectedGraphWidget->getGraphType())
       {
           case GraphECG:
            m_graphSettingsPop->initialized(m_selectedGraphWidget->getGraphWaveSize(),GraphECG);
           break;

           case GraphABP:
            m_graphSettingsPop->initialized(m_selectedGraphWidget->getGraphWaveSize(),GraphABP);
           break;

           case GraphPLETH:
            m_graphSettingsPop->initialized(m_selectedGraphWidget->getGraphWaveSize(),GraphPLETH);
           break;

           case GraphCO2:
                 //TODO
           break;
       }
       m_graphSettingsPop->show();
   }
}

void MainWindow::onGraphMenuPopupOk()
{
    if(m_selectedGraphWidget->getGraphWaveSize()!=m_graphSettingsPop->m_graphWaveSize)
    {
        switch(m_selectedGraphWidget->getGraphType())
        {
            case GraphECG:
                 m_selectedGraphWidget->initialized(GraphECG,m_graphSettingsPop->m_graphWaveSize);
                 m_selectedGraphWidget->repaint();
            break;

            case GraphABP:
                m_selectedGraphWidget->initialized(GraphABP,m_graphSettingsPop->m_graphWaveSize);
                m_selectedGraphWidget->repaint();
            break;

            case GraphPLETH:
                m_selectedGraphWidget->initialized(GraphPLETH,m_graphSettingsPop->m_graphWaveSize);
                m_selectedGraphWidget->repaint();
            break;

            case GraphCO2:
                  //TODO
            break;
        }
    }

    m_graphSettingsPop->hide();
    m_graphSettingsPop->close();
    m_graphSettingsPop->m_isVisible=false;
    m_selectedGraphWidget=NULL;
}

void MainWindow::onGraphMenuPopupCancel()
{
    m_graphSettingsPop->hide();
    m_graphSettingsPop->close();
    m_graphSettingsPop->m_isVisible=false;
    m_selectedGraphWidget=NULL;
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
        printf("pixmap is NULL\n");
}

#endif
