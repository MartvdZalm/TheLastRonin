#ifndef LIBRARYWINDOW_H
#define LIBRARYWINDOW_H

#include "../components/playlist/TrackList.h"
#include "../components/shared/NavigationBar.h"
#include "../components/shared/PlaybackBar.h"
#include "../database/Container.h"
#include "../events/AppEvents.h"
#include "../model/Track.h"
#include "../repository/TrackRepository.h"
#include "../styles/ButtonStyle.h"
#include "../styles/ComboBoxStyle.h"
#include "../styles/InputStyle.h"
#include "BaseWindow.h"
#include <QComboBox>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QPushButton>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QWidget>

class LibraryWindow : public BaseWindow
{
    Q_OBJECT

  public:
    explicit LibraryWindow(QWidget* parent = nullptr);
    ~LibraryWindow();

    void setupUI() override;
    void setupConnections() override;
    void setupEvents() override;

  private slots:
    void onSearchTextChanged(const QString& text);
    void onSortChanged(const QString& sortBy);
    void onFilterChanged(const QString& filterBy);
    void onTrackClicked(QListWidgetItem* item);
    void onAddToPlaylistClicked();
    void onDeleteTrackClicked();
    void onImportTracksClicked();
    void onRefreshLibraryClicked();

  private:
    void loadTracks();
    void updateTrackList();
    void filterTracks();
    void sortTracks();
    QWidget* createTrackItemWidget(const Track& track, int index);
    QString formatDuration(int seconds) const;
    void updateStatusLabel();
    void showStatistics();
    void setupTrackList();
    void setupFilters();

    QLineEdit* searchInput;
    QComboBox* sortComboBox;
    QComboBox* filterComboBox;
    QPushButton* addToPlaylistBtn;
    QPushButton* deleteTrackBtn;
    QPushButton* importTracksBtn;
    QPushButton* refreshLibraryBtn;
    QPushButton* statisticsBtn;
    TrackList* trackList;
    QLabel* statusLabel;
    PlaybackBar* playbackBarWidget;

    QList<Track> allTracks;
    QList<Track> filteredTracks;
    QString currentSearchText;
    QString currentSortBy;
    QString currentFilterBy;
    int currentTrackIndex = 0;
};

#endif // LIBRARYWINDOW_H
