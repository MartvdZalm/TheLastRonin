#include "BaseWindow.h"
#include "../styles/AppStyle.h"

BaseWindow::BaseWindow(QWidget* parent) : QWidget(parent)
{
    AppStyle::styleSheet();
}

BaseWindow::~BaseWindow() {}
