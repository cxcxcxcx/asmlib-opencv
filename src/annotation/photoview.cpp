#include "photoview.h"
#include<QMouseEvent>
#include<iostream>
#include<QGraphicsView>
using std::cout;
using std::endl;

PhotoView::PhotoView(QWidget *parent)
    : QGraphicsView(parent){
    this->setMouseTracking(true);
    setScene(&gScene);
    //gScene.addRect(0, 0, 90, 90);
    scaleFactor = 1;
    pixItem = NULL;
    dragging = false;
    allowAddPoints = true;
    //pointPaint.ql = &selectedPoints;


    // this->scale(scaleFactor, scaleFactor);
    this->setRenderHint(QPainter::Antialiasing);
    this->setRenderHint(QPainter::SmoothPixmapTransform);

    gScene.addItem(&pointPaint);
    pointPaint.setRatio(this->mapToScene(1, 0).x()-this->mapToScene(0, 0).x());

    //this->fitInView(QRect(QPoint(0, 0), img->size()));
    //this->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
}

void PhotoView::zoomIn(){
    scaleFactor *= 1.2;
    this->scale(1.2, 1.2);
    this->setRenderHint(QPainter::SmoothPixmapTransform);
    pointPaint.setRatio(this->mapToScene(1, 0).x()-this->mapToScene(0, 0).x());
    this->update();
}

void PhotoView::zoomOut(){
    scaleFactor /= 1.2;
    this->scale(1/1.2, 1/1.2);
    pointPaint.setRatio(this->mapToScene(1, 0).x()-this->mapToScene(0, 0).x());
    this->update();
}

void PhotoView::mousePressEvent(QMouseEvent *event){
    QPointF qp = this->mapToScene(event->x(),event->y());
    int tI = pointPaint.pickPointByMouse(qp);
    if (tI != -1){
        pointPaint.selectPoint(tI);
        emit selectedPointUpdated(tI);
        dragging = true;
    }
    else if (allowAddPoints) {
        pointPaint.addPoint(qp);
        pointPaint.selectPoint(-1);
        emit selectedPointUpdated(pointPaint.getSize());
    }

    //QGraphicsView::mousePressEvent(event);
}

void PhotoView::setImage(const QString & imgFileName)
{
//    imageFileName = imgFileName;
//    QImage *img = new QImage(imgFileName);
//    this->setBackgroundBrush(*img);

    if (pixItem != NULL){
        gScene.removeItem(pixItem);
        delete pixItem;
    }

    pixData = QPixmap(imgFileName);//, NULL, Qt::SmoothTransformation);
    pixItem = gScene.addPixmap(pixData);
    pixItem->setTransformationMode(Qt::SmoothTransformation);
//    this->setSceneRect(QRect(QPoint(0, 0), img->size()));
//    gScene.setSceneRect(QRect(QPoint(0, 0), img->size()));
//
    this->setSceneRect(QRect(QPoint(0, 0), pixData.size()));
    gScene.setSceneRect(QRect(QPoint(0, 0), pixData.size()));
    this->update();
}

void PhotoView::mouseReleaseEvent(QMouseEvent *event)
{
    dragging = false;
}

void PhotoView::setMirrorPoint(int pId)
{
    pointPaint.setMirrorPoint(pId);
}

void PhotoView::mouseMoveEvent(QMouseEvent *event)
{
    if ( dragging ){
        pointPaint.updateSelectedPoint(this->mapToScene(event->pos()));
        emit pointsUpdated();
        return;
    }
    QPointF qp = this->mapToScene(event->x(),event->y());
    int tI = pointPaint.pickPointByMouse(qp);
    pointPaint.highLight(tI);
    QGraphicsView::mouseMoveEvent(event);
}

void PhotoView::setPointList(const QList< QPoint > &ql){
    pointPaint.clear();
    for (int i=0;i<ql.size();i++)
        pointPaint.addPoint(QPointF(ql[i]));
    emit pointsUpdated();
}
