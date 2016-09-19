
#ifndef HELPER_H
#define HELPER_H

#include <QBrush>
#include <QFont>
#include <QPen>
#include <QWidget>
#include "data.h"

class Helper
{
public:
    Helper();

public:
    void initialized(const int gW, const int gH, TGraphType gType, float gXScale);
    QRect animate(signed short int xGraphData);
    void paintGraph(QPainter *painter);

    void clearGraph(QPainter *painter);

    int getGraphHeight(){ return graphHeight; }
    int getGraphWidth() { return graphWidth; }

    void  setGraphXScale(float xScale);
    float getGraphXScale();

    void   setGraphYScale(float yScale);
    float  getGraphYScale();

    TGraphType getGraphType() { return graphType; }

    QPointF getGraphPosition() { return graphPoint2; }

    QRectF getGraphRectDirty();
    QRectF getGraphRectEraser();

    void resetValues();

private:
    float graphXScale;
    float graphYScale;

    float graphXMaxUnit;        /* Number of units in plug-in area on x-axis. */
    float graphYMaxUnit;        /* Number of units in plug-in area on y-axis. */

    float graphXPerUnit;        /* Calculating # of pixels in x-axis per unit. */
    float graphYPerUnit;        /* Calculating # of pixels in y-axis per unit. */

    float graphYOffset;         /* Calculating y-axis offset value needed to shift the graph to the middle of the plugin area. */

    float graphCellWidth;       /* Graph cell width */

    TGraphType graphType;
    QColor graphColor;
    int graphWidth;
    int graphHeight;

    QPointF graphPoint1;
    QPointF graphPoint2;

    QImage *graphBgImage;

    signed short int graphData;
};

#endif
