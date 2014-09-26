#ifndef DATASUPPLIER_H
#define DATASUPPLIER_H

#include <QObject>
#include <QTimer>

typedef struct {
    unsigned short int index;
    signed short int ecg;
    signed short int abp;
    signed short int pleth;
} pm_data_struct;

class DataSupplier: public QObject
{
Q_OBJECT
public:
    DataSupplier(QObject * parent = 0);
    ~DataSupplier();
    QList <unsigned int>  ecgData;
    QList <unsigned int>  abpData;
    QList <unsigned int>  plethData;

public slots:
    void tickerUpdate();

  signals:
    void dataReceived(pm_data_struct *);

private:
    unsigned int getECGData();
    unsigned int getABPData();
    unsigned int getPlethData();

private:
    pm_data_struct pm_data;
    QTimer *timer;
    int ecgIndex;
    int abpIndex;
    int plethIndex;
};

#endif // DATASUPPLIER_H
