#ifndef ADDTRACKDIALOG_H
#define ADDTRACKDIALOG_H

#include "../../model/Track.h"
#include <QDialog>
#include <QLineEdit>

class AddTrackDialog : public QDialog
{
    Q_OBJECT

  private:
    QLineEdit* titleInput;
    QLineEdit* artistInput;
    QLineEdit* filePathInput;
    QPushButton* chooseFileButton;
    QString selectedFilePath;

  public:
    AddTrackDialog(QWidget* parent = nullptr);
    Track getTrack() const;

  private:
    void setupUI();
};

#endif // ADDTRACKDIALOG_H
