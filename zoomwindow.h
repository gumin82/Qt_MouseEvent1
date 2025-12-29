#ifndef ZOOMWINDOW_H
#define ZOOMWINDOW_H

#include <QMainWindow>
#include <QImage>
#include <QPixmap>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QFileDialog>
#include <QToolBar>
#include <QColorDialog>
#include <QLabel>
#include <QStatusBar>

class ZoomWindow : public QMainWindow
{
    Q_OBJECT

public:
    ZoomWindow(const QImage &img, QWidget *parent = nullptr);
    ~ZoomWindow();

protected:
    void paintEvent(QPaintEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private slots:
    void saveAs();
    void toggleBrush();
    void selectBrushColor();
    void setBrushSize();

private:
    QImage image;
    QPixmap pixmap;
    bool brushEnabled;
    QColor brushColor;
    int brushSize;
    QPoint lastPoint;
    bool drawing;
    QAction *brushAction;
    QLabel *statusLabel;
    QToolBar *toolbar;
};

#endif // ZOOMWINDOW_H
