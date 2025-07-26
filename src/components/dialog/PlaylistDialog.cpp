#include "PlaylistDialog.h"

#include "../../styles/ButtonStyle.h"
#include <QDialogButtonBox>
#include <QFileDialog>
#include <QPixmap>
#include <QPushButton>
#include <QVBoxLayout>

PlaylistDialog::PlaylistDialog(QWidget* parent) : QDialog(parent)
{
    setupUI();
}

PlaylistDialog::PlaylistDialog(const Playlist& existingPlaylist, QWidget* parent) : QDialog(parent)
{
    setupUI();
    populateFields(existingPlaylist);
}

void PlaylistDialog::setupUI()
{
    setWindowTitle("Add Playlist");

    this->setFixedWidth(350);

    nameEdit = new QLineEdit;
    descriptionEdit = new QLineEdit;

    QPushButton* chooseImageBtn = new QPushButton("Choose Cover Image");
    chooseImageBtn->setStyleSheet(ButtonStyle::styleSheet());

    QHBoxLayout* imageRow = new QHBoxLayout;
    imageRow->addWidget(chooseImageBtn);
    imageRow->addStretch();

    imagePreview = new QLabel("No image selected");
    imagePreview->setFixedSize(250, 120);
    imagePreview->setStyleSheet("border: 1px solid gray;");
    imagePreview->setAlignment(Qt::AlignCenter);

    connect(chooseImageBtn, &QPushButton::clicked, this, &PlaylistDialog::chooseCoverImage);

    QDialogButtonBox* buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

    QPushButton* okButton = buttons->button(QDialogButtonBox::Ok);
    QPushButton* cancelButton = buttons->button(QDialogButtonBox::Cancel);

    okButton->setStyleSheet(ButtonStyle::styleSheet());
    cancelButton->setStyleSheet(ButtonStyle::styleSheet());

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

void PlaylistDialog::populateFields(const Playlist& playlist)
{
    nameEdit->setText(playlist.name);
    descriptionEdit->setText(playlist.description);
    coverImagePath = playlist.coverImagePath;
    imagePreview->setPixmap(QPixmap(coverImagePath).scaled(imagePreview->size(), Qt::KeepAspectRatio));
}

QString PlaylistDialog::getName() const
{
    return nameEdit->text();
}

QString PlaylistDialog::getDescription() const
{
    return descriptionEdit->text();
}

QString PlaylistDialog::getCoverImagePath() const
{
    return coverImagePath;
}

void PlaylistDialog::chooseCoverImage()
{
    QString path = QFileDialog::getOpenFileName(this, "Choose Playlist Cover", "", "Images (*.png *.jpg *.jpeg)");
    if (!path.isEmpty())
    {
        coverImagePath = path;
        QPixmap pix(path);
        imagePreview->setPixmap(pix.scaled(imagePreview->size(), Qt::KeepAspectRatio));
    }
}
