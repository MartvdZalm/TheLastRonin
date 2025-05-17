#ifndef SPOTIFYSERVICE_H
#define SPOTIFYSERVICE_H

#include <QObject>
#include <QNetworkAccessmanager>
#include <QNetworkReply>
#include <string>

class SpotifyService : public QObject
{
    Q_OBJECT

public:
    SpotifyService(QObject *parent = nullptr);

    std::string getAllPlaylistsByUserId(std::string& userId);
    std::string getPlaylistById(std::string& userId);

public slots:
    void onRequestFinished();

private:
    QNetworkAccessManager *manager;
    QNetworkReply *reply;
};

#endif // SPOTIFYSERVICE_H
