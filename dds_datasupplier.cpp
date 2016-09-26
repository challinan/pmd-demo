#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <time.h>
#include <fcntl.h>
#include <pthread.h>
#include <string.h>
#include "dds_datasupplier.h"

using namespace DDS;

pm_data_struct pm;

namespace DataBus {

	void pm_data_structListener::on_data_available(DataReader* reader)
	{
		DataBus::pm_data_structDataReader *pm_data_struct_reader = NULL;
		DataBus::pm_data_structSeq data_seq;
		SampleInfoSeq info_seq;
		ReturnCode_t retcode;
		int i;

		pm_data_struct_reader = DataBus::pm_data_structDataReader::narrow(reader);
        if (pm_data_struct_reader == NULL) {
            printf("DataReader narrow error\n");
            return;
        }

        retcode = pm_data_struct_reader->take(
            data_seq, info_seq, LENGTH_UNLIMITED,
            ANY_SAMPLE_STATE, ANY_VIEW_STATE, ANY_INSTANCE_STATE);

        if (retcode == RETCODE_NO_DATA) {
            return;
        } else if (retcode != RETCODE_OK) {
            printf("take error %d\n", retcode);
            return;
        }

        for (i = 0; i < data_seq.length(); ++i) {
            if (info_seq[i].valid_data) {
				pm.ecgValue = data_seq[i].ecgValue;
				pm.abpValue = data_seq[i].abpValue;
				pm.plethValue = data_seq[i].plethValue;
                // printf("Received data - %d\n", i);
                // DataBus::pm_data_structTypeSupport::print_data(&data_seq[i]);
				dds_parent->dataReady(&pm);
            }
        }

        retcode = pm_data_struct_reader->return_loan(data_seq, info_seq);
        if (retcode != RETCODE_OK) {
            printf("return loan error %d\n", retcode);
        }
    }

}

DDSDataSupplier::DDSDataSupplier(QObject* parent): QObject(parent)
{
	printf("DDSDataSupplier::DDSDataSupplier() entered - parent %p\n", parent);

	connect(this, SIGNAL(dataReceivedSig(pm_data_struct *)), parent, SLOT(dataReceivedSlot(pm_data_struct *)));

	// Initialize DDS
    participant = NULL;
    subscriber = NULL;
    topic = NULL;
    reader_listener = NULL;
    reader = NULL;
    type_name = NULL;
    count = 0;
    status = 0;	
	domainId = 0;

	/* To customize the participant QoS, use 
    the configuration file USER_QOS_PROFILES.xml */
	printf("DDSDataSupplier::DDSDataSupplier() calling create_participant - domainId = %d\n", domainId);
    participant = TheParticipantFactory->create_participant(
        domainId, PARTICIPANT_QOS_DEFAULT,
        NULL /* listener */, STATUS_MASK_NONE);
    if (participant == NULL) {
        printf("create_participant error\n");
        subscriber_shutdown(participant);
        return;
    }
    
	/* To customize the subscriber QoS, use 
    the configuration file USER_QOS_PROFILES.xml */
	printf("DDSDataSupplier::DDSDataSupplier(): calling create_subscriber\n");
    subscriber = participant->create_subscriber(
        SUBSCRIBER_QOS_DEFAULT, NULL /* listener */, STATUS_MASK_NONE);
    if (subscriber == NULL) {
        printf("create_subscriber error\n");
        subscriber_shutdown(participant);
        return;
    }

    /* Register the type before creating the topic */
    type_name = DataBus::pm_data_structTypeSupport::get_type_name();
    retcode = DataBus::pm_data_structTypeSupport::register_type(
        participant, type_name);
    if (retcode != RETCODE_OK) {
        printf("register_type error %d\n", retcode);
        subscriber_shutdown(participant);
        return;
    }

    /* To customize the topic QoS, use 
    the configuration file USER_QOS_PROFILES.xml */
    topic = participant->create_topic(
        "Example DataBus_pm_data_struct",
        type_name, TOPIC_QOS_DEFAULT, NULL /* listener */,
        STATUS_MASK_NONE);
    if (topic == NULL) {
        printf("create_topic error\n");
        subscriber_shutdown(participant);
        return;
    }

    /* Create a data reader listener */
    reader_listener = new DataBus::pm_data_structListener(this);

    /* To customize the data reader QoS, use 
    the configuration file USER_QOS_PROFILES.xml */
    reader = subscriber->create_datareader(
        topic, DATAREADER_QOS_DEFAULT, reader_listener,
        STATUS_MASK_ALL);
    if (reader == NULL) {
        printf("create_datareader error\n");
        subscriber_shutdown(participant);
        delete reader_listener;
        return;
    }

}

DDSDataSupplier::~DDSDataSupplier()
{
	printf("DDSDataSupplier::~DDSDataSupplier() closing\n");
	// Cleanup
	delete reader_listener;
	subscriber_shutdown(participant);
}

void DDSDataSupplier::readData() {
}

void DDSDataSupplier::dataReady(pm_data_struct *pm)
{
	// Pass the data to the UI for display
	emit DDSDataSupplier::dataReceivedSig(pm);	
}

unsigned int DDSDataSupplier::getECGData()
{
    if (ecgIndex == ecgData.count()-1)
        ecgIndex = -1;

    ecgIndex++;
    return ecgData[ecgIndex];
}

unsigned int DDSDataSupplier::getABPData()
{
    if (abpIndex == abpData.count()-1)
        abpIndex = -1;

    abpIndex++;
    return abpData[abpIndex];
}

unsigned int DDSDataSupplier::getPlethData()
{
    if (plethIndex == plethData.count()-1)
        plethIndex = -1;

    plethIndex++;
    return plethData[plethIndex];
}

int DDSDataSupplier::subscriber_shutdown(DomainParticipant *participant) {

    ReturnCode_t retcode;
    int status = 0;

    if (participant != NULL) {
        retcode = participant->delete_contained_entities();
        if (retcode != RETCODE_OK) {
            printf("delete_contained_entities error %d\n", retcode);
            status = -1;
        }

        retcode = TheParticipantFactory->delete_participant(participant);
        if (retcode != RETCODE_OK) {
            printf("delete_participant error %d\n", retcode);
            status = -1;
        }
    }
   /* RTI Connext provides the finalize_instance() method on
    domain participant factory for people who want to release memory used
    by the participant factory. Uncomment the following block of code for
    clean destruction of the singleton. */
    /*

    retcode = DomainParticipantFactory::finalize_instance();
    if (retcode != RETCODE_OK) {
        printf("finalize_instance error %d\n", retcode);
        status = -1;
    }
    */
    return status;
}
