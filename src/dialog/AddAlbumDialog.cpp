#include "AddAlbumDialog.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QDialogButtonBox>
#include <QPushButton>

AddAlbumDialog::AddAlbumDialog(QWidget* parent)
    : QDialog(parent)
{
    setWindowTitle("Add Album");

    titleEdit = new QLineEdit;
    artistEdit = new QLineEdit;
    QPushButton* chooseImageBtn = new QPushButton("Choose Cover Image");
    imagePreview = new QLabel("No image selected");
    imagePreview->setFixedSize(250, 120);
    imagePreview->setStyleSheet("border: 1px solid gray;");
    imagePreview->setAlignment(Qt::AlignCenter);
    releaseDate = new QLineEdit;

    connect(chooseImageBtn, &QPushButton::clicked, this, &AddAlbumDialog::chooseCoverImage);

    QDialogButtonBox* buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(buttons, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);

    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(new QLabel("Title:"));
    layout->addWidget(titleEdit);
    layout->addWidget(new QLabel("Artist:"));
    layout->addWidget(artistEdit);
    layout->addWidget(chooseImageBtn);
    layout->addWidget(imagePreview);
    layout->addWidget(new QLabel("Release Date: "));
    layout->addWidget(releaseDate);
    layout->addWidget(buttons);
    setLayout(layout);
}

QString AddAlbumDialog::getTitle() const
{
    return titleEdit->text();
}

QString AddAlbumDialog::getArtist() const
{
    return artistEdit->text();
}

QString AddAlbumDialog::getCoverPath() const
{
    return coverPath;
}

QString AddAlbumDialog::getReleasDate() const
{
    return releaseDate->text();
}

void AddAlbumDialog::chooseCoverImage()
{
    QString path = QFileDialog::getOpenFileName(this, "Choose Album Cover", "", "Images (*.png *.jpg *.jpeg)");
    if (!path.isEmpty()) {
        coverPath = path;
        QPixmap pix(path);
        imagePreview->setPixmap(pix.scaled(imagePreview->size(), Qt::KeepAspectRatio));
    }
}
