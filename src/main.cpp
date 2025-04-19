#include "mainwindow.h"

#include <QApplication>
#include "styles/ButtonStyle.h"
#include "styles/InputStyle.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QString globalStyle = InputStyle::styleSheet() + ButtonStyle::styleSheet();
    app.setStyleSheet(globalStyle);

    MainWindow w;
    w.showMaximized();

    return app.exec();
}
