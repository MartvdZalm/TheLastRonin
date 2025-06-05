#include "SpotifyService.h"

#include <QUrl>
#include <QNetworkRequest>
#include <QNetworkReply>

SpotifyService::SpotifyService(QObject *parent)
: QObject(parent), manager(new QNetworkAccessManager(this))
{
}

// std::string SpotifyService::getAllPlaylistsByUserId(std::string& userId)
// {
//     std::string apiUrl = "https://api.spotify.com/v1/users/" + userId + "/playlists";

//     QUrl url(apiUrl.c_str());

//     QNetworkRequest request(url);
//     QString token = "your-bearer-token";
//     request.setRawHeader("Authorization", "Bearer " + token.toUtf8());

//     reply = manager->get(request);

// }

// std::string SpotifyService::getPlaylistById(std::string& userId)
// {

// }

void SpotifyService::onRequestFinished()
{

}
