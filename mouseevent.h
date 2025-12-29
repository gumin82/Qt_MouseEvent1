#ifndef MOUSEEVENT_H
#define MOUSEEVENT_H

#include <QMainWindow>
#include <QMouseEvent>
#include <QStatusBar>
#include <QLabel>
#include <QImage>
#include <QPixmap>
#include <QPoint>
#include <QPainter>
#include <QRubberBand>
#include <QRect>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QFileDialog>

class MouseEvent : public QMainWindow
{
    Q_OBJECT

public:
    MouseEvent(QWidget *parent = nullptr);
    ~MouseEvent();
protected:
    void mouseDoubleClickEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);
private slots:
    void loadImage();
    void setZoomRatio();
private:
    QLabel *statusLabel;
    QLabel *MousePosLabel;
    QLabel *grayValueLabel;
    QImage image;
    QPixmap pixmap;
    QPoint origin;
    QRubberBand *rubberBand;
    double zoomRatio;
    QRect imageRect;
};
#endif // MOUSEEVENT_H
