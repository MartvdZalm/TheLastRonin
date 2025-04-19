#ifndef ALBUMPAGE_H
#define ALBUMPAGE_H

#include <QWidget>
#include "../album/Album.h"

class AlbumPage : public QWidget
{
    Q_OBJECT

public:
    explicit AlbumPage(const Album& album, QWidget* parent = nullptr);

private:
    Album albumData;
};

#endif // ALBUMPAGE_H
