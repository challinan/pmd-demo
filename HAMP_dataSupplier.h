#ifndef HAMP_DATASUPPLIER_H
#define HAMP_DATASUPPLIER_H

#include <QtNetwork>
#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>

#define SERV_PORT   3000 /* server port */

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

public slots:
    void acceptConnection();
    void startRead();
    void clientDisconnected();
    void updateTimer();


signals:
    void dataReceived(pm_data_struct *);
    void connectionStatus(bool);

private:
    QTcpServer server;
    QTcpSocket* client;
    pm_data_struct pm_data;
    QTimer *timer;
    QString m_screenShortPath;
};

#endif // HAMP_DATASUPPLIER_H
