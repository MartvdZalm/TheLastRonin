#ifndef SPOTIFYSERVICE_H
#define SPOTIFYSERVICE_H

#include <string>

class SpotifyService
{
public:
    SpotifyService();

    std::string getAllPlaylistsByUserId(std::string& userId);
    std::string getPlaylistById(std::string& userId);
};

#endif // SPOTIFYSERVICE_H
