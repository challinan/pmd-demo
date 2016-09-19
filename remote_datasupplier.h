#ifndef RMT_DATASUPPLIER_H
#define RMT_DATASUPPLIER_H

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

class RemoteDataSupplier: public QObject
{
    Q_OBJECT

public:
    RemoteDataSupplier(QObject * parent = 0);
    ~RemoteDataSupplier();
    void startStopSocket(bool flg);
    QList <unsigned int>  ecgData;
    QList <unsigned int>  abpData;
    QList <unsigned int>  plethData;

public slots:
	void acceptConnection();
	void readData();

signals:
    void dataReceivedSig(pm_data_struct *);

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
	QTcpSocket* client;
	QString hostname;
	int portnum;
	char byteBuffer[sizeof(pm_data)*2];
};

#endif // RMT_DATASUPPLIER_H
