
#include <QPainter>
#include <QPaintEvent>
#include <QWidget>
#include <QDebug>
#include "helper.h"

Helper::Helper()
{
}

void Helper::initialized(const int gW, const int gH, TGraphType gType, float gXScale)
{
    graphBgImage = new QImage(":/images/grid.png");

    graphType = gType;

    if (graphType == GraphECG)
    {
        graphColor.setRgb(0,255,0);
    }
    else if (graphType == GraphABP)
    {
        graphColor.setRgb(255,0,0);
    }
    else if (graphType == GraphPLETH)
    {
        graphColor.setRgb(0,255,245);
    }

    // Graph widthxheight
    graphHeight = gH;
    graphWidth = gW;

    /* Calculating y-axis offset value needed to shift the graph
          to the middle of the plugin area. */
    //the zero line will be at %80 of height
    graphYOffset = graphHeight * 0.20;

    // Setup graph scales
    setGraphXScale(gXScale);
    setGraphYScale(1);

    //set the initial values to be same
    graphPoint2=graphPoint1;
}

void Helper::setGraphXScale(float xScale)
{
    graphXScale = xScale;

    /* Number of units in plug-in area on x-axis. */
    graphXMaxUnit = graphWidth / graphXScale;

    /* Calculating # of pixels in x-axis per unit. */
    graphXPerUnit = graphWidth / graphXMaxUnit;

    // Initialize graph cell width
    graphCellWidth = graphXScale + 10.0f;

    // Initialize graph previous point
    graphPoint1.setX(0);
    graphPoint1.setY(graphYMaxUnit - graphYOffset);
}

float Helper::getGraphXScale()
{
    return graphXScale;
}

void Helper::setGraphYScale(float yScale)
{
    graphYScale = yScale;

    /* Number of units in plug-in area on y-axis. */
    graphYMaxUnit = graphHeight / graphYScale;

    /* Calculating # of pixels in y-axis per unit. */
    graphYPerUnit = graphHeight / graphYMaxUnit;

    // Initialize graph previous point
    graphPoint1.setY(graphYMaxUnit - graphYOffset);
}

float Helper::getGraphYScale()
{
    return graphYScale;
}

void Helper::clearGraph(QPainter *painter)
{
    //qDebug() << "Graph has been cleared";
    painter->drawImage(0, 0, *graphBgImage);
}

void Helper::paintGraph(QPainter *painter)
{
    // Erase the old graph line
    painter->drawImage(getGraphRectEraser(), *graphBgImage, getGraphRectEraser());

    // Draw line to previous to the new point
    painter->setPen(QPen (graphColor, 1, Qt::SolidLine));
    painter->drawLine(graphPoint1, graphPoint2);

    // Save for later reference
    graphPoint1 = graphPoint2;

    // Wrap the line
    if (graphPoint1.x() + graphXPerUnit >= graphWidth)
        graphPoint1.setX(0);
}

QRect Helper::animate(signed short int xGraphData)
{
    // Save new graph data
    graphData = xGraphData;

    // Calculate new point
    graphPoint2.setY((graphYMaxUnit - (graphData + graphYOffset)));
    graphPoint2.setX(graphPoint1.x() + graphXPerUnit);

    // Calculate dirty rect
    return getGraphRectDirty().toRect();
}

QRectF Helper::getGraphRectDirty()
{
    return getGraphRectEraser();
}

QRectF Helper::getGraphRectEraser()
{
    return QRectF(graphPoint1.x(), 0.0, graphCellWidth, graphHeight);
}

void Helper::resetValues()
{
    graphPoint1.setX(0);
    graphPoint2=graphPoint1;
}
