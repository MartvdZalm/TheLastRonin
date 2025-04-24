#ifndef HOMEWINDOW_H
#define HOMEWINDOW_H

#include "BaseWindow.h"
#include "../components/home/PlaylistGrid.h"
#include "../dao/PlaylistDAO.h"
#include "../dao/TrackDAO.h"
#include <QPushButton>
#include <QLineEdit>
#include <QGridLayout>
#include <QComboBox>

class HomeWindow : public BaseWindow
{
    Q_OBJECT

public:
    explicit HomeWindow(QWidget* parent = nullptr);

    void setupUI() override;
    void setupConnections() override;
    void setStyle() override;
    void setupEvents() override;

protected:
    void resizeEvent(QResizeEvent* event) override;

private:
    void showPlaylistDialog();
    void importPlaylistFromFolder();
    void searchPlaylists(const QString& query);
    void onSortChanged(const QString& sortBy);
    void onFilterChanged(const QString& genre);
    void updatePlaylistGrid(const QList<Playlist>& playlists);

    PlaylistGrid* playlistGrid = nullptr;
    QLineEdit* searchInput = nullptr;
    QPushButton* addPlaylistBtn = nullptr;
    QPushButton* importPlaylistBtn = nullptr;
    QGridLayout* playlistGridLayout = nullptr;
    QComboBox* sortComboBox = nullptr;

    PlaylistDAO playlistDAO;
    TrackDAO trackDAO;
};

#endif // HOMEWINDOW_H
