#include "NavigationBar.h"
#include <QIcon>

NavigationBar::NavigationBar(QWidget* parent)
    : QWidget(parent)
{
    backButton = new QPushButton(QIcon(":/Images/ArrowBack"), "", this);
    forwardButton = new QPushButton(QIcon(":/Images/ArrowForward"), "", this);
    titleLabel = new QLabel("Title", this);

    backButton->setToolTip("Go back");
    forwardButton->setToolTip("Go forward");

    auto layout = new QHBoxLayout(this);
    layout->addWidget(backButton);
    layout->addWidget(forwardButton);
    layout->addStretch();
    layout->addWidget(titleLabel);
    layout->addStretch();

    layout->setContentsMargins(5, 5, 5, 5);
    setLayout(layout);

    connect(backButton, &QPushButton::clicked, this, &NavigationBar::backClicked);
    connect(forwardButton, &QPushButton::clicked, this, &NavigationBar::forwardClicked);
}

void NavigationBar::setTitle(const QString& title)
{
    titleLabel->setText(title);
}

void NavigationBar::setBackEnabled(bool enabled)
{
    backButton->setEnabled(enabled);
}

void NavigationBar::setForwardEnabled(bool enabled)
{
    forwardButton->setEnabled(enabled);
}
