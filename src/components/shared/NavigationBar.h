#ifndef NAVIGATIONBAR_H
#define NAVIGATIONBAR_H

#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QWidget>

class NavigationBar : public QWidget
{
    Q_OBJECT

  public:
    explicit NavigationBar(QWidget* parent = nullptr);

    void setBackEnabled(bool enabled);
    void setForwardEnabled(bool enabled);

  signals:
    void backClicked();
    void forwardClicked();
    void settingsClicked();

  private:
    QPushButton* backButton;
    QPushButton* forwardButton;
    QPushButton* settingsButton;
};

#endif // NAVIGATIONBAR_H
