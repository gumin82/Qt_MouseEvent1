#include "mouseevent.h"
#include "zoomwindow.h"
#include <QDebug>
#include <QInputDialog>
#include <QMessageBox>

MouseEvent::MouseEvent(QWidget *parent)
    : QMainWindow(parent), rubberBand(nullptr), zoomRatio(2.0)
{
    setWindowTitle(QStringLiteral("測試滑鼠事件 - 圖像檢視器"));
    
    // Create menu bar
    QMenuBar *menuBar = new QMenuBar(this);
    QMenu *fileMenu = menuBar->addMenu(QStringLiteral("檔案"));
    QMenu *viewMenu = menuBar->addMenu(QStringLiteral("檢視"));
    
    // File menu actions
    QAction *loadAction = new QAction(QStringLiteral("開啟圖片"), this);
    connect(loadAction, &QAction::triggered, this, &MouseEvent::loadImage);
    fileMenu->addAction(loadAction);
    
    // View menu actions
    QAction *zoomAction = new QAction(QStringLiteral("設定放大倍率"), this);
    connect(zoomAction, &QAction::triggered, this, &MouseEvent::setZoomRatio);
    viewMenu->addAction(zoomAction);
    
    setMenuBar(menuBar);
    
    // Status bar labels
    statusLabel = new QLabel;
    statusLabel->setText(QStringLiteral("指標位置"));
    statusLabel->setFixedWidth(100);
    
    MousePosLabel = new QLabel;
    MousePosLabel->setText(tr(" "));
    MousePosLabel->setFixedWidth(100);
    
    grayValueLabel = new QLabel;
    grayValueLabel->setText(QStringLiteral("灰階值: -"));
    grayValueLabel->setFixedWidth(150);
    
    statusBar()->addPermanentWidget(statusLabel);
    statusBar()->addPermanentWidget(MousePosLabel);
    statusBar()->addPermanentWidget(grayValueLabel);
    
    setMouseTracking(true);
    resize(800, 600);
}

MouseEvent::~MouseEvent()
{
    if (rubberBand) {
        delete rubberBand;
    }
}
void MouseEvent::mouseMoveEvent(QMouseEvent *event)
{
    QString str = "(" + QString::number(event->x()) + "," +
                  QString::number(event->y()) + ")";
    MousePosLabel->setText(str);
    
    // Display grayscale value if cursor is over image
    if (!image.isNull() && imageRect.contains(event->pos())) {
        int imgX = event->x() - imageRect.x();
        int imgY = event->y() - imageRect.y();
        
        if (imgX >= 0 && imgX < image.width() && imgY >= 0 && imgY < image.height()) {
            QRgb pixel = image.pixel(imgX, imgY);
            int gray = qGray(pixel);
            grayValueLabel->setText(QStringLiteral("灰階值: ") + QString::number(gray));
        }
    } else {
        grayValueLabel->setText(QStringLiteral("灰階值: -"));
    }
    
    // Update rubber band if dragging
    if (rubberBand && rubberBand->isVisible()) {
        rubberBand->setGeometry(QRect(origin, event->pos()).normalized());
    }
}
void MouseEvent::mousePressEvent(QMouseEvent *event)
{
    QString str = "(" + QString::number(event->x()) + "," +
                  QString::number(event->y()) + ")";
    if (event->button() == Qt::LeftButton) {
        statusBar()->showMessage(QStringLiteral("左鍵:") + str);
        
        // Start rubber band selection if over image
        if (!image.isNull() && imageRect.contains(event->pos())) {
            origin = event->pos();
            if (!rubberBand) {
                rubberBand = new QRubberBand(QRubberBand::Rectangle, this);
            }
            rubberBand->setGeometry(QRect(origin, QSize()));
            rubberBand->show();
        }
    } else if (event->button() == Qt::RightButton) {
        statusBar()->showMessage(QStringLiteral("右鍵:") + str);
    } else if (event->button() == Qt::MiddleButton) {
        statusBar()->showMessage(QStringLiteral("中鍵:") + str);
    }
    qDebug() << "按壓";
}
void MouseEvent::mouseReleaseEvent(QMouseEvent *event)
{
    QString str = "(" + QString::number(event->x()) + "," +
                  QString::number(event->y()) + ")";
    statusBar()->showMessage(QStringLiteral("釋放:") + str);
    qDebug() << "釋放";
    
    // Process rubber band selection
    if (rubberBand && rubberBand->isVisible()) {
        rubberBand->hide();
        
        QRect selectedRect = rubberBand->geometry().normalized();
        
        // Check if selection is valid (minimum size)
        if (selectedRect.width() > 5 && selectedRect.height() > 5 && !image.isNull()) {
            // Calculate the selected region in image coordinates
            int imgX = selectedRect.x() - imageRect.x();
            int imgY = selectedRect.y() - imageRect.y();
            int imgW = selectedRect.width();
            int imgH = selectedRect.height();
            
            // Clamp to image boundaries
            if (imgX < 0) imgX = 0;
            if (imgY < 0) imgY = 0;
            if (imgX + imgW > image.width()) imgW = image.width() - imgX;
            if (imgY + imgH > image.height()) imgH = image.height() - imgY;
            
            if (imgW > 0 && imgH > 0) {
                // Extract the selected region
                QImage selectedImage = image.copy(imgX, imgY, imgW, imgH);
                
                // Scale it according to zoom ratio
                QImage zoomedImage = selectedImage.scaled(
                    selectedImage.width() * zoomRatio,
                    selectedImage.height() * zoomRatio,
                    Qt::KeepAspectRatio,
                    Qt::SmoothTransformation
                );
                
                // Open zoom window
                ZoomWindow *zoomWindow = new ZoomWindow(zoomedImage, this);
                zoomWindow->setAttribute(Qt::WA_DeleteOnClose);
                zoomWindow->show();
            }
        }
    }
}
void MouseEvent::mouseDoubleClickEvent(QMouseEvent *event)
{
    QString str = "(" + QString::number(event->x()) + "," +
                  QString::number(event->y()) + ")";
    statusBar()->showMessage(QStringLiteral("雙擊:") + str);
    qDebug() << "雙擊";
}

void MouseEvent::paintEvent(QPaintEvent *event)
{
    QMainWindow::paintEvent(event);
    
    if (!pixmap.isNull()) {
        QPainter painter(this);
        
        // Calculate position to center the image
        int x = (width() - pixmap.width()) / 2;
        int y = (height() - pixmap.height() - statusBar()->height() - menuBar()->height()) / 2;
        y += menuBar()->height();
        
        imageRect = QRect(x, y, pixmap.width(), pixmap.height());
        painter.drawPixmap(imageRect, pixmap);
    }
}

void MouseEvent::loadImage()
{
    QString fileName = QFileDialog::getOpenFileName(
        this,
        QStringLiteral("開啟圖片"),
        "",
        QStringLiteral("圖片檔案 (*.png *.jpg *.jpeg *.bmp *.gif)")
    );
    
    if (!fileName.isEmpty()) {
        image.load(fileName);
        if (!image.isNull()) {
            // Scale image to fit window if too large
            int maxWidth = width() - 50;
            int maxHeight = height() - statusBar()->height() - menuBar()->height() - 50;
            
            if (image.width() > maxWidth || image.height() > maxHeight) {
                pixmap = QPixmap::fromImage(image.scaled(maxWidth, maxHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation));
            } else {
                pixmap = QPixmap::fromImage(image);
            }
            
            update();
            statusBar()->showMessage(QStringLiteral("已載入圖片: ") + fileName);
        } else {
            QMessageBox::warning(this, QStringLiteral("錯誤"), QStringLiteral("無法載入圖片"));
        }
    }
}

void MouseEvent::setZoomRatio()
{
    bool ok;
    double ratio = QInputDialog::getDouble(
        this,
        QStringLiteral("設定放大倍率"),
        QStringLiteral("放大倍率:"),
        zoomRatio,
        0.5,
        10.0,
        1,
        &ok
    );
    
    if (ok) {
        zoomRatio = ratio;
        statusBar()->showMessage(QStringLiteral("放大倍率已設定為: ") + QString::number(zoomRatio));
    }
}
