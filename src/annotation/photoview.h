/*
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
** If you have questions regarding the use of this file, please contact
** Chen Xing at cxcxcxcx@gmail.com .
*/

#ifndef PHOTOVIEW_H
#define PHOTOVIEW_H

#include <QGraphicsView>
#include "pointspaint.h"
#include <QPoint>
#include <QObject>
#include <QList>

class PhotoView : public QGraphicsView
{
    Q_OBJECT

public:
    PhotoView(QWidget *parent = 0);

    void zoomIn();
    void zoomOut();

    void setImage(const QString & imgFileName);

    QList< QPoint > getPointList(){ return pointPaint.getPointList(); }
    void setPointList(const QList< QPoint > &ql);

    bool allowAddPoints;
public slots:
    void setMirrorPoint(int pId);

signals:
    void selectedPointUpdated(int pId);
    void pointsUpdated();

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

    QGraphicsScene gScene;
    PointsPaint pointPaint;

    // QList< QPoint > selectedPoints;

    double scaleFactor;

private:
    bool dragging;
    QString imageFileName;

    QGraphicsPixmapItem *pixItem;
    QPixmap pixData;
};

#endif // PHOTOVIEW_H
