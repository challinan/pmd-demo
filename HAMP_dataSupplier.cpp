
#include "HAMP_dataSupplier.h"
#include <iostream>
using namespace std;

HAMPDataSupplier::HAMPDataSupplier(QObject* parent): QObject(parent)
{
    connect(&server, SIGNAL(newConnection()),this, SLOT(acceptConnection()));

    connect (this,SIGNAL(dataReceived(pm_data_struct*)),parent, SLOT(dataReceived(pm_data_struct*)));
    connect (this,SIGNAL(connectionStatus(bool)),parent, SLOT(connectionStatus(bool)));


    server.listen(QHostAddress::Any, SERV_PORT);
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateTimer()));

    /*
   * set the path with name and fromat of the image
   */
    m_screenShortPath=QDir::currentPath() + tr("/SnapShort.png");
}

HAMPDataSupplier::~HAMPDataSupplier()
{
    server.close();
}

void HAMPDataSupplier::acceptConnection()
{
    client = server.nextPendingConnection();
    //printf("\n\r Connection established ");
    connect(client, SIGNAL(readyRead()),this, SLOT(startRead()));
    connect(client, SIGNAL(disconnected()), this, SLOT(clientDisconnected()));
    timer->stop();

    /*
   * updated connection status information to main applicaiton, based on the
   * connection status a messages is displayed on the screen.
   */
    emit connectionStatus(true);

}

void HAMPDataSupplier::startRead()
{
    //pm_data_struct pm_data = {0, -500, -500, -500};
    int read_bytes=0;
    read_bytes = client->read((char*)&pm_data,sizeof(pm_data_struct));

    if(read_bytes < 0)
    {
        client->close();

        return;
    }
    else if(read_bytes == 0)
    {
        client->close();
    }
    else
    {
        //printf("Data received = %u : %d : %d : %d. Data length = %d bytes\n", pm_data.index, pm_data.ecg, pm_data.abp, pm_data.pleth, read_bytes);
        /*
         * send data values to the main application.
         */
        emit dataReceived(&pm_data);
    }
}

void HAMPDataSupplier::clientDisconnected()
{
    timer->start(50);
    pm_data.abpValue=0;
    pm_data.ecgValue =0;
    pm_data.plethValue=0;
    /*
     * updated connection status information to main applicaiton, based on the
     * connection status a messages is displayed on the screen.
     */
    emit connectionStatus(false);
}

void HAMPDataSupplier::updateTimer()
{
    pm_data.index++;
    emit dataReceived(&pm_data);
}

/*
 * The function is called when the (Start)button is pressed
 * True = Enableld or Start Nucleus
 * False= Disable or Stop Nucleus
 */
void HAMPDataSupplier::startStopNucleus(bool flg)
{
    printf("\nNucleus is %s",flg==true?"started":"stopped");
}

/*
 * return the path where the snapShort is saved.
 */
QString HAMPDataSupplier::getScreenShortPath()
{
    return m_screenShortPath;
}
