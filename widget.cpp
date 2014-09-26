
#include <QPainter>
#include <QTimer>
#include <QPaintEvent>
#include <QGraphicsProxyWidget>
#include <QDebug>
#include "widget.h"

Widget::Widget(QWidget *parent, QWidget *mainWindow) : QWidget(parent)
{
    // setup widget properties
    setAttribute(Qt::WA_OpaquePaintEvent, true);
    setFixedSize(parent->width(), parent->height());

    // Setup signals/slots
    connect(this, SIGNAL(mouseClickEvent(Widget * )), mainWindow, SLOT(launchGraphMenuPopup(Widget *)));

    // Setup graph renderer
    m_image = new QImage(size(), QImage::Format_RGB32);
    if (m_image)
    {
        m_imagePainter = new QPainter(m_image);
        m_imagePainter->end();
    }

    // Setup default values
    m_scrollEnable = false;
}

void Widget::initialized(TGraphType gType, float gXScale)
{
    // setup default values
    m_mouseClick = false;
    m_clearWidget = true;


    // Initialize helper class for graph renderer
    m_helper.initialized(this->width(), this->height(), gType, gXScale);
    //initialize the image with background
	QRect dirtyRect(0,0,this->width(),this->height());
    updateOffScreen(dirtyRect);
}

void Widget::animate(signed short int graphData)
{
    // qDebug() << "---------- new data available -----------------";

    QRect dirtyRect = m_helper.animate(graphData);

    // Do off-screen graph rendering
    updateOffScreen(dirtyRect);

    // Ask Qt to repaint the widget
    //repaint();
    // Ask Qt to repaint the widget
   if(m_scrollEnable)
       repaint();
   else
       repaint(dirtyRect);
}

void Widget::updateOffScreen(QRect dirtyRect)
{
    /** off-screen painter **/

    m_imagePainter->begin(m_image);
    m_imagePainter->setRenderHint(QPainter::Antialiasing);

    // Clear graph if there any pending event
    if (clearGraph(m_imagePainter) == true)
    {
        // Update dirty region as
        dirtyRect |= QRect(0, 0, m_helper.getGraphWidth(), m_helper.getGraphHeight());
    }

    // Draw new graph line
    m_helper.paintGraph(m_imagePainter);
    m_imagePainter->end();
}

void Widget::paintEvent(QPaintEvent *event)
{
    if (event->region() == QRegion())
    {
        qWarning() << "Widget::paintEvent is having an empty region";
    }

    {   /** on-screen painter **/

        QPainter painter(this);
        if (m_scrollEnable)
        {
            /**
             * Our off-screen painter doesn't know abt scrolling so rendering is
             * done as non-scrolling mode. On-screen painter divides the off-screen
             * graph into two rectangles and swap those.
             */
            int width     = m_helper.getGraphWidth();
            int height    = m_helper.getGraphHeight();
            float xOffset = m_helper.getGraphPosition().x() + 2.0f; // 2.0f is padding to x-axis position

            // Paint the second part of the buffer to the start of the graph.
            painter.drawImage(QRect(0,       0, width - xOffset, height), *m_image,
                              QRect(xOffset, 0, width - xOffset, height));

            // Paint the first part of the buffer to the end of the graph.
            painter.drawImage(QRect(width - xOffset, 0, xOffset, height), *m_image,
                              QRect(0,               0, xOffset, height));
        }
        else
        {
            //Limit the drawing to the updated part
            //this needs to be checked on hardware for the cost/performance as the painteEvent have the dirty rect set
            //what happen if we draw all the image inseatd of its dirty portion ???
           QRect r=event->rect();
           painter.drawImage(r.x(), r.y(), *m_image,r.x(),r.y(),r.width(),r.height());
        }

        painter.end();
    }
}

void Widget::mousePressEvent(QMouseEvent *e)
{
    // suppress compiler warning
    (void)e;

    // set the flag meaning "click begin"
    m_mouseClick = true;
}

void Widget::mouseReleaseEvent(QMouseEvent *e)
{
    // check if cursor is inside the widget boundary
    if ((m_mouseClick) && (this->rect().contains(e->pos())))
    {
        // emit Click signal to Mainwindow to lauch the popup
        emit mouseClickEvent(this);
    }
}

void Widget::clearWidget()
{
    // Place clear widget request which will be satisfy on the next paint event
    m_clearWidget = true;
}

bool Widget::clearGraph(QPainter *painterWidget)
{
    bool isCleared = false;

    // Any pending clear request?
    if (m_clearWidget)
    {
        // Clear the graph
        m_helper.clearGraph(painterWidget);

        // Clear graph clear request
        m_clearWidget = false;
        isCleared = true;
    }

    return isCleared;
}

void Widget::setScrollingMode(bool bValue)
{
    // Update scroll status
    m_scrollEnable = bValue;

    //the graph should start from ends in scrolling(right) or non-scrolling(left)
    m_helper.resetValues();

    // Clear widget and graph
    clearWidget();

    //clear the graph
    //initialize the image with background
   	QRect dirtyRect(0,0,this->width(),this->height());
    m_imagePainter->begin(m_image);
    m_imagePainter->setRenderHint(QPainter::Antialiasing);
    clearGraph(m_imagePainter);
    m_imagePainter->end();

    // full graph refresh to get the background
    repaint();
}
