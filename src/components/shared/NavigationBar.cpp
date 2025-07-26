#include "NavigationBar.h"

#include "../../styles/ButtonStyle.h"
#include <QIcon>

NavigationBar::NavigationBar(QWidget* parent) : QWidget(parent)
{
    backButton = new QPushButton(QIcon(":/Images/ArrowBack"), "", this);
    backButton->setStyleSheet(ButtonStyle::styleSheet());

    forwardButton = new QPushButton(QIcon(":/Images/ArrowForward"), "", this);
    forwardButton->setStyleSheet(ButtonStyle::styleSheet());

    backButton->setToolTip("Go back");
    forwardButton->setToolTip("Go forward");

    backButton->setFixedSize(40, 40);
    forwardButton->setFixedSize(40, 40);

    auto layout = new QHBoxLayout(this);
    layout->addWidget(backButton);
    layout->addWidget(forwardButton);
    layout->addStretch();
    layout->setContentsMargins(5, 5, 5, 5);
    layout->setSpacing(5);
    setLayout(layout);

    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

    connect(backButton, &QPushButton::clicked, this, &NavigationBar::backClicked);
    connect(forwardButton, &QPushButton::clicked, this, &NavigationBar::forwardClicked);
}

void NavigationBar::setBackEnabled(bool enabled)
{
    backButton->setEnabled(enabled);
}

void NavigationBar::setForwardEnabled(bool enabled)
{
    forwardButton->setEnabled(enabled);
}
