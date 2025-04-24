#ifndef BASEWINDOW_H
#define BASEWINDOW_H

#include <QWidget>

class BaseWindow : public QWidget
{
    Q_OBJECT

public:
    BaseWindow(QWidget* parent = nullptr);
    ~BaseWindow();

    virtual void setupUI() = 0;
    virtual void setupConnections() = 0;
    virtual void setStyle() = 0;
    virtual void setupEvents() = 0;
};

#endif // BASEWINDOW_H
