#ifndef ADDALBUMDIALOG_H
#define ADDALBUMDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>

class AddAlbumDialog : public QDialog
{
    Q_OBJECT

public:
    AddAlbumDialog(QWidget* parent = nullptr);

    QString getTitle() const;
    QString getArtist() const;
    QString getCoverPath() const;
    QString getReleasDate() const;

private slots:
    void chooseCoverImage();

private:
    QLineEdit* titleEdit;
    QLineEdit* artistEdit;
    QLabel* imagePreview;
    QString coverPath;
    QLineEdit* releaseDate;
};

#endif // ADDALBUMDIALOG_H
