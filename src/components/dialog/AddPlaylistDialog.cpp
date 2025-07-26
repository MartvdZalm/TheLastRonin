#include "AddPlaylistDialog.h"

#include "../../styles/ButtonStyle.h"
#include "../../styles/InputStyle.h"
#include <QDialogButtonBox>
#include <QFileDialog>
#include <QPixmap>
#include <QPushButton>
#include <QVBoxLayout>

AddPlaylistDialog::AddPlaylistDialog(QWidget* parent) : QDialog(parent)
{
    this->setWindowTitle("Add Playlist");
    this->setFixedWidth(350);
    this->setupUI();
}

AddPlaylistDialog::AddPlaylistDialog(const Playlist& existingPlaylist, QWidget* parent) : QDialog(parent)
{
    this->setupUI();
    this->populateFields(existingPlaylist);
}

void AddPlaylistDialog::setupUI()
{
    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(10);

    mainLayout->addWidget(new QLabel("Playlist name:"));
    nameEdit = new QLineEdit();
    nameEdit->setStyleSheet(InputStyle::primary());
    mainLayout->addWidget(nameEdit);

    mainLayout->addWidget(new QLabel("Description (Optional):"));
    descriptionEdit = new QLineEdit();
    descriptionEdit->setStyleSheet(InputStyle::primary());
    mainLayout->addWidget(descriptionEdit);

    QHBoxLayout* imageRow = new QHBoxLayout();
    QPushButton* chooseImageBtn = new QPushButton("Choose");
    chooseImageBtn->setStyleSheet(ButtonStyle::primary());
    imageRow->addWidget(chooseImageBtn);
    imageRow->addStretch();
    mainLayout->addLayout(imageRow);

    imagePreview = new QLabel("No image selected");
    imagePreview->setFixedSize(250, 120);
    imagePreview->setStyleSheet("border: 1px solid gray;");
    imagePreview->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(imagePreview);

    connect(chooseImageBtn, &QPushButton::clicked, this, &AddPlaylistDialog::chooseCoverImage);

    QDialogButtonBox* buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    QPushButton* okButton = buttons->button(QDialogButtonBox::Ok);
    okButton->setStyleSheet(ButtonStyle::primary());
    QPushButton* cancelButton = buttons->button(QDialogButtonBox::Cancel);
    cancelButton->setStyleSheet(ButtonStyle::primary());

    connect(buttons, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);

    mainLayout->addWidget(buttons);

    this->setLayout(mainLayout);
}

void AddPlaylistDialog::populateFields(const Playlist& playlist)
{
    nameEdit->setText(playlist.name);
    descriptionEdit->setText(playlist.description);
    coverImagePath = playlist.coverImagePath;
    imagePreview->setPixmap(QPixmap(coverImagePath).scaled(imagePreview->size(), Qt::KeepAspectRatio));
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
    if (!path.isEmpty())
    {
        coverImagePath = path;
        QPixmap pix(path);
        imagePreview->setPixmap(pix.scaled(imagePreview->size(), Qt::KeepAspectRatio));
    }
}
