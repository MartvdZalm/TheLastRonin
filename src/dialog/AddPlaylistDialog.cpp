#include "AddPlaylistDialog.h"

#include <QVBoxLayout>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QFileDialog>
#include <QPixmap>

AddPlaylistDialog::AddPlaylistDialog(QWidget* parent)
    : QDialog(parent)
{
    setWindowTitle("Add Playlist");

    this->setFixedWidth(350);

    nameEdit = new QLineEdit;
    descriptionEdit = new QLineEdit;

    QPushButton* chooseImageBtn = new QPushButton("Choose Cover Image");
    QHBoxLayout* imageRow = new QHBoxLayout;
    imageRow->addWidget(chooseImageBtn);
    imageRow->addStretch();

    imagePreview = new QLabel("No image selected");
    imagePreview->setFixedSize(250, 120);
    imagePreview->setStyleSheet("border: 1px solid gray;");
    imagePreview->setAlignment(Qt::AlignCenter);

    connect(chooseImageBtn, &QPushButton::clicked, this, &AddPlaylistDialog::chooseCoverImage);

    QDialogButtonBox* buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(buttons, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);

    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(new QLabel("Playlist Name:"));
    layout->addWidget(nameEdit);
    layout->addWidget(new QLabel("Description:"));
    layout->addWidget(descriptionEdit);
    layout->addLayout(imageRow);
    layout->addWidget(imagePreview);
    layout->addWidget(buttons);
    setLayout(layout);
}

QString AddPlaylistDialog::getName() const
{
    return nameEdit->text();
}

QString AddPlaylistDialog::getDescription() const
{
    return descriptionEdit->text();
}

QString AddPlaylistDialog::getCoverImagePath() const
{
    return coverImagePath;
}

void AddPlaylistDialog::chooseCoverImage()
{
    QString path = QFileDialog::getOpenFileName(this, "Choose Playlist Cover", "", "Images (*.png *.jpg *.jpeg)");
    if (!path.isEmpty()) {
        coverImagePath = path;
        QPixmap pix(path);
        imagePreview->setPixmap(pix.scaled(imagePreview->size(), Qt::KeepAspectRatio));
    }
}
