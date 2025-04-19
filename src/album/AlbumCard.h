#ifndef ALBUMCARD_H
#define ALBUMCARD_H

#include <QWidget>
#include <QLabel>
#include "album.h"

class AlbumCard : public QWidget
{
    Q_OBJECT

public:
    AlbumCard(const Album& album, QWidget* parent = nullptr);

signals:
    void albumClicked(Album album);

protected:
    void mousePressEvent(QMouseEvent* event) override;
    bool eventFilter(QObject* watched, QEvent* event) override;

private:
    Album albumData;
    QLabel* previewLabel;
};

#endif // ALBUMCARD_H
