#ifndef HOMEWINDOW_H
#define HOMEWINDOW_H

#include "BaseWindow.h"
#include "../components/home/PlaylistGrid.h"
#include "../dao/PlaylistDAO.h"
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
    void showPlaylistDialog();
    void searchPlaylists(const QString& query);

    PlaylistGrid* playlistGrid = nullptr;
    QLineEdit* searchInput = nullptr;
    QPushButton* addPlaylistBtn = nullptr;
    QGridLayout* playlistGridLayout = nullptr;

    PlaylistDAO playlistDAO;
};

#endif // HOMEWINDOW_H
