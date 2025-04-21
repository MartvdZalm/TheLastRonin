#include "mainwindow.h"

#include <QApplication>
#include "styles/AppStyle.h"

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

    app.setStyleSheet(AppStyle::styleSheet());
    app.setWindowIcon(QIcon(":/Images/TheLastRoninIcon"));

    MainWindow window;
    window.setWindowTitle("TheLastRonin");
    window.showMaximized();

    return app.exec();
}
