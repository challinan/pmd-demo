#ifndef RMT_DATASUPPLIER_H
#define RMT_DATASUPPLIER_H

#include <QObject>
#include <QTimer>
#include "pmd.h"
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

// Need a forward declaration here
namespace DataBus {

    class pm_data_structListener : public DataReaderListener {
      public:
        virtual void on_requested_deadline_missed(
            DataReader* /*reader*/,
            const RequestedDeadlineMissedStatus& /*status*/) {}

        virtual void on_requested_incompatible_qos(
            DataReader* /*reader*/,
            const RequestedIncompatibleQosStatus& /*status*/) {}

        virtual void on_sample_rejected(
            DataReader* /*reader*/,
            const SampleRejectedStatus& /*status*/) {}

        virtual void on_liveliness_changed(
            DataReader* /*reader*/,
            const LivelinessChangedStatus& /*status*/) {}

        virtual void on_sample_lost(
            DataReader* /*reader*/,
            const SampleLostStatus& /*status*/) {}

        virtual void on_subscription_matched(
            DataReader* /*reader*/,
            const SubscriptionMatchedStatus& /*status*/) {}

        virtual void on_data_available(DataReader* reader);
	};
}


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
	int domainId;
    int count;
    int status;
};

#endif // RMT_DATASUPPLIER_H
