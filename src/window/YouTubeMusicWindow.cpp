#include "YouTubeMusicWindow.h"

#include "../components/dialog/AddTrackDialog.h"
#include "../database/Container.h"
#include "../events/AppEvents.h"
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
#include <QProcess>
#include <QStandardPaths>
#include <QTimer>
#include <QUrlQuery>

YouTubeMusicWindow::YouTubeMusicWindow(QWidget* parent) : BaseWindow(parent)
{
    setWindowTitle(tr("YouTube Music"));
    networkManager = new QNetworkAccessManager(this);
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

    connect(networkManager, &QNetworkAccessManager::finished, this, &YouTubeMusicWindow::onSearchFinished);
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
        showError(tr("Please enter a search term"));
        return;
    }

    statusLabel->setText(tr("Searching..."));
    searchYouTubeMusic(query);
}

void YouTubeMusicWindow::searchYouTubeMusic(const QString& query)
{
    QProcess* checkProcess = new QProcess(this);

    connect(checkProcess, &QProcess::readyReadStandardOutput, this,
            [checkProcess]() { QByteArray output = checkProcess->readAllStandardOutput(); });

    connect(checkProcess, &QProcess::readyReadStandardError, this,
            [checkProcess]() { QByteArray error = checkProcess->readAllStandardError(); });

    checkProcess->start("yt-dlp", QStringList() << "--version");

    connect(checkProcess, &QProcess::finished, this,
            [this, query, checkProcess](int exitCode)
            {
                checkProcess->deleteLater();

                if (exitCode != 0)
                {
                    qDebug() << "yt-dlp not found";
                    return;
                }

                performSearch(query);
            });

    connect(checkProcess, &QProcess::errorOccurred, this,
            [this, query, checkProcess](QProcess::ProcessError error)
            {
                qDebug() << "Check process error:" << error;
                checkProcess->deleteLater();
            });

    QTimer* checkTimeout = new QTimer(this);
    checkTimeout->setSingleShot(true);
    connect(checkTimeout, &QTimer::timeout, this,
            [this, query, checkProcess, checkTimeout]()
            {
                qDebug() << "Check process timed out";
                if (checkProcess->state() == QProcess::Running)
                {
                    checkProcess->terminate();
                    checkProcess->waitForFinished(1000);
                }
                checkTimeout->deleteLater();
            });
    checkTimeout->start(3000);

    connect(checkProcess, &QProcess::finished, this,
            [checkTimeout](int exitCode)
            {
                Q_UNUSED(exitCode)
                if (checkTimeout)
                {
                    checkTimeout->stop();
                    checkTimeout->deleteLater();
                }
            });
}

void YouTubeMusicWindow::performSearch(const QString& query)
{
    qDebug() << "Performing real search for:" << query;

    QProcess* process = new QProcess(this);
    QStringList arguments;
    arguments << "--dump-json"
              << "--no-playlist"
              << "--max-downloads" << "20"
              << "--extractor-args" << "youtube:skip=dash"
              << "--no-warnings"
              << "--quiet"
              << "ytsearch20:" + query;

    qDebug() << "yt-dlp arguments:" << arguments;

    QString* accumulatedOutput = new QString();

    connect(process, &QProcess::readyReadStandardOutput, this,
            [this, process, accumulatedOutput]()
            {
                QByteArray newOutput = process->readAllStandardOutput();
                QString outputStr = QString::fromUtf8(newOutput);
                accumulatedOutput->append(outputStr);

                QStringList lines = accumulatedOutput->split('\n');

                QString lastLine = lines.takeLast();
                *accumulatedOutput = lastLine;

                for (const QString& line : lines)
                {
                    if (line.trimmed().isEmpty())
                        continue;

                    processJsonLine(line.trimmed());
                }

                if (!searchResults.isEmpty())
                {
                    statusLabel->setText(tr("Found %1 results so far...").arg(searchResults.size()));
                }
            });

    connect(process, &QProcess::readyReadStandardError, this,
            [process]()
            {
                QByteArray error = process->readAllStandardError();
                QString errorStr = QString::fromUtf8(error);
                if (!errorStr.contains("WARNING") && !errorStr.trimmed().isEmpty())
                {
                    qDebug() << "Search stderr (non-warning):" << errorStr;
                }
            });

    connect(process, &QProcess::finished, this,
            [this, process, accumulatedOutput](int exitCode)
            {
                qDebug() << "Search process finished with exit code:" << exitCode;

                if (!accumulatedOutput->trimmed().isEmpty())
                {
                    processJsonLine(accumulatedOutput->trimmed());
                }

                delete accumulatedOutput;

                updateTrackList(searchResults);
                if (searchResults.isEmpty())
                {
                    if (exitCode == 0)
                    {
                        statusLabel->setText(tr("No results found. Try a different search term."));
                    }
                    else
                    {
                        statusLabel->setText(tr("Search failed. Please try again."));
                    }
                }
                else
                {
                    statusLabel->setText(tr("Found %1 results").arg(searchResults.size()));
                }

                process->deleteLater();
            });

    connect(process, &QProcess::errorOccurred, this,
            [this, process, accumulatedOutput](QProcess::ProcessError error)
            {
                QString errorMsg = tr("Process error: %1").arg(error);
                qDebug() << "Process error:" << errorMsg;
                showError(errorMsg);
                delete accumulatedOutput;
                process->deleteLater();
            });

    QTimer* timeoutTimer = new QTimer(this);
    timeoutTimer->setSingleShot(true);
    connect(timeoutTimer, &QTimer::timeout, this,
            [this, process, timeoutTimer, accumulatedOutput]()
            {
                qDebug() << "Search process timed out after 90 seconds";

                if (!searchResults.isEmpty())
                {
                    qDebug() << "Have" << searchResults.size() << "results, stopping gracefully";
                    if (process->state() == QProcess::Running)
                    {
                        process->terminate();
                        process->waitForFinished(3000);
                    }
                    statusLabel->setText(tr("Search completed with %1 results (timed out)").arg(searchResults.size()));
                }
                else
                {
                    if (process->state() == QProcess::Running)
                    {
                        process->terminate();
                        process->waitForFinished(5000);
                        if (process->state() == QProcess::Running)
                        {
                            process->kill();
                        }
                    }
                    showError(tr("Search timed out with no results. Try a simpler search term."));
                }

                delete accumulatedOutput;
                timeoutTimer->deleteLater();
            });
    timeoutTimer->start(90000);

    searchResults.clear();

    process->start("yt-dlp", arguments);

    connect(process, &QProcess::finished, this,
            [timeoutTimer](int exitCode)
            {
                Q_UNUSED(exitCode)
                if (timeoutTimer)
                {
                    timeoutTimer->stop();
                    timeoutTimer->deleteLater();
                }
            });
}

void YouTubeMusicWindow::processJsonLine(const QString& line)
{
    if (line.trimmed().isEmpty())
        return;

    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(line.toUtf8(), &parseError);

    if (parseError.error == QJsonParseError::NoError && doc.isObject())
    {
        QJsonObject obj = doc.object();

        Track track;
        track.setTitle(obj["title"].toString());

        QString artist = obj["uploader"].toString();
        if (artist.isEmpty())
            artist = obj["channel"].toString();
        track.setArtist(artist);

        track.setDuration(obj["duration"].toInt());
        track.setFilePath(obj["webpage_url"].toString());

        QString thumbnailUrl;

        if (obj.contains("thumbnail"))
        {
            thumbnailUrl = obj["thumbnail"].toString();
        }

        track.setThumbnailUrl(thumbnailUrl);

        if (!track.getTitle().isEmpty() && !track.getFilePath().isEmpty())
        {
            searchResults.append(track);
            qDebug() << "Added track:" << track.getTitle() << "by" << track.getArtist();

            updateTrackList(searchResults);
        }
    }
    else
    {
        qDebug() << "JSON parse error:" << parseError.errorString() << "for line:" << line.left(100);
    }
}

void YouTubeMusicWindow::handleSearchFinished(QProcess* process, int exitCode)
{
    QByteArray output = process->readAllStandardOutput();
    QByteArray errorOutput = process->readAllStandardError();

    if (exitCode == 0)
    {
        if (output.isEmpty())
        {
            statusLabel->setText(tr("No output from search. Check yt-dlp installation."));
            process->deleteLater();
            return;
        }

        QStringList lines = QString::fromUtf8(output).split('\n', Qt::SkipEmptyParts);
        qDebug() << "Found" << lines.size() << "lines of output";

        searchResults.clear();
        int validTracks = 0;

        for (int i = 0; i < lines.size(); ++i)
        {
            const QString& line = lines[i];
            if (line.trimmed().isEmpty())
                continue;

            QJsonParseError parseError;
            QJsonDocument doc = QJsonDocument::fromJson(line.toUtf8(), &parseError);
            if (parseError.error == QJsonParseError::NoError && doc.isObject())
            {
                QJsonObject obj = doc.object();

                Track track;
                track.setTitle(obj["title"].toString());
                track.setArtist(obj["uploader"].toString().isEmpty() ? obj["channel"].toString()
                                                                     : obj["uploader"].toString());
                track.setDuration(obj["duration"].toInt());
                track.setFilePath(obj["webpage_url"].toString());

                if (!track.getTitle().isEmpty())
                {
                    searchResults.append(track);
                    validTracks++;
                    qDebug() << "Added track:" << track.getTitle() << "by" << track.getArtist();
                }
            }
            else
            {
                qDebug() << "JSON parse error:" << parseError.errorString() << "for line:" << line.left(100);
            }
        }

        qDebug() << "Total valid tracks found:" << validTracks;
        updateTrackList(searchResults);

        if (searchResults.isEmpty())
        {
            statusLabel->setText(tr("No valid results found. Try a different search term."));
        }
        else
        {
            statusLabel->setText(tr("Found %1 results").arg(searchResults.size()));
        }
    }
    else
    {
        QString errorMsg = tr("Search failed (exit code: %1)").arg(exitCode);
        if (!errorOutput.isEmpty())
        {
            errorMsg += "\n\nError details:\n" + QString::fromUtf8(errorOutput);
        }
        qDebug() << "Search failed:" << errorMsg;
        showError(errorMsg);
    }

    process->deleteLater();
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

    QProcess* process = new QProcess(this);
    QStringList arguments = ytDlpArgs;
    arguments << "--no-post-overwrites"
              << "--no-warnings"
              << "-o" << fileName;
    arguments << track.getFilePath();

    connect(process, &QProcess::finished, this,
            [this, process, track, fileName](int exitCode)
            {
                if (exitCode == 0)
                {
                    QMessageBox::information(this, tr("Download Complete"),
                                             tr("Successfully downloaded '%1'").arg(track.getTitle()));
                    statusLabel->setText(tr("Download complete"));
                }
                else
                {
                    QByteArray errorOutput = process->readAllStandardError();
                    QString errorMsg = tr("Download failed. Make sure yt-dlp is installed.");

                    QString errorStr = QString::fromUtf8(errorOutput);
                    if (errorStr.contains("ffmpeg not found") || errorStr.contains("ffprobe not found"))
                    {
                        errorMsg = tr("Download completed but ffmpeg is not installed for post-processing.\n\n"
                                      "The file was downloaded but may not be in the requested format.\n\n"
                                      "To get proper format conversion, please install ffmpeg:\n\n"
                                      "Windows: Download from https://ffmpeg.org/download.html\n"
                                      "macOS: brew install ffmpeg\n"
                                      "Linux: sudo apt install ffmpeg");
                    }
                    else if (!errorOutput.isEmpty())
                    {
                        errorMsg += "\n\nError details:\n" + QString::fromUtf8(errorOutput);
                    }
                    showError(errorMsg);
                }

                process->deleteLater();
            });

    connect(process, &QProcess::errorOccurred, this,
            [this, process](QProcess::ProcessError error)
            {
                QString errorMsg;
                switch (error)
                {
                case QProcess::FailedToStart:
                    errorMsg = tr("Failed to start download. Make sure yt-dlp is installed.");
                    break;
                case QProcess::Crashed:
                    errorMsg = tr("Download process crashed.");
                    break;
                case QProcess::Timedout:
                    errorMsg = tr("Download timed out.");
                    break;
                default:
                    errorMsg = tr("Download failed with unknown error.");
                    break;
                }
                showError(errorMsg);
                process->deleteLater();
            });

    process->start("yt-dlp", arguments);
}

void YouTubeMusicWindow::onSearchFinished(QNetworkReply* reply)
{
    reply->deleteLater();

    if (reply->error() != QNetworkReply::NoError)
    {
        showError(tr("Network error: %1").arg(reply->errorString()));
        return;
    }

    QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
    if (doc.isNull())
    {
        showError(tr("Invalid JSON response"));
        return;
    }

    parseSearchResults(doc);
}

void YouTubeMusicWindow::parseSearchResults(const QJsonDocument& json) {}

void YouTubeMusicWindow::showError(const QString& message)
{
    statusLabel->setText(message);
    QMessageBox::warning(this, tr("Error"), message);
}

void YouTubeMusicWindow::onSearchTextChanged(const QString& text)
{
    currentSearchText = text;
}
