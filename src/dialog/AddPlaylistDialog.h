#ifndef ADDPLAYLISTDIALOG_H
#define ADDPLAYLISTDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>

class AddPlaylistDialog : public QDialog
{
    Q_OBJECT

public:
    AddPlaylistDialog(QWidget* parent = nullptr);

    QString getName() const;
    QString getDescription() const;
    QString getCoverImagePath() const;

private slots:
    void chooseCoverImage();

private:
    QLineEdit* nameEdit;
    QLineEdit* descriptionEdit;
    QLabel* imagePreview;
    QString coverImagePath;
};

#endif // ADDPLAYLISTDIALOG_H
