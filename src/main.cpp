#include <QApplication>
#include <QIcon>
#include "repository/SettingRepository.h"
#include "repository/PlaylistRepository.h"

// #include "service/LanguageService.h"
#include "styles/AppStyle.h"
// #include "window/MainWindow.h"
#include "database/ORM.h"
#include "model/Playlist.h"
#include "model/Track.h"
#include "model/Setting.h"
#include <iostream>

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    // app.setStyleSheet(AppStyle::styleSheet());
    app.setWindowIcon(QIcon(":/Images/TheLastRoninIcon"));

    if (!ORM::instance().initialize())
    {
        qCritical() << "Failed to initialize ORM";
        return -1;
    }

    SettingRepository settingRepository;
    std::unique_ptr<Setting> settingModel = settingRepository.findByKey("language");
    std::cout << settingModel->getValue().toStdString() << std::endl;

    PlaylistRepository playlistRepository;
    std::vector<std::unique_ptr<Playlist>> playlists = playlistRepository.findAll();

    for (const auto& playlist : playlists) {
        std::cout << playlist->getName().toStdString() << std::endl;
    }

    //std::unique_ptr<Playlist> playlist = playlistRepository.find(4);

    //std::cout << playlist->getName().toStdString() << std::endl;

    //playlist->setName("Updated playlist name");

    //playlistRepository.save(playlist.get());

    //std::unique_ptr<Playlist> playlistnew = playlistRepository.find(4);

    //std::cout << playlistnew->getName().toStdString() << std::endl;

    /*auto playlist = std::make_unique<Playlist>();
    playlist->setName("new Playlist");
    playlist->setDescription("this is a new playlist made with the new code");

    if (playlistRepository.save(playlist.get()))
    {
        qDebug() << "Playlist created with ID:" << playlist->id();
    }
    else
    {
        qDebug() << "Failed to create playlist";
    }*/

    // if (settingModel != nullptr)
    // {
    //     LanguageService::instance().loadLanguage(settingModel->getValue());
    // }



    // MainWindow window;
    // window.setWindowTitle("TheLastRonin");
    // window.showMaximized();

    return app.exec();
}
