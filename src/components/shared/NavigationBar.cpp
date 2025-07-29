#include "NavigationBar.h"

#include "../../styles/ButtonStyle.h"
#include <QIcon>

NavigationBar::NavigationBar(QWidget* parent) : QWidget(parent)
{
    backButton = new QPushButton(QIcon(":/Images/ArrowBack"), "", this);
    backButton->setStyleSheet(ButtonStyle::primary());
    backButton->setToolTip("Go back");
    backButton->setFixedSize(40, 40);

    forwardButton = new QPushButton(QIcon(":/Images/ArrowForward"), "", this);
    forwardButton->setStyleSheet(ButtonStyle::primary());
    forwardButton->setToolTip("Go forward");
    forwardButton->setFixedSize(40, 40);

    settingsButton = new QPushButton(QIcon(":/Images/Settings"), "", this);
    settingsButton->setStyleSheet(ButtonStyle::primary());
    settingsButton->setToolTip("Settings");
    settingsButton->setFixedSize(40, 40);

    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->addWidget(backButton);
    layout->addWidget(forwardButton);
    layout->addStretch();
    layout->addWidget(settingsButton);

    layout->setContentsMargins(5, 5, 5, 5);
    layout->setSpacing(5);
    setLayout(layout);

    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

    connect(backButton, &QPushButton::clicked, this, &NavigationBar::backClicked);
    connect(forwardButton, &QPushButton::clicked, this, &NavigationBar::forwardClicked);
    connect(settingsButton, &QPushButton::clicked, this, &NavigationBar::settingsClicked);
}

void NavigationBar::setBackEnabled(bool enabled)
{
    backButton->setEnabled(enabled);
}

void NavigationBar::setForwardEnabled(bool enabled)
{
    forwardButton->setEnabled(enabled);
}
