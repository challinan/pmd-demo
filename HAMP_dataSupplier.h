#ifndef NUC_DATASUPPLIER_H
#define NUC_DATASUPPLIER_H

#include <QObject>
#include <QTimer>
#include <QtNetwork>
#include <QTcpServer>
#include <QTcpSocket>

typedef struct {
    unsigned short int index;
    signed short int ecgValue;
    signed short int abpValue;
    signed short int plethValue;
} pm_data_struct;

class HAMPDataSupplier: public QObject
{
    Q_OBJECT

public:
    HAMPDataSupplier(QObject * parent = 0);
    ~HAMPDataSupplier();
    void startStopNucleus(bool flg);
    QString getScreenShortPath();
    QList <unsigned int>  ecgData;
    QList <unsigned int>  abpData;
    QList <unsigned int>  plethData;

public slots:
    void updateData();
    void updateTimer();

  signals:
    void dataReceived(pm_data_struct *);
    void connectionStatus(bool);

private:
    unsigned int getECGData();
    unsigned int getABPData();
    unsigned int getPlethData();

private:
    pm_data_struct pm_data;
    QTimer *timer;
    QTimer *disTimer;
    int ecgIndex;
    int abpIndex;
    int plethIndex;
    QString m_screenShortPath;
};

#endif // DATASUPPLIER_H
