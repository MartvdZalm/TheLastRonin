#ifndef ADDPLAYLISTDIALOG_H
#define ADDPLAYLISTDIALOG_H

#include "../../model/Playlist.h"
#include <QDialog>
#include <QLabel>
#include <QLineEdit>

class AddPlaylistDialog : public QDialog
{
    Q_OBJECT

  private:
    QLineEdit* nameEdit;
    QLineEdit* descriptionEdit;
    QLabel* imagePreview;
    QString coverImagePath;

  public:
    AddPlaylistDialog(QWidget* parent = nullptr);
    AddPlaylistDialog(const Playlist& existingPlaylist, QWidget* parent = nullptr);

    QString getName() const;
    QString getDescription() const;
    QString getCoverImagePath() const;

  private slots:
    void chooseCoverImage();

  private:
    void setupUI();
    void populateFields(const Playlist& playlist);
};

#endif // ADDPLAYLISTDIALOG_H
