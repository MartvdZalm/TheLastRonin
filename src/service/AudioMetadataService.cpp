#include "AudioMetadataService.h"

#include <QEventLoop>
#include <QFileInfo>
#include <QMediaPlayer>
#include <QTimer>
#include <QUrl>

std::optional<int> AudioMetadataService::getDuration(const QString& filePath)
{
    QFileInfo fileInfo(filePath);
    if (!fileInfo.exists())
    {
        return std::nullopt;
    }

    QMediaPlayer player;
    QEventLoop loop;
    QTimer timeout;

    timeout.setSingleShot(true);
    timeout.start(3000);

    int duration = 0;
    bool loaded = false;

    QObject::connect(&player, &QMediaPlayer::durationChanged,
                     [&](qint64 dur)
                     {
                         if (dur > 0)
                         {
                             duration = dur / 1000;
                             loaded = true;
                             loop.quit();
                         }
                     });

    QObject::connect(&timeout, &QTimer::timeout, [&]() { loop.quit(); });

    QObject::connect(&player, &QMediaPlayer::errorOccurred, [&]() { loop.quit(); });

    player.setSource(QUrl::fromLocalFile(filePath));
    loop.exec();

    return loaded ? std::optional<int>(duration) : std::nullopt;
}