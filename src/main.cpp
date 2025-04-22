#include <QApplication>

#include "styles/AppStyle.h"
#include "window/HomeWindow.h"
#include "database/DatabaseManager.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setStyleSheet(AppStyle::styleSheet());
    app.setWindowIcon(QIcon(":/Images/TheLastRoninIcon"));

    DatabaseManager& db = DatabaseManager::instance();

    if (db.openDatabase()) {
        if (!db.initSchema()) {
            qCritical() << "Failed to initialize database schema.";
            return -1;
        }
    } else {
        qCritical() << "Failed to open the database. Exiting.";
        return -1;
    }

    HomeWindow window;
    window.setWindowTitle("TheLastRonin");
    window.showMaximized();

    return app.exec();
}
