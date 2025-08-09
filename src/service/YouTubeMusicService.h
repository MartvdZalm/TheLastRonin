#ifndef YOUTUBEMUSICSERVICE_H
#define YOUTUBEMUSICSERVICE_H

#include "../model/Track.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QObject>
#include <QProcess>

class YouTubeMusicService : public QObject
{
    Q_OBJECT

  public:
    static YouTubeMusicService& instance();

    void searchMusic(const QString& query);
    void downloadTrack(const QString& url, const QString& outputPath);

  signals:
    void searchResultsReady(const QList<Track>& tracks);
    void downloadProgress(int percentage);
    void downloadFinished(const QString& filePath);
    void error(const QString& message);

  private slots:
    void onSearchFinished(QNetworkReply* reply);
    void onDownloadFinished(int exitCode, QProcess::ExitStatus exitStatus);

  private:
    explicit YouTubeMusicService(QObject* parent = nullptr);
    ~YouTubeMusicService();

    QNetworkAccessManager* networkManager;
    QProcess* downloadProcess;

    void parseSearchResults(const QJsonDocument& json);
    Track parseTrackFromJson(const QJsonObject& obj);
};

#endif // YOUTUBEMUSICSERVICE_H