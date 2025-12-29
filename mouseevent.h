#ifndef MOUSEEVENT_H
#define MOUSEEVENT_H

#include <QMainWindow>

class MouseEvent : public QMainWindow
{
    Q_OBJECT

public:
    MouseEvent(QWidget *parent = nullptr);
    ~MouseEvent();
};
#endif // MOUSEEVENT_H
