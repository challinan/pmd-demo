#include "remote_datasupplier.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <time.h>
#include <fcntl.h>
#include <pthread.h>
#include <string.h>

using namespace std;

int fd;

RemoteDataSupplier::RemoteDataSupplier(QObject* parent): QObject(parent)
{
	printf("RemoteDataSupplier::RemoteDataSupplier() entered - parent %p\n", parent);

	client = new QTcpSocket(this);
	connect(client, SIGNAL(connected()), this, SLOT(acceptConnection()));
	connect(client, SIGNAL(readyRead()), this, SLOT(readData()));
	printf("RemoteDataSupplier::RemoteDataSupplier() calling connect to main\n");
	connect(this, SIGNAL(dataReceivedSig(pm_data_struct *)), parent, SLOT(dataReceivedSlot(pm_data_struct *)));
	printf("RemoteDataSupplier::RemoteDataSupplier() connect called to main\n");

	// Connect to host
	hostname = "localhost";
	portnum = 3020;
	client->abort(); // Sort of like a flush
	client->connectToHost(hostname, portnum);
}

RemoteDataSupplier::~RemoteDataSupplier()
{
	printf("RemoteDataSupplier::~RemoteDataSupplier() closing\n");
    // close socket if not already closed
}

void RemoteDataSupplier::acceptConnection() {
	printf("RemoteDataSupplier::acceptConnection() connection accepted\n");
}

void RemoteDataSupplier::readData() {
	qint64 rc;
	rc = client->read((char *)&pm_data, sizeof(pm_data));
	emit dataReceivedSig(&pm_data);
}

void RemoteDataSupplier::startStopSocket(bool flg)
{

}

unsigned int RemoteDataSupplier::getECGData()
{
    if (ecgIndex == ecgData.count()-1)
        ecgIndex = -1;

    ecgIndex++;
    return ecgData[ecgIndex];
}

unsigned int RemoteDataSupplier::getABPData()
{
    if (abpIndex == abpData.count()-1)
        abpIndex = -1;

    abpIndex++;
    return abpData[abpIndex];
}

unsigned int RemoteDataSupplier::getPlethData()
{
    if (plethIndex == plethData.count()-1)
        plethIndex = -1;

    plethIndex++;
    return plethData[plethIndex];
}

