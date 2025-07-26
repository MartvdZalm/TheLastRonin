#include "AddTrackDialog.h"

#include "../../model/Track.h"
#include "../../styles/ButtonStyle.h"
#include <QDialogButtonBox>
#include <QFileDialog>
#include <QFileInfo>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>

AddTrackDialog::AddTrackDialog(QWidget* parent) : QDialog(parent)
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
    chooseFileButton = new QPushButton("Choose Audio File...", this);
    chooseFileButton->setStyleSheet(ButtonStyle::primary());

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

    connect(chooseFileButton, &QPushButton::clicked, this,
            [=]()
            {
                QString filePath = QFileDialog::getOpenFileName(
                    this, "Choose Audio File", "",
                    "Audio Files (*.mp3 *.wav);;MP3 Files (*.mp3);;WAV Files (*.wav);;All Files (*)");

                if (!filePath.isEmpty())
                {
                    filePathInput->setText(filePath);
                    selectedFilePath = filePath;

                    if (titleInput->text().isEmpty())
                    {
                        QFileInfo fileInfo(filePath);
                        titleInput->setText(fileInfo.baseName());
                    }
                }
            });

    QDialogButtonBox* buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

    QPushButton* okButton = buttons->button(QDialogButtonBox::Ok);
    QPushButton* cancelButton = buttons->button(QDialogButtonBox::Cancel);

    okButton->setStyleSheet(ButtonStyle::primary());
    cancelButton->setStyleSheet(ButtonStyle::primary());

    connect(buttons, &QDialogButtonBox::accepted, this,
            [=]()
            {
                if (!selectedFilePath.isEmpty())
                {
                    accept();
                }
            });

    connect(cancelButton, &QPushButton::clicked, this, &AddTrackDialog::reject);

    layout->addWidget(buttons);
}

Track AddTrackDialog::getTrack() const
{
    return Track{
        .title = titleInput->text(),
        .filePath = selectedFilePath,
        .artist = artistInput->text(),
    };
}
