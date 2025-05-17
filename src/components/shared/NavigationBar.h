#ifndef NAVIGATIONBAR_H
#define NAVIGATIONBAR_H

#include <QWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QLabel>

class NavigationBar : public QWidget
{
    Q_OBJECT

public:
    explicit NavigationBar(QWidget* parent = nullptr);

    void setTitle(const QString& title);
    void setBackEnabled(bool enabled);
    void setForwardEnabled(bool enabled);

signals:
    void backClicked();
    void forwardClicked();

private:
    QPushButton* backButton;
    QPushButton* forwardButton;
    QLabel* titleLabel;
};

#endif // NAVIGATIONBAR_H
