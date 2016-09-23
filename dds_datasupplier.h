#ifndef RMT_DATASUPPLIER_H
#define RMT_DATASUPPLIER_H

#include <QObject>
#include <QTimer>
#include "pmdSupport.h"
#include "ndds/ndds_cpp.h"
#include "ndds/ndds_namespace_cpp.h"

using namespace DDS;

typedef struct {
    unsigned short int index;
    signed short int ecgValue;
    signed short int abpValue;
    signed short int plethValue;
} pm_data_struct;

class DDSDataSupplier: public QObject
{
    Q_OBJECT

public:
    DDSDataSupplier(QObject * parent = 0);
    ~DDSDataSupplier();
    QList <unsigned int>  ecgData;
    QList <unsigned int>  abpData;
    QList <unsigned int>  plethData;

public slots:
	void readData();

signals:
    void dataReceivedSig(pm_data_struct *);

private:
    unsigned int getECGData();
    unsigned int getABPData();
    unsigned int getPlethData();
	int subscriber_shutdown(DomainParticipant *participant);

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

	// DDS Subscriber
    DomainParticipant *participant;
    Subscriber *subscriber;
    Topic *topic;
    DataBus::pm_data_structListener *reader_listener;
    DataReader *reader;
    ReturnCode_t retcode;
    const char *type_name;
    int count;
    int status = 0;
};

#endif // RMT_DATASUPPLIER_H
