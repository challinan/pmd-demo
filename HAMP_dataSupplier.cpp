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
     disTimer->setInterval(50);
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

	read_bytes = read(fd, &pm_data,sizeof(pm_data_struct));

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
	int i=0;

    	printf("\nNucleus is %s",flg==true?"started":"stopped");

	if(flg ==true)
	{
		/*system("modprobe -r virtio_rpmsg_bus");
		system("modprobe -r imx6sx_rpmsg_driver");
		printf("\nLoading imx6sx rpmsg driver");
		system("modprobe imx6sx_rpmsg_driver");*/
#if 0
		printf("\nLoading rpmsg user device driver");
		system("modprobe rpmsg_user_dev_driver");
#endif
		printf("\n\r PMD demo: Starting patient monitoring app on Cortex M4\r\n");
		printf("\n\r PMD demo: Loading virtio_rpmsg_bus\r\n");
		system("modprobe virtio_rpmsg_bus");
		printf("\n\r PMD demo: Loading imx6sx_remoteproc\r\n");
		system("modprobe imx6sx_remoteproc");
		printf("\n\r PMD demo: Loading rpmsg_user_dev_driver\r\n");
		system("modprobe rpmsg_user_dev_driver");

		do
		{
			printf("\nSend start msg to remote");
			fd = open("/dev/rpmsg", O_RDWR);
			i++;
			if (i > 100000)
				break;
		} while(fd < 0);

		if(fd < 0)
			printf("Failed to open file \r\n");

		/* Send start message to remote */
		printf("\nSend start msg to remote");
		write(fd, &startup_msg , sizeof(int));
		disTimer->stop();
		timer->start();
 	    	emit connectionStatus(true);


	}
	else
	{
		printf("\nSend stop msg to remote");
		timer->stop();
		write(fd, &shutdown_msg , sizeof(int));
		close(fd);
	    	disTimer->start(50);
	    	pm_data.abpValue=0;
	    	pm_data.ecgValue =0;
	    	pm_data.plethValue=0;
	    	emit connectionStatus(false);
		printf("\nUnload rpmsg_user_device driver, virtio_rpmsg_bus driver, imx6sx_rpmsg_driver driver");
		system("modprobe -r rpmsg_user_dev_driver");
		system("modprobe -r imx6sx_remoteproc");		
		system("modprobe -r virtio_rpmsg_bus");
		
	}
}



void HAMPDataSupplier::updateTimer()
{
	pm_data.index++;
    emit dataReceived(&pm_data);
}

QString HAMPDataSupplier::getScreenShortPath()
{
    return m_screenShortPath;
}
