#include "AddTrackDialog.h"

#include <QVBoxLayout>
#include <QLineEdit>
#include <QDialogButtonBox>
#include <QLabel>
#include <QPushButton>
#include <QFileInfo>
#include <QFileDialog>
#include "../playlist/Track.h"

AddTrackDialog::AddTrackDialog(QWidget* parent)
    : QDialog(parent)
{
    setupUI();
    this->setWindowTitle("Add Track");
    this->setFixedWidth(500);
}

void AddTrackDialog::setupUI()
{
    titleInput = new QLineEdit(this);
    artistInput = new QLineEdit(this);
    filePathInput = new QLineEdit(this);
    filePathInput->setReadOnly(true);
    chooseFileButton = new QPushButton("Choose MP3...", this);

    QPushButton* addButton = new QPushButton("Add", this);
    QPushButton* cancelButton = new QPushButton("Cancel", this);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(new QLabel("Title:"));
    layout->addWidget(titleInput);
    layout->addWidget(new QLabel("Artist:"));
    layout->addWidget(artistInput);
    layout->addWidget(new QLabel("File:"));

    QHBoxLayout* fileRow = new QHBoxLayout;
    fileRow->addWidget(filePathInput);
    fileRow->addWidget(chooseFileButton);
    layout->addLayout(fileRow);

    QHBoxLayout* buttonRow = new QHBoxLayout;
    buttonRow->addWidget(addButton);
    buttonRow->addWidget(cancelButton);
    layout->addLayout(buttonRow);

    connect(chooseFileButton, &QPushButton::clicked, this, [=]() {
        QString filePath = QFileDialog::getOpenFileName(this, "Choose MP3", "", "MP3 Files (*.mp3)");
        if (!filePath.isEmpty()) {
            filePathInput->setText(filePath);
            selectedFilePath = filePath;

            if (titleInput->text().isEmpty()) {
                QFileInfo fileInfo(filePath);
                titleInput->setText(fileInfo.baseName());
            }
        }
    });

    connect(addButton, &QPushButton::clicked, this, [=]() {
        if (!selectedFilePath.isEmpty()) {
            accept();
        }
    });

    connect(cancelButton, &QPushButton::clicked, this, &AddTrackDialog::reject);
}

Track AddTrackDialog::getTrack() const
{
    return Track {
        .title = titleInput->text(),
        .filePath = selectedFilePath,
        .artist = artistInput->text(),
    };
}
