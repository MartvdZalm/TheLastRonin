#include <QApplication>

#include "styles/AppStyle.h"
#include "window/MainWindow.h"
#include "database/DatabaseManager.h"
#include "dao/SettingsDAO.h"
#include "service/LanguageService.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setStyleSheet(AppStyle::styleSheet());
    app.setWindowIcon(QIcon(":/Images/TheLastRoninIcon"));

    DatabaseManager& db = DatabaseManager::instance();

    if (db.openDatabase())
    {
        if (!db.initSchema())
        {
            qCritical() << "Failed to initialize database schema.";
            return -1;
        }
    }
    else
    {
        qCritical() << "Failed to open the database. Exiting.";
        return -1;
    }

    SettingsDAO settingsDAO;
    QString savedLanguage = settingsDAO.getSetting("language");
    if (!savedLanguage.isEmpty())
    {
        LanguageService::instance().loadLanguage(savedLanguage);
    }

    MainWindow window;
    window.setWindowTitle("TheLastRonin");
    window.showMaximized();

    return app.exec();
}
