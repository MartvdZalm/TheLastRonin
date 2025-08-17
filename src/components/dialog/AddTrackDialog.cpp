#include "AddTrackDialog.h"

#include "../../model/Track.h"
#include "../../styles/ButtonStyle.h"
#include "../../styles/InputStyle.h"
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
    this->setWindowTitle(tr("Add Track"));
    this->setFixedWidth(500);
}

void AddTrackDialog::setupUI()
{
    titleInput = new QLineEdit(this);
    titleInput->setStyleSheet(InputStyle::primary());

    artistInput = new QLineEdit(this);
    artistInput->setStyleSheet(InputStyle::primary());

    filePathInput = new QLineEdit(this);
    filePathInput->setReadOnly(true);
    filePathInput->setStyleSheet(InputStyle::primary());

    chooseFileButton = new QPushButton(tr("Choose Audio File..."), this);
    chooseFileButton->setStyleSheet(ButtonStyle::primary());

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(new QLabel(tr("Title:")));
    layout->addWidget(titleInput);
    layout->addWidget(new QLabel(tr("Artist:")));
    layout->addWidget(artistInput);
    layout->addWidget(new QLabel(tr("File:")));

    QHBoxLayout* fileRow = new QHBoxLayout;
    fileRow->addWidget(filePathInput);
    fileRow->addWidget(chooseFileButton);
    layout->addLayout(fileRow);

    connect(chooseFileButton, &QPushButton::clicked, this,
            [=]()
            {
                QString filePath =
                    QFileDialog::getOpenFileName(this, tr("Choose Audio File"), "",
                                                 tr("Audio Files (*.mp3 *.wav *.mp4 *.m4a);;MP3 Files (*.mp3);;WAV "
                                                    "Files (*.wav);;MP4 Files (*.mp4 *.m4a);;All Files (*)"));

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
    Track track;
    track.setTitle(titleInput->text());
    track.setFilePath(selectedFilePath);
    track.setArtist(artistInput->text());
    return track;
}
