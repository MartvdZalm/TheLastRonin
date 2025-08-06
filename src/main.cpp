#include <QApplication>
#include <QIcon>
#include <QStandardPaths>
#include <QDir>
#include <QSqlError>
#include <qsqlquery.h>

#include "database/DatabaseManager.h"
#include "database/Container.h"
#include "styles/AppStyle.h"
#include "window/MainWindow.h"
#include "service/LanguageService.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    app.setStyleSheet(AppStyle::styleSheet());
    app.setWindowIcon(QIcon(":/Images/TheLastRoninIcon"));

    if (!DatabaseManager::instance().initialize()) {
        qFatal("Failed to initialize database");
        return -1;
    }

    Container::instance().initialize(DatabaseManager::instance().database());

    QString language = Container::instance().getSettingRepository()->getValue("language", "English");
    LanguageService::instance().loadLanguage(language);

    MainWindow window;
    window.setWindowTitle("TheLastRonin");
    window.showMaximized();

    return app.exec();
}
