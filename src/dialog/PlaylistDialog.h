#ifndef PLAYLISTDIALOG_H
#define PLAYLISTDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include "../playlist/Playlist.h"

class PlaylistDialog : public QDialog
{
    Q_OBJECT

public:
    PlaylistDialog(QWidget* parent = nullptr);
    PlaylistDialog(const Playlist& existingPlaylist, QWidget* parent = nullptr);

    QString getName() const;
    QString getDescription() const;
    QString getCoverImagePath() const;

private slots:
    void chooseCoverImage();

private:
    void setupUI();
    void populateFields(const Playlist& playlist);

    QLineEdit* nameEdit;
    QLineEdit* descriptionEdit;
    QLabel* imagePreview;
    QString coverImagePath;
};

#endif // PLAYLISTDIALOG_H
