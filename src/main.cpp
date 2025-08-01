#include <QApplication>

#include "database/DatabaseManager.h"
#include "repository/SettingsRepository.h"
#include "service/LanguageService.h"
#include "styles/AppStyle.h"
#include "window/MainWindow.h"

int main(int argc, char* argv[])
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

    SettingsRepository SettingsRepository;
    QString savedLanguage = SettingsRepository.getSetting("language");
    if (!savedLanguage.isEmpty())
    {
        LanguageService::instance().loadLanguage(savedLanguage);
    }

    MainWindow window;
    window.setWindowTitle("TheLastRonin");
    window.showMaximized();

    return app.exec();
}
