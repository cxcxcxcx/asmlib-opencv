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

#ifndef POINTSSHOW_H
#define POINTSSHOW_H

#include <QObject>
#include <QGraphicsItem>
#include <QList>
#include <QPoint>

class PointsPaint : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    PointsPaint();

    QRectF boundingRect() const;
    // QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                QWidget *widget);


    int pickPointByMouse(const QPointF &mouseP) const;
    int getSize() const{ return ql.size(); }
    void addPoint(const QPointF &mouseP);
    void updateSelectedPoint(const QPointF &mouseP);

    void highLight(int pId);
    void selectPoint(int pId);
    void setMirrorPoint(int pId);

    void setRatio(double r){ ratio = r; }

    void clear();
    void setColorScheme(int schemeId);

    QList< QPoint > getPointList() const { return ql; }

private:
    int selectedPoint;
    int mirrorPoint;
    int mouseHighlightPoint;

    QRect boundRect;

    void testUpdateRect(const QPoint &qp);

    QList< QPoint > ql;

    // Colors
    QColor colorHighlightPen, colorHighlightBrush,
            colorSelectedPen, colorSelectedBrush,
            colorMirrorPen, colorMirrorBrush,
            colorPen, colorBrush;

    double ratio;
};

#endif // POINTSSHOW_H
