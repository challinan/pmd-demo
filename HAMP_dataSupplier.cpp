#include "HAMP_dataSupplier.h"
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

HAMPDataSupplier::HAMPDataSupplier(QObject* parent): QObject(parent)
{
     ecgIndex =-1;
     abpIndex =-1;
     plethIndex=-1;
     pm_data.index=0;

     connect (this,SIGNAL(dataReceived(pm_data_struct*)),parent, SLOT(dataReceived(pm_data_struct*)));
     connect (this,SIGNAL(connectionStatus(bool)),
              parent, SLOT(connectionStatus(bool)));

     timer = new QTimer(this);
     connect(timer, SIGNAL(timeout()), this, SLOT(updateData()));
     timer->setInterval(30);
     timer->stop();

     disTimer = new QTimer(this);
     connect(disTimer, SIGNAL(timeout()), this, SLOT(updateTimer()));
     disTimer->setInterval(500);
     //disTimer->stop();
     m_screenShortPath=QDir::rootPath() + tr("guest1_rootfs/www/pages/screen-dump.png");
}

HAMPDataSupplier::~HAMPDataSupplier()
{
    ecgData.clear();
    abpData.clear();
    plethData.clear();
}

void HAMPDataSupplier::updateData()
{
    int read_bytes=0;

	qDebug("HAMPDataSupplier::updateData entered\n");
	read_bytes = read(fd, &pm_data,sizeof(pm_data_struct));
	if ( read_bytes == -1 ) {
		qDebug("HAMPDataSupplier::updateData: read failed %d\n, read_bytes");
	}
	qDebug("HAMPDataSupplier::updateData read %d bytes", read_bytes);

    emit dataReceived(&pm_data);
}

/* Shutdown message ID */
#define STARTUP_MSG	0xEF56A559
#define SHUTDOWN_MSG	0xEF56A55A

void HAMPDataSupplier::startStopNucleus(bool flg)
{
	unsigned int size;
	int startup_msg = STARTUP_MSG;
	int shutdown_msg = SHUTDOWN_MSG;
	int bytes_rcvd;
	int i=0, rc;

    printf("Nucleus is %s",flg==true?"starting":"stopping");

	if(flg ==true)
	{
		printf("PMD demo: Starting patient monitoring app on Cortex M4\n");
		printf("PMD demo: Loading virtio_rpmsg_bus\n");
		sleep(1);
		system("modprobe virtio_rpmsg_bus");
		printf("PMD demo: Loading imx6sx_remoteproc\n");
		sleep(1);
		system("modprobe imx6sx_remoteproc");
		printf("PMD demo: Loading rpmsg_user_dev_driver\n");
		system("modprobe rpmsg_user_dev_driver");

		sleep(1);
		do
		{
			printf("Open rpmsg device file\n");
			fd = open("/dev/rpmsg", O_RDWR);
			i++;
			if (i > 10)
				break;
			sleep(1);
		} while (fd < 0);

		if(fd < 0) {
			printf("Failed to open file: no rpmsg device - exiting\n");
			goto bailout;
		}

		/* Send start message to remote */
		printf("Send start msg to remote\n");
		rc = write(fd, &startup_msg , sizeof(int));
		if ( rc == -1 ) {
			qDebug("Error writing rpmsg\n");
		}
		qDebug("HAMPDataSupplier::startStopNucleus: Sent %d bytes\n", rc);
		disTimer->stop();
		timer->start();
 	    	emit connectionStatus(true);
	}
	else
	{
		printf("Shutting down Nucleus\n");
		timer->stop();
		rc = write(fd, &shutdown_msg , sizeof(int));
        if ( rc == -1 ) {
            qDebug("Error writing rpmsg\n");
        }
		close(fd);

	   	disTimer->start(50);
	   	pm_data.abpValue=0;
	   	pm_data.ecgValue =0;
	   	pm_data.plethValue=0;
	   	emit connectionStatus(false);

bailout:
		printf("Unloading drivers\n"); 
		system("modprobe -r rpmsg_user_dev_driver");
		system("modprobe -r imx6sx_remoteproc");		
		system("modprobe -r virtio_rpmsg_bus");
		
	}
}



void HAMPDataSupplier::updateTimer()
{
	qDebug("HAMPDataSupplier::updateTimer: Entered - pm_data.index = %d\n", pm_data.index);
	pm_data.index++;
    emit dataReceived(&pm_data);
}

QString HAMPDataSupplier::getScreenShortPath()
{
    return m_screenShortPath;
}
