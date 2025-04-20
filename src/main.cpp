#include "mainwindow.h"

#include <QApplication>
#include "styles/ButtonStyle.h"
#include "styles/InputStyle.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    DatabaseManager& db = DatabaseManager::instance();
    if (db.openDatabase()) {
        db.initSchema();
    } else {
        qCritical() << "Failed to open the database. Exiting.";
        return -1;
    }

    QString globalStyle = InputStyle::styleSheet() + ButtonStyle::styleSheet();
    app.setStyleSheet(globalStyle);

    MainWindow window;
    window.setWindowTitle("TheLastRonin");
    window.setWindowIcon(QIcon(":/Images/TheLastRoninIcon"));
    window.showMaximized();

    return app.exec();
}
