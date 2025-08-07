#ifndef HOMEWINDOW_H
#define HOMEWINDOW_H

#include "../components/home/PlaylistGrid.h"
#include "../service/TrackService.h"
#include "BaseWindow.h"
#include <QComboBox>
#include <QGridLayout>
#include <QLineEdit>
#include <QPushButton>

class HomeWindow : public BaseWindow
{
    Q_OBJECT

  public:
    explicit HomeWindow(QWidget* parent = nullptr);

    void setupUI() override;
    void setupConnections() override;
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
    QPushButton* deleteDataBtn = nullptr;
    QGridLayout* playlistGridLayout = nullptr;
    QComboBox* sortComboBox = nullptr;

    TrackService trackService;
};

#endif // HOMEWINDOW_H
