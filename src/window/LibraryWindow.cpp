#include "LibraryWindow.h"

#include "../components/dialog/AddTrackDialog.h"
#include "../database/Container.h"
#include "../events/AppEvents.h"
#include "../styles/ButtonStyle.h"
#include "../styles/ComboBoxStyle.h"
#include "../styles/InputStyle.h"
#include "../styles/ListStyle.h"
#include <QDir>
#include <QDirIterator>
#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>
#include <QSet>
#include <QTimer>

LibraryWindow::LibraryWindow(QWidget* parent) : BaseWindow(parent)
{
    setWindowTitle(tr("Music Library"));
    setupUI();
    setupConnections();
    setupEvents();
    loadTracks();
}

LibraryWindow::~LibraryWindow() {}

void LibraryWindow::setupUI()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(15);

    NavigationBar* navBar = new NavigationBar(this);
    mainLayout->addWidget(navBar);

    QHBoxLayout* searchFilterLayout = new QHBoxLayout;

    searchInput = new QLineEdit(this);
    searchInput->setPlaceholderText(tr("Search tracks..."));
    searchInput->setClearButtonEnabled(true);
    searchInput->setStyleSheet(InputStyle::primary());
    searchFilterLayout->addWidget(searchInput);

    filterComboBox = new QComboBox(this);
    filterComboBox->addItem(tr("All Tracks"));
    filterComboBox->addItem(tr("By Artist"));
    filterComboBox->addItem(tr("By Album"));
    filterComboBox->setStyleSheet(ComboBoxStyle::primary());
    searchFilterLayout->addWidget(filterComboBox);

    sortComboBox = new QComboBox(this);
    sortComboBox->addItem(tr("Sort by Title"));
    sortComboBox->addItem(tr("Sort by Artist"));
    sortComboBox->addItem(tr("Sort by Album"));
    sortComboBox->addItem(tr("Sort by Duration"));
    sortComboBox->addItem(tr("Sort by Date Added"));
    sortComboBox->setStyleSheet(ComboBoxStyle::primary());
    searchFilterLayout->addWidget(sortComboBox);

    searchFilterLayout->addStretch();
    mainLayout->addLayout(searchFilterLayout);

    QHBoxLayout* actionLayout = new QHBoxLayout;

    importTracksBtn = new QPushButton(tr("Import Tracks"), this);
    importTracksBtn->setStyleSheet(ButtonStyle::primary());
    actionLayout->addWidget(importTracksBtn);

    addToPlaylistBtn = new QPushButton(tr("Add to Playlist"), this);
    addToPlaylistBtn->setStyleSheet(ButtonStyle::primary());
    addToPlaylistBtn->setEnabled(false);
    actionLayout->addWidget(addToPlaylistBtn);

    deleteTrackBtn = new QPushButton(tr("Delete Track"), this);
    deleteTrackBtn->setStyleSheet(ButtonStyle::remove());
    deleteTrackBtn->setEnabled(false);
    actionLayout->addWidget(deleteTrackBtn);

    actionLayout->addStretch();
    mainLayout->addLayout(actionLayout);

    statusLabel = new QLabel(tr("Loading tracks..."), this);
    statusLabel->setStyleSheet("color: #888; font-size: 12px;");
    mainLayout->addWidget(statusLabel);

    trackList = new TrackList(QList<Track>(), this);
    mainLayout->addWidget(trackList, 1);

    playbackBarWidget = PlaybackBar::instance();
}

void LibraryWindow::setupConnections()
{
    connect(searchInput, &QLineEdit::textChanged, this, &LibraryWindow::onSearchTextChanged);
    connect(sortComboBox, &QComboBox::currentTextChanged, this, &LibraryWindow::onSortChanged);
    connect(filterComboBox, &QComboBox::currentTextChanged, this, &LibraryWindow::onFilterChanged);
    connect(trackList, &QListWidget::itemClicked, this, &LibraryWindow::onTrackClicked);
    connect(trackList, &QListWidget::itemSelectionChanged, this,
            [this]()
            {
                bool hasSelection = !trackList->selectedItems().isEmpty();
                addToPlaylistBtn->setEnabled(hasSelection);
                deleteTrackBtn->setEnabled(hasSelection);
            });

    connect(addToPlaylistBtn, &QPushButton::clicked, this, &LibraryWindow::onAddToPlaylistClicked);
    connect(deleteTrackBtn, &QPushButton::clicked, this, &LibraryWindow::onDeleteTrackClicked);
    connect(importTracksBtn, &QPushButton::clicked, this, &LibraryWindow::onImportTracksClicked);
}

void LibraryWindow::setupEvents()
{
    connect(&AppEvents::instance(), &AppEvents::playlistChanged, this, [this]() { loadTracks(); });
}

void LibraryWindow::loadTracks()
{
    allTracks = Container::instance().getTrackRepository()->findAll();
    filterTracks();
    updateTrackList();
    updateStatusLabel();
}

void LibraryWindow::updateTrackList()
{
    if (trackList)
    {
        trackList->updateTracks(filteredTracks);
    }
}

void LibraryWindow::filterTracks()
{
    filteredTracks.clear();

    for (const Track& track : allTracks)
    {
        bool matchesSearch = currentSearchText.isEmpty() ||
                             track.getTitle().contains(currentSearchText, Qt::CaseInsensitive) ||
                             track.getArtist().contains(currentSearchText, Qt::CaseInsensitive) ||
                             track.getAlbum().contains(currentSearchText, Qt::CaseInsensitive);

        bool matchesFilter = true;
        if (currentFilterBy == tr("By Artist") && !track.getArtist().isEmpty())
        {
            matchesFilter = true;
        }
        else if (currentFilterBy == tr("By Album") && !track.getAlbum().isEmpty())
        {
            matchesFilter = true;
        }

        if (matchesSearch && matchesFilter)
        {
            filteredTracks.append(track);
        }
    }

    sortTracks();
    if (trackList && statusLabel)
    {
        updateTrackList();
    }
}

void LibraryWindow::sortTracks()
{
    if (currentSortBy == tr("Sort by Artist"))
    {
        std::sort(filteredTracks.begin(), filteredTracks.end(),
                  [](const Track& a, const Track& b) { return a.getArtist() < b.getArtist(); });
    }
    else if (currentSortBy == tr("Sort by Album"))
    {
        std::sort(filteredTracks.begin(), filteredTracks.end(),
                  [](const Track& a, const Track& b) { return a.getAlbum() < b.getAlbum(); });
    }
    else if (currentSortBy == tr("Sort by Duration"))
    {
        std::sort(filteredTracks.begin(), filteredTracks.end(),
                  [](const Track& a, const Track& b) { return a.getDuration() < b.getDuration(); });
    }
    else if (currentSortBy == tr("Sort by Date Added"))
    {
        std::sort(filteredTracks.begin(), filteredTracks.end(),
                  [](const Track& a, const Track& b) { return a.getCreatedAt() < b.getCreatedAt(); });
    }
    else
    {
        std::sort(filteredTracks.begin(), filteredTracks.end(),
                  [](const Track& a, const Track& b) { return a.getTitle() < b.getTitle(); });
    }
}

void LibraryWindow::updateStatusLabel()
{
    if (!statusLabel)
        return;

    int totalTracks = allTracks.size();
    int filteredTracksCount = filteredTracks.size();

    if (totalTracks == filteredTracksCount)
    {
        statusLabel->setText(tr("Showing %1 tracks").arg(totalTracks));
    }
    else
    {
        statusLabel->setText(tr("Showing %1 of %2 tracks").arg(filteredTracksCount).arg(totalTracks));
    }
}

QString LibraryWindow::formatDuration(int seconds) const
{
    if (seconds <= 0)
        return "--:--";

    int minutes = seconds / 60;
    seconds = seconds % 60;

    return QString("%1:%2").arg(minutes).arg(seconds, 2, 10, QChar('0'));
}

void LibraryWindow::onSearchTextChanged(const QString& text)
{
    currentSearchText = text;
    filterTracks();
    updateStatusLabel();
}

void LibraryWindow::onSortChanged(const QString& sortBy)
{
    currentSortBy = sortBy;
    sortTracks();
    updateTrackList();
}

void LibraryWindow::onFilterChanged(const QString& filterBy)
{
    currentFilterBy = filterBy;
    filterTracks();
    updateTrackList();
    updateStatusLabel();
}

void LibraryWindow::onTrackClicked(QListWidgetItem* item)
{
    if (!item)
        return;

    Track track = item->data(Qt::UserRole).value<Track>();
    currentTrackIndex = trackList->row(item);

    Playlist tempPlaylist;
    tempPlaylist.setName(tr("Library"));
    tempPlaylist.setTracks(filteredTracks);

    playbackBarWidget->updatePlaylist(tempPlaylist);
    playbackBarWidget->playTrackAtIndex(currentTrackIndex);
}

void LibraryWindow::onAddToPlaylistClicked()
{
    QList<QListWidgetItem*> selectedItems = trackList->selectedItems();
    if (selectedItems.isEmpty())
        return;
}

void LibraryWindow::onDeleteTrackClicked()
{
    QList<QListWidgetItem*> selectedItems = trackList->selectedItems();
    if (selectedItems.isEmpty())
        return;

    QMessageBox::StandardButton reply = QMessageBox::question(
        this, tr("Confirm Deletion"),
        tr("Are you sure you want to delete %1 selected track(s)? This cannot be undone.").arg(selectedItems.size()),
        QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes)
    {
        for (QListWidgetItem* item : selectedItems)
        {
            Track track = item->data(Qt::UserRole).value<Track>();
            Container::instance().getTrackRepository()->deleteById(track.getId());
        }
        loadTracks();
    }
}

void LibraryWindow::onImportTracksClicked()
{
    QStringList filePaths = QFileDialog::getOpenFileNames(this, tr("Import Tracks"), "",
                                                          tr("Audio Files (*.mp3 *.wav *.flac);;MP3 Files (*.mp3);;WAV "
                                                             "Files (*.wav);;FLAC Files (*.flac);;All Files (*)"));

    if (filePaths.isEmpty())
        return;

    int importedCount = 0;
    for (const QString& filePath : filePaths)
    {
        QFileInfo fileInfo(filePath);

        Track track;
        track.setTitle(fileInfo.baseName());
        track.setFilePath(filePath);
        track.setCreatedAt(QDateTime::currentDateTime());
        track.setUpdatedAt(QDateTime::currentDateTime());
        auto savedTrack = trackService.addTrack(track);

        if (savedTrack)
        {
            importedCount++;
        }
    }

    if (importedCount > 0)
    {
        loadTracks();
        QMessageBox::information(this, tr("Import Complete"),
                                 tr("Successfully imported %1 track(s).").arg(importedCount));
    }
}