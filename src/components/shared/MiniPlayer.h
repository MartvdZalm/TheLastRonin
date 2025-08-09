#ifndef MINIPLAYER_H
#define MINIPLAYER_H

#include "../../model/Track.h"
#include <QAudioOutput>
#include <QHBoxLayout>
#include <QLabel>
#include <QMediaPlayer>
#include <QMouseEvent>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QPoint>
#include <QPushButton>
#include <QSlider>
#include <QVBoxLayout>
#include <QVideoWidget>
#include <QWidget>

class MiniPlayer : public QWidget
{
    Q_OBJECT

  public:
    explicit MiniPlayer(QWidget* parent = nullptr);
    ~MiniPlayer();

    void setPlayerData(QMediaPlayer* player, QAudioOutput* audioOutput);
    void updateTrackInfo(const Track& track);
    void updateProgress(qint64 position, qint64 duration);
    void updatePlayPauseButton(bool isPlaying);
    void updateVolumeSlider(int volume);
    void setVideoMode(bool enabled);

  signals:
    void playPauseClicked();
    void nextClicked();
    void prevClicked();
    void progressChanged(int position);
    void volumeChanged(int volume);
    void miniPlayerClosed();
    void expandToFullPlayer();

  protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void closeEvent(QCloseEvent* event) override;

  private slots:
    void onPlayPauseClicked();
    void onNextClicked();
    void onPrevClicked();
    void onProgressSliderMoved(int position);
    void onVolumeSliderChanged(int volume);
    void onToggleVideoMode();

  private:
    void setupUI();
    void setupConnections();
    void loadThumbnail(const QString& thumbnailUrl);
    void updateDisplayMode();

    QLabel* songLabel;
    Track track;
    QPushButton* playPauseButton;
    QPushButton* nextButton;
    QPushButton* prevButton;
    QPushButton* closeButton;
    QSlider* progressSlider;
    QSlider* volumeSlider;
    QPushButton* volumeButton;
    QMediaPlayer* playerRef;
    QAudioOutput* audioOutputRef;
    bool isDragging;
    QPoint dragStartPosition;
    QPoint windowStartPosition;
    bool isMuted = false;
    int storedVolume = 50;

    QLabel* thumbnailLabel;
    QVideoWidget* videoWidget;
    QPushButton* toggleVideoButton;
    QNetworkAccessManager* networkManager;
    bool isVideoMode = false;
    QWidget* mediaDisplayWidget;
};

#endif // MINIPLAYER_H
