#ifndef MEHV_DATASUPPLIER_H
#define MEHV_DATASUPPLIER_H

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

class Server: public QObject
{
    Q_OBJECT

public:
    Server(QObject * parent = 0);
    ~Server();

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
};

#endif // MEHV_DATASUPPLIER_H
