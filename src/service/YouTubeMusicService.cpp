#include "YouTubeMusicService.h"
#include <QDir>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QProcess>
#include <QStandardPaths>

YouTubeMusicService::YouTubeMusicService(QObject* parent) : QObject(parent)
{
    networkManager = new QNetworkAccessManager(this);
    downloadProcess = new QProcess(this);

    connect(networkManager, &QNetworkAccessManager::finished, this, &YouTubeMusicService::onSearchFinished);
    connect(downloadProcess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this,
            &YouTubeMusicService::onDownloadFinished);
}

YouTubeMusicService::~YouTubeMusicService() {}

YouTubeMusicService& YouTubeMusicService::instance()
{
    static YouTubeMusicService instance;
    return instance;
}

void YouTubeMusicService::searchMusic(const QString& query)
{
    QProcess* process = new QProcess(this);
    QStringList arguments;
    arguments << "--dump-json" << "--no-playlist" << "--max-downloads" << "10";
    arguments << "ytsearch10:" + query;

    connect(process, &QProcess::finished, this,
            [this, process](int exitCode)
            {
                if (exitCode == 0)
                {
                    QByteArray output = process->readAllStandardOutput();
                    QStringList lines = QString::fromUtf8(output).split('\n', Qt::SkipEmptyParts);

                    QList<Track> tracks;
                    for (const QString& line : lines)
                    {
                        if (line.trimmed().isEmpty())
                            continue;

                        QJsonDocument doc = QJsonDocument::fromJson(line.toUtf8());
                        if (doc.isObject())
                        {
                            Track track = parseTrackFromJson(doc.object());
                            tracks.append(track);
                        }
                    }

                    emit searchResultsReady(tracks);
                }
                else
                {
                    emit error("Search failed. Make sure yt-dlp is installed.");
                }

                process->deleteLater();
            });

    process->start("yt-dlp", arguments);
}

void YouTubeMusicService::downloadTrack(const QString& url, const QString& outputPath)
{
    if (downloadProcess->state() == QProcess::Running)
    {
        downloadProcess->terminate();
        downloadProcess->waitForFinished();
    }

    QStringList arguments;
    arguments << "-x" << "--audio-format" << "mp3" << "--audio-quality" << "0";
    arguments << "-o" << outputPath;
    arguments << url;

    downloadProcess->start("yt-dlp", arguments);
}

void YouTubeMusicService::onSearchFinished(QNetworkReply* reply)
{
    reply->deleteLater();

    if (reply->error() != QNetworkReply::NoError)
    {
        emit error("Network error: " + reply->errorString());
        return;
    }

    QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
    if (doc.isNull())
    {
        emit error("Invalid JSON response");
        return;
    }

    parseSearchResults(doc);
}

void YouTubeMusicService::onDownloadFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    if (exitCode == 0)
    {
        emit downloadFinished("Download completed");
    }
    else
    {
        emit error("Download failed");
    }
}

void YouTubeMusicService::parseSearchResults(const QJsonDocument& json)
{
    QList<Track> tracks;

    if (json.isArray())
    {
        QJsonArray array = json.array();
        for (const QJsonValue& value : array)
        {
            if (value.isObject())
            {
                Track track = parseTrackFromJson(value.toObject());
                tracks.append(track);
            }
        }
    }
    else if (json.isObject())
    {
        Track track = parseTrackFromJson(json.object());
        tracks.append(track);
    }

    emit searchResultsReady(tracks);
}

Track YouTubeMusicService::parseTrackFromJson(const QJsonObject& obj)
{
    Track track;
    track.setTitle(obj["title"].toString());
    track.setArtist(obj["uploader"].toString());
    track.setDuration(obj["duration"].toInt());
    track.setFilePath(obj["webpage_url"].toString());

    return track;
}
