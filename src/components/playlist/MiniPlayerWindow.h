#ifndef MINIPLAYERWINDOW_H
#define MINIPLAYERWINDOW_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QSlider>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QMouseEvent>
#include <QPoint>
#include "../../model/Track.h"

class MiniPlayerWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MiniPlayerWindow(QWidget* parent = nullptr);
    ~MiniPlayerWindow();

    void setPlayerData(QMediaPlayer* player, QAudioOutput* audioOutput);
    void updateTrackInfo(const Track &track);
    void updateProgress(qint64 position, qint64 duration);
    void updatePlayPauseButton(bool isPlaying);

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

private:
    void setupUI();
    void setStyle();
    void styleButton(QPushButton* button, const QString& iconPath = "");
    void updateBackgroundStyle(const QString& imagePath);

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
};

#endif // MINIPLAYERWINDOW_H
