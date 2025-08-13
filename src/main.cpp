#include <QApplication>
#include <QDir>
#include <QIcon>
#include <QSqlError>
#include <QStandardPaths>
#include <qsqlquery.h>

#include "core/Logger.h"
#include "database/Container.h"
#include "database/DatabaseManager.h"
#include "manager/LanguageManager.h"
#include "styles/AppStyle.h"
#include "window/MainWindow.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    app.setStyleSheet(AppStyle::styleSheet());
    app.setWindowIcon(QIcon(":/Images/TheLastRoninIcon"));

    Logger::initialize("TheLastRonin");

    if (!DatabaseManager::instance().initialize())
    {
        qFatal("Failed to initialize database");
        return -1;
    }

    Container::instance().initialize(DatabaseManager::instance().database());

    auto setting = Container::instance().getSettingRepository()->setValue("language", "English");
    LanguageManager::instance().loadLanguage(setting->getValue());

    MainWindow window;
    window.setWindowTitle("TheLastRonin");
    window.showMaximized();

    return app.exec();
}
