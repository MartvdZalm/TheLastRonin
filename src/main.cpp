#include <QApplication>

#include "database/DatabaseManager.h"
#include "repository/SettingRepository.h"
#include "service/LanguageService.h"
#include "styles/AppStyle.h"
#include "window/MainWindow.h"
#include "database/ORM.h"
#include "model/Playlist.h"
#include "model/Track.h"
#include "model/Setting.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    app.setStyleSheet(AppStyle::styleSheet());
    app.setWindowIcon(QIcon(":/Images/TheLastRoninIcon"));

    //DatabaseManager& db = DatabaseManager::instance();

    //if (db.openDatabase())
    //{
    //    if (!db.initSchema())
    //    {
    //        qCritical() << "Failed to initialize database schema.";
    //        return -1;
    //    }
    //}
    //else
    //{
    //    qCritical() << "Failed to open the database. Exiting.";
    //    return -1;
    //}

    REGISTER_MODEL(Playlist);
    REGISTER_MODEL(Track);
    REGISTER_MODEL(Setting);

    if (!ORM::instance().initialize())
    {
        qCritical() << "Failed to initialize ORM";
        return -1;
    }

    SettingRepository settingRepository;
    std::unique_ptr<Setting> settingModel = settingRepository.findByKey("language");

    if (settingModel != nullptr)
    {
        LanguageService::instance().loadLanguage(settingModel->getValue());
    }

    MainWindow window;
    window.setWindowTitle("TheLastRonin");
    window.showMaximized();

    return app.exec();
}
