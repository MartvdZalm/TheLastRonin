#ifndef YOUTUBEMUSICWINDOW_H
#define YOUTUBEMUSICWINDOW_H

#include "../components/playlist/TrackList.h"
#include "../components/shared/NavigationBar.h"
#include "../components/shared/PlaybackBar.h"
#include "../database/Container.h"
#include "../events/AppEvents.h"
#include "../model/Track.h"
#include "../styles/ButtonStyle.h"
#include "../styles/ComboBoxStyle.h"
#include "../styles/InputStyle.h"
#include "BaseWindow.h"
#include <QComboBox>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QProcess>
#include <QPushButton>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QWidget>

class YouTubeMusicWindow : public BaseWindow
{
    Q_OBJECT

  public:
    explicit YouTubeMusicWindow(QWidget* parent = nullptr);
    ~YouTubeMusicWindow();

    void setupUI() override;
    void setupConnections() override;
    void setupEvents() override;

  private slots:
    void onSearchClicked();
    void onSearchTextChanged(const QString& text);
    void onTrackClicked(QListWidgetItem* item);
    void onPlayTrackClicked();
    void onAddToPlaylistClicked();
    void onDownloadTrackClicked();
    void onSearchFinished(QNetworkReply* reply);

  private:
    void searchYouTubeMusic(const QString& query);
    void performSearch(const QString& query);
    void handleSearchFinished(QProcess* process, int exitCode);
    void parseSearchResults(const QJsonDocument& json);
    void updateTrackList(const QList<Track>& tracks);
    QWidget* createTrackItemWidget(const Track& track, int index);
    QString formatDuration(int seconds) const;
    void showError(const QString& message);
    void processJsonLine(const QString& line);

    QLineEdit* searchInput;
    QPushButton* searchBtn;
    QPushButton* playTrackBtn;
    QPushButton* addToPlaylistBtn;
    QPushButton* downloadTrackBtn;
    TrackList* trackList;
    QLabel* statusLabel;
    PlaybackBar* playbackBarWidget;

    QNetworkAccessManager* networkManager;

    QList<Track> searchResults;
    QString currentSearchText;
    int currentTrackIndex = 0;
};

#endif // YOUTUBEMUSICWINDOW_H
