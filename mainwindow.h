#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "helper.h"
#include "widget.h"
#include "data.h"

#ifdef PMD_MEHV
    #include "MEHV_dataSupplier.h"
#endif

#ifdef PMD_NUCLEUS
    #include "NUC_dataSupplier.h"
#endif

#ifdef PMD_HAMP
    #include "HAMP_dataSupplier.h"
#endif

namespace Ui {
class MainWindow;
}

class GraphSettingsPopup;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    int getPulseValue();
    QString getABPValue();
    QString getPLETHValue();

private slots:
    void updatePulse();
    void updateABP();
    void updatePLETH();

    void updateTimer();
    void updateTimeString();
    void animateAlarm();

    void on_pbtn_Scrolling_clicked(bool checked);

    void on_pbtn_StartStop_clicked(bool checked);
    void on_pbtn_PauseAll_clicked(bool checked);
    void on_pbtn_ECG_Alarm_clicked(bool checked);
    void on_pbtn_spo2_Alarm_clicked(bool checked);
    void on_pbtn_ABP_Alarm_clicked(bool checked);

    void dataReceived(pm_data_struct* current);

    void onGraphMenuPopupOk();
    void launchGraphMenuPopup(Widget *widget);
    void onGraphMenuPopupCancel();

#ifndef PMD_NUCLEUS
    void connectionStatus(bool status);
#endif

    void on_pbtn_Silent_clicked();

#ifdef PMD_HAMP
    void takeScreenSnapshort();
#endif

private:
    Ui::MainWindow *ui;
    Widget *m_graphWidget1;
#ifndef ONLY_SHOW_ECG_GRAPH
    Widget *m_graphWidget2;
    Widget *m_graphWidget3;
#endif

    Widget *m_selectedGraphWidget;

    QTimer *m_timerDataValues;

    /* List of Normal Pluse data*/
    QList<int>  m_HB_Simulate_Data;

    /* List for Higher and Lower ABP data*/
    QList<int>  m_ABP_Higher_Simulate_Data;
    QList<int>  m_ABP_Lower_Simulate_Data;

    /* List of SPO2 data*/
    QList<int>  m_PLETH_Simulate_Data;

    QTimer *m_timerAlarm;

    GraphSettingsPopup *m_graphSettingsPop;
    QString m_alarmBtnNormalStyleSheet;
    QString m_alarmBtnRedStyleSheet;

#ifdef PMD_MEHV
    Server *m_nserver;
#endif
    bool m_cstatus;
    bool m_isPauseAll;
    bool m_isStart;
    bool m_isAlarmSilent;
    bool m_btnSilentPressed;
#ifdef PMD_NUCLEUS
    DataSupplier *m_dataSupplier;
#endif

#ifdef PMD_HAMP
    HAMPDataSupplier *m_hampdataSupplier;
#endif

};

#endif // MAINWINDOW_H
