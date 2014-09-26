
#include "MEHV_dataSupplier.h"
#include <iostream>
using namespace std;

Server::Server(QObject* parent): QObject(parent)
{
  connect(&server, SIGNAL(newConnection()),
    this, SLOT(acceptConnection()));
  connect (this,SIGNAL(dataReceived(pm_data_struct*)),
           parent, SLOT(dataReceived(pm_data_struct*)));
  connect (this,SIGNAL(connectionStatus(bool)),
           parent, SLOT(connectionStatus(bool)));


  server.listen(QHostAddress::Any, SERV_PORT);
  timer = new QTimer(this);
  connect(timer, SIGNAL(timeout()), this, SLOT(updateTimer()));
}

Server::~Server()
{
  server.close();
}

void Server::acceptConnection()
{
  client = server.nextPendingConnection();
 // printf("\n\r Connection established ");
  connect(client, SIGNAL(readyRead()), this, SLOT(startRead()));
  connect(client, SIGNAL(disconnected()),this, SLOT(clientDisconnected()));
  timer->stop();
  emit connectionStatus(true);

}

void Server::startRead()
{
    //pm_data_struct pm_data = {0, -500, -500, -500};
    int read_bytes=0;
    read_bytes = client->read((char*)&pm_data,sizeof(pm_data_struct));

    if(read_bytes < 0)
    {
      //perror("\n\r read error readbytes =0");
      client->close();

      return;
    }
    else if(read_bytes == 0)
    {
      //printf("\n\r No data received");
      client->close();
    }
    else
    {
        //printf("Data received = %u : %d : %d : %d. Data length = %d bytes\n", pm_data.index, pm_data.ecg, pm_data.abp, pm_data.pleth, read_bytes);
        emit dataReceived(&pm_data);
    }
}

void Server::clientDisconnected()
{
    timer->start(50);
    pm_data.abpValue=0;
    pm_data.ecgValue =0;
    pm_data.plethValue=0;
    emit connectionStatus(false);
}

void Server::updateTimer()
{
    pm_data.index++;
    emit dataReceived(&pm_data);
}
