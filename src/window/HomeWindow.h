#ifndef HOMEWINDOW_H
#define HOMEWINDOW_H

#include "BaseWindow.h"
#include "../playlist/PlaylistManager.h"
#include <QPushButton>
#include <QLineEdit>
#include <QGridLayout>

class HomeWindow : public BaseWindow
{
    Q_OBJECT

public:
    explicit HomeWindow(QWidget* parent = nullptr);

    void setupUI() override;
    void setupConnections() override;
    void reload() override;
    void setStyle() override;

private:
    PlaylistManager playlistManager;

    QLineEdit* searchInput = nullptr;
    QPushButton* addPlaylistBtn = nullptr;
    QGridLayout* playlistGridLayout = nullptr;
};

#endif // HOMEWINDOW_H
