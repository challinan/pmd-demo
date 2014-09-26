
#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QMouseEvent>
#include "data.h"
#include "helper.h"

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent,QWidget *mainWindow);
    void initialized(TGraphType gType, float graphScale);

    TGraphType getGraphType()  { return m_helper.getGraphType();   }
    float getGraphWaveSize()   { return m_helper.getGraphXScale(); }

    void clearWidget();
    bool clearGraph(QPainter *painterWidget);
    void setScrollingMode(bool bValue);
    bool getScrollingMode(){return m_scrollEnable;}
    void updateOffScreen(QRect dirtyRect);

public slots:
    void animate(signed short int graphData);

signals:
    void mouseClickEvent(Widget *);

protected:
    void paintEvent(QPaintEvent *event);
    void mouseReleaseEvent ( QMouseEvent * e );
    void mousePressEvent ( QMouseEvent * e );

private:
    Helper m_helper;
    bool m_scrollEnable;
    bool m_firstEntry;

    // member variable - flag of click beginning
    bool m_mouseClick;
    bool m_clearWidget;

    QImage *m_image;
    QPainter *m_imagePainter;
};

#endif  // WIDGET_H
