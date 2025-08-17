#include "YouTubeMusicWindow.h"

#include "../components/dialog/AddTrackDialog.h"
#include "../core/Logger.h"
#include "../database/Container.h"
#include "../events/AppEvents.h"
#include "../manager/ProcessManager.h"
#include "../styles/ButtonStyle.h"
#include "../styles/ComboBoxStyle.h"
#include "../styles/InputStyle.h"
#include "../styles/ListStyle.h"
#include <QDebug>
#include <QDir>
#include <QFileDialog>
#include <QInputDialog>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QPointer>
#include <QProcess>
#include <QStandardPaths>
#include <QTimer>
#include <QUrlQuery>

YouTubeMusicWindow::YouTubeMusicWindow(QWidget* parent) : BaseWindow(parent)
{
    setWindowTitle(tr("YouTube Music"));
    // networkManager = new QNetworkAccessManager(this);
    setupUI();
    setupConnections();
    setupEvents();
}

YouTubeMusicWindow::~YouTubeMusicWindow() {}

void YouTubeMusicWindow::setupUI()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(15);

    NavigationBar* navBar = new NavigationBar(this);
    mainLayout->addWidget(navBar);

    QHBoxLayout* searchLayout = new QHBoxLayout;

    searchInput = new QLineEdit(this);
    searchInput->setPlaceholderText(tr("Search YouTube Music..."));
    searchInput->setClearButtonEnabled(true);
    searchInput->setStyleSheet(InputStyle::primary());
    searchLayout->addWidget(searchInput);

    searchBtn = new QPushButton(tr("Search"), this);
    searchBtn->setStyleSheet(ButtonStyle::primary());
    searchLayout->addWidget(searchBtn);

    mainLayout->addLayout(searchLayout);

    QHBoxLayout* actionLayout = new QHBoxLayout;

    playTrackBtn = new QPushButton(tr("Play Track"), this);
    playTrackBtn->setStyleSheet(ButtonStyle::primary());
    playTrackBtn->setEnabled(false);
    actionLayout->addWidget(playTrackBtn);

    addToPlaylistBtn = new QPushButton(tr("Add to Playlist"), this);
    addToPlaylistBtn->setStyleSheet(ButtonStyle::primary());
    addToPlaylistBtn->setEnabled(false);
    actionLayout->addWidget(addToPlaylistBtn);

    downloadTrackBtn = new QPushButton(tr("Download"), this);
    downloadTrackBtn->setStyleSheet(ButtonStyle::primary());
    downloadTrackBtn->setEnabled(false);
    actionLayout->addWidget(downloadTrackBtn);

    actionLayout->addStretch();
    mainLayout->addLayout(actionLayout);

    statusLabel = new QLabel(tr("Search for music on YouTube..."), this);
    statusLabel->setStyleSheet("color: #888; font-size: 12px;");
    mainLayout->addWidget(statusLabel);

    trackList = new TrackList(QList<Track>(), this);
    mainLayout->addWidget(trackList, 1);

    playbackBarWidget = PlaybackBar::instance();
}

void YouTubeMusicWindow::setupConnections()
{
    connect(searchBtn, &QPushButton::clicked, this, &YouTubeMusicWindow::onSearchClicked);
    connect(searchInput, &QLineEdit::returnPressed, this, &YouTubeMusicWindow::onSearchClicked);
    connect(trackList, &QListWidget::itemClicked, this, &YouTubeMusicWindow::onTrackClicked);
    connect(trackList, &QListWidget::itemSelectionChanged, this,
            [this]()
            {
                bool hasSelection = !trackList->selectedItems().isEmpty();
                playTrackBtn->setEnabled(hasSelection);
                addToPlaylistBtn->setEnabled(hasSelection);
                downloadTrackBtn->setEnabled(hasSelection);
            });

    connect(playTrackBtn, &QPushButton::clicked, this, &YouTubeMusicWindow::onPlayTrackClicked);
    connect(addToPlaylistBtn, &QPushButton::clicked, this, &YouTubeMusicWindow::onAddToPlaylistClicked);
    connect(downloadTrackBtn, &QPushButton::clicked, this, &YouTubeMusicWindow::onDownloadTrackClicked);

    // connect(networkManager, &QNetworkAccessManager::finished, this, &YouTubeMusicWindow::onSearchFinished);
}

void YouTubeMusicWindow::setupEvents()
{
    connect(&AppEvents::instance(), &AppEvents::playlistChanged, this,
            [this]()
            {
               
            });
}

void YouTubeMusicWindow::onSearchClicked()
{
    QString query = searchInput->text().trimmed();
    if (query.isEmpty())
    {
        return;
    }

    if (QStandardPaths::findExecutable("yt-dlp").isEmpty())
    {
        LOG_ERROR("yt-dlp missing please follow the instructions in the readme on how to install.");
        return;
    }

    performSearch(query);
}

void YouTubeMusicWindow::performSearch(const QString& query)
{
    statusLabel->setText(tr("Searching..."));

    QStringList arguments;
    arguments << "--dump-json"
              << "--no-playlist"
              << "--max-downloads" << "15"
              << "--extractor-args" << "youtube:skip=dash"
              << "--no-warnings"
              << "--quiet"
              << "ytsearch20:" + query;

    LOG_INFO("yt-dlp arguments: " + arguments.join(", "));

    QProcess* process = ProcessManager::instance().createProcess("yt-dlp", arguments);

    QString* accumulatedOutput = new QString();
    QPointer<YouTubeMusicWindow> safeThis = this;

    connect(process, &QProcess::readyReadStandardOutput, this,
            [safeThis, process, accumulatedOutput]()
            {
                if (!safeThis)
                    return;

                QString outputStr = QString::fromUtf8(process->readAllStandardOutput());
                accumulatedOutput->append(outputStr);

                QStringList lines = accumulatedOutput->split('\n');

                QString lastLine = lines.takeLast();
                *accumulatedOutput = lastLine;

                for (const QString& line : lines)
                {
                    if (line.trimmed().isEmpty())
                        continue;

                    safeThis->processJsonLine(line.trimmed());
                }

                if (!safeThis->searchResults.isEmpty())
                {
                    safeThis->statusLabel->setText(
                        tr("Found %1 results so far...").arg(safeThis->searchResults.size()));
                }
            });

    connect(process, &QProcess::finished, this,
            [safeThis, process, accumulatedOutput](int exitCode)
            {
                if (!safeThis)
                {
                    delete accumulatedOutput;
                    return;
                }

                LOG_INFO("Search process finished with exit code: " + exitCode);

                if (!accumulatedOutput->trimmed().isEmpty())
                {
                    safeThis->processJsonLine(accumulatedOutput->trimmed());
                }

                delete accumulatedOutput;

                safeThis->updateTrackList(safeThis->searchResults);
                if (safeThis->searchResults.isEmpty())
                {
                    if (exitCode == 0)
                    {
                        safeThis->statusLabel->setText(tr("No results found. Try a different search term."));
                    }
                    else
                    {
                        safeThis->statusLabel->setText(tr("Search failed. Please try again."));
                    }
                }
                else
                {
                    safeThis->statusLabel->setText(tr("Found %1 results").arg(safeThis->searchResults.size()));
                }
            });

    searchResults.clear();

    process->start();
}

void YouTubeMusicWindow::processJsonLine(const QString& line)
{
    if (line.trimmed().isEmpty())
        return;

    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(line.toUtf8(), &parseError);

    if (parseError.error == QJsonParseError::NoError && doc.isObject())
    {
        QJsonObject jsonObject = doc.object();

        Track track;
        track.setTitle(jsonObject["title"].toString());

        QString artist = jsonObject["uploader"].toString();

        if (artist.isEmpty())
            artist = jsonObject["channel"].toString();

        track.setArtist(artist);

        track.setDuration(jsonObject["duration"].toInt());
        track.setFilePath(jsonObject["webpage_url"].toString());

        QString thumbnailUrl;

        if (jsonObject.contains("thumbnail"))
        {
            thumbnailUrl = jsonObject["thumbnail"].toString();
        }

        track.setThumbnailUrl(thumbnailUrl);

        if (!track.getTitle().isEmpty() && !track.getFilePath().isEmpty())
        {
            searchResults.append(track);
            updateTrackList(searchResults);
        }
    }
    else
    {
        LOG_ERROR("JSON parse error: " + parseError.errorString() + " for line: " + line.left(100));
    }
}

void YouTubeMusicWindow::updateTrackList(const QList<Track>& tracks)
{
    if (trackList)
    {
        trackList->updateTracks(tracks);
    }
}

void YouTubeMusicWindow::onTrackClicked(QListWidgetItem* item)
{
    if (!item)
        return;

    Track track = item->data(Qt::UserRole).value<Track>();
    currentTrackIndex = trackList->row(item);

    Playlist tempPlaylist;
    tempPlaylist.setName(tr("YouTube Music"));
    tempPlaylist.setTracks(searchResults);

    playbackBarWidget->updatePlaylist(tempPlaylist);
    playbackBarWidget->playTrackAtIndex(currentTrackIndex);
}

void YouTubeMusicWindow::onPlayTrackClicked()
{
    QList<QListWidgetItem*> selectedItems = trackList->selectedItems();
    if (selectedItems.isEmpty())
        return;

    onTrackClicked(selectedItems.first());
}

void YouTubeMusicWindow::onAddToPlaylistClicked()
{
    QList<QListWidgetItem*> selectedItems = trackList->selectedItems();
    if (selectedItems.isEmpty())
        return;

    Track track = selectedItems.first()->data(Qt::UserRole).value<Track>();
}

void YouTubeMusicWindow::onDownloadTrackClicked()
{
    QList<QListWidgetItem*> selectedItems = trackList->selectedItems();
    if (selectedItems.isEmpty())
        return;

    Track track = selectedItems.first()->data(Qt::UserRole).value<Track>();

    QString downloadDir = QStandardPaths::writableLocation(QStandardPaths::MusicLocation);

    QStringList formats;
    formats << tr("MP3 Audio (*.mp3)") << tr("MP4 Video (*.mp4)") << tr("All Files (*)");

    QString selectedFormat =
        QInputDialog::getItem(this, tr("Choose Format"), tr("Select download format:"), formats, 0, false);

    if (selectedFormat.isEmpty())
        return;

    QString fileExtension;
    QStringList ytDlpArgs;

    if (selectedFormat.contains("MP3"))
    {
        fileExtension = ".mp3";
        ytDlpArgs << "-x" << "--audio-format" << "mp3" << "--audio-quality" << "0" << "--extract-audio";
    }
    else if (selectedFormat.contains("MP4"))
    {
        fileExtension = ".mp4";
        ytDlpArgs << "--format" << "best[ext=mp4]/best";
    }
    else
    {
        fileExtension = ".mp4";
        ytDlpArgs << "--format" << "best";
    }

    QString fileName =
        QFileDialog::getSaveFileName(this, tr("Save Track"), downloadDir + "/" + track.getTitle() + fileExtension,
                                     selectedFormat + ";;All Files (*)");

    if (fileName.isEmpty())
        return;

    if (!fileName.endsWith(fileExtension, Qt::CaseInsensitive))
    {
        fileName += fileExtension;
    }

    statusLabel->setText(tr("Downloading %1...").arg(track.getTitle()));

    QStringList arguments = ytDlpArgs;
    arguments << "--no-post-overwrites"
              << "--no-warnings"
              << "-o" << fileName;
    arguments << track.getFilePath();

    QProcess* process = ProcessManager::instance().createProcess("yt-dlp", arguments);
    QPointer<YouTubeMusicWindow> safeThis = this;

    connect(process, &QProcess::finished, this,
            [safeThis, process, track, fileName](int exitCode)
            {
                if (!safeThis)
                    return;

                if (exitCode == 0)
                {
                    QMessageBox::information(nullptr, tr("Download Complete"),
                                             tr("Successfully downloaded '%1'").arg(track.getTitle()));
                    safeThis->statusLabel->setText(tr("Download complete"));
                }
                else
                {
                    LOG_ERROR("Download failed. Make sure yt-dlp is installed.");
                }
            });

    process->start();
}
