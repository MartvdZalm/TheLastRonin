#ifndef AUDIOMETADATASERVICE_H
#define AUDIOMETADATASERVICE_H

#include <QString>
#include <optional>

class AudioMetadataService
{
  public:
    std::optional<int> getDuration(const QString& filePath);
};

#endif // AUDIOMETADATASERVICE_H
