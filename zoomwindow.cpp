#include "zoomwindow.h"
#include <QPaintEvent>
#include <QMessageBox>
#include <QInputDialog>
#include <QDebug>

ZoomWindow::ZoomWindow(const QImage &img, QWidget *parent)
    : QMainWindow(parent), image(img), brushEnabled(false), 
      brushColor(Qt::red), brushSize(5), drawing(false)
{
    setWindowTitle(QStringLiteral("放大檢視"));
    
    // Create menu bar
    QMenuBar *menuBar = new QMenuBar(this);
    QMenu *fileMenu = menuBar->addMenu(QStringLiteral("檔案"));
    QMenu *toolMenu = menuBar->addMenu(QStringLiteral("工具"));
    
    // File menu actions
    QAction *saveAction = new QAction(QStringLiteral("另存新檔"), this);
    saveAction->setShortcut(QKeySequence::Save);
    connect(saveAction, &QAction::triggered, this, &ZoomWindow::saveAs);
    fileMenu->addAction(saveAction);
    
    // Tool menu actions
    brushAction = new QAction(QStringLiteral("畫筆工具"), this);
    brushAction->setCheckable(true);
    connect(brushAction, &QAction::triggered, this, &ZoomWindow::toggleBrush);
    toolMenu->addAction(brushAction);
    
    QAction *colorAction = new QAction(QStringLiteral("選擇畫筆顏色"), this);
    connect(colorAction, &QAction::triggered, this, &ZoomWindow::selectBrushColor);
    toolMenu->addAction(colorAction);
    
    QAction *sizeAction = new QAction(QStringLiteral("設定畫筆大小"), this);
    connect(sizeAction, &QAction::triggered, this, &ZoomWindow::setBrushSize);
    toolMenu->addAction(sizeAction);
    
    setMenuBar(menuBar);
    
    // Create toolbar
    toolbar = new QToolBar(this);
    toolbar->addAction(saveAction);
    toolbar->addSeparator();
    toolbar->addAction(brushAction);
    toolbar->addAction(colorAction);
    toolbar->addAction(sizeAction);
    addToolBar(toolbar);
    
    // Status bar
    statusLabel = new QLabel(this);
    statusLabel->setText(QStringLiteral("就緒"));
    statusBar()->addWidget(statusLabel);
    
    // Update pixmap
    pixmap = QPixmap::fromImage(image);
    
    // Set window size based on image size
    int winWidth = qMin(image.width() + 50, 1200);
    int winHeight = qMin(image.height() + 150, 900);
    resize(winWidth, winHeight);
    
    setMouseTracking(true);
}

ZoomWindow::~ZoomWindow()
{
}

void ZoomWindow::paintEvent(QPaintEvent *event)
{
    QMainWindow::paintEvent(event);
    
    QPainter painter(this);
    
    // Calculate position to center the image
    int x = (width() - pixmap.width()) / 2;
    int y = (height() - pixmap.height() - statusBar()->height() - menuBar()->height() - toolbar->height()) / 2;
    y += menuBar()->height() + toolbar->height();
    
    painter.drawPixmap(x, y, pixmap);
}

void ZoomWindow::mouseMoveEvent(QMouseEvent *event)
{
    QString str = QStringLiteral("位置: (") + QString::number(event->x()) + "," +
                  QString::number(event->y()) + ")";
    
    if (brushEnabled) {
        str += QStringLiteral(" | 畫筆模式");
        
        if (drawing) {
            // Draw on the image
            QPainter painter(&image);
            painter.setPen(QPen(brushColor, brushSize, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
            
            int offsetX = (width() - pixmap.width()) / 2;
            int offsetY = (height() - pixmap.height() - statusBar()->height() - menuBar()->height() - toolbar->height()) / 2;
            offsetY += menuBar()->height() + toolbar->height();
            
            QPoint imgPos1 = lastPoint - QPoint(offsetX, offsetY);
            QPoint imgPos2 = event->pos() - QPoint(offsetX, offsetY);
            
            painter.drawLine(imgPos1, imgPos2);
            painter.end();
            
            // Update pixmap
            pixmap = QPixmap::fromImage(image);
            update();
            
            lastPoint = event->pos();
        }
    }
    
    statusLabel->setText(str);
}

void ZoomWindow::mousePressEvent(QMouseEvent *event)
{
    if (brushEnabled && event->button() == Qt::LeftButton) {
        drawing = true;
        lastPoint = event->pos();
    }
}

void ZoomWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if (drawing && event->button() == Qt::LeftButton) {
        drawing = false;
    }
}

void ZoomWindow::saveAs()
{
    QString fileName = QFileDialog::getSaveFileName(
        this,
        QStringLiteral("另存新檔"),
        "",
        QStringLiteral("PNG 圖片 (*.png);;JPEG 圖片 (*.jpg *.jpeg);;BMP 圖片 (*.bmp)")
    );
    
    if (!fileName.isEmpty()) {
        if (image.save(fileName)) {
            QMessageBox::information(this, QStringLiteral("成功"), QStringLiteral("圖片已儲存"));
            statusLabel->setText(QStringLiteral("已儲存至: ") + fileName);
        } else {
            QMessageBox::warning(this, QStringLiteral("錯誤"), QStringLiteral("無法儲存圖片"));
        }
    }
}

void ZoomWindow::toggleBrush()
{
    brushEnabled = !brushEnabled;
    if (brushEnabled) {
        statusLabel->setText(QStringLiteral("畫筆工具已啟用"));
        setCursor(Qt::CrossCursor);
    } else {
        statusLabel->setText(QStringLiteral("畫筆工具已停用"));
        setCursor(Qt::ArrowCursor);
        drawing = false;
    }
}

void ZoomWindow::selectBrushColor()
{
    QColor color = QColorDialog::getColor(brushColor, this, QStringLiteral("選擇畫筆顏色"));
    if (color.isValid()) {
        brushColor = color;
        statusLabel->setText(QStringLiteral("畫筆顏色已更改"));
    }
}

void ZoomWindow::setBrushSize()
{
    bool ok;
    int size = QInputDialog::getInt(
        this,
        QStringLiteral("設定畫筆大小"),
        QStringLiteral("畫筆大小 (像素):"),
        brushSize,
        1,
        100,
        1,
        &ok
    );
    
    if (ok) {
        brushSize = size;
        statusLabel->setText(QStringLiteral("畫筆大小已設定為: ") + QString::number(brushSize));
    }
}
