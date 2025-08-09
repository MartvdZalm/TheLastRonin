#include "NavigationBar.h"

#include "../../events/AppEvents.h"
#include "../../styles/ButtonStyle.h"
#include "../../window/MainWindow.h"
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

    homeButton = new QPushButton(QIcon(":/Images/Home"), "", this);
    homeButton->setStyleSheet(ButtonStyle::primary());
    homeButton->setToolTip("Home");
    homeButton->setFixedSize(40, 40);

    libraryButton = new QPushButton(QIcon(":/Images/Library"), "", this);
    libraryButton->setStyleSheet(ButtonStyle::primary());
    libraryButton->setToolTip("Library");
    libraryButton->setFixedSize(40, 40);

    youtubeMusicButton = new QPushButton(QIcon(":/Images/YouTube"), "", this);
    youtubeMusicButton->setStyleSheet(ButtonStyle::primary());
    youtubeMusicButton->setToolTip("YouTube Music");
    youtubeMusicButton->setFixedSize(40, 40);

    settingsButton = new QPushButton(QIcon(":/Images/Settings"), "", this);
    settingsButton->setStyleSheet(ButtonStyle::primary());
    settingsButton->setToolTip("Settings");
    settingsButton->setFixedSize(40, 40);

    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->addWidget(backButton);
    layout->addWidget(forwardButton);
    layout->addStretch();
    layout->addWidget(homeButton);
    layout->addWidget(libraryButton);
    layout->addWidget(youtubeMusicButton);
    layout->addWidget(settingsButton);

    layout->setContentsMargins(5, 5, 5, 5);
    layout->setSpacing(5);
    setLayout(layout);

    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

    connect(backButton, &QPushButton::clicked, this,
            [this]()
            {
                if (auto mainWindow = qobject_cast<MainWindow*>(window()))
                {
                    mainWindow->goBack();
                }
            });

    connect(forwardButton, &QPushButton::clicked, this,
            [this]()
            {
                if (auto mainWindow = qobject_cast<MainWindow*>(window()))
                {
                    mainWindow->goNext();
                }
            });

    connect(homeButton, &QPushButton::clicked, this, [this]() { AppEvents::instance().navigateToHome(); });

    connect(libraryButton, &QPushButton::clicked, this, [this]() { AppEvents::instance().navigateToLibrary(); });

    connect(youtubeMusicButton, &QPushButton::clicked, this,
            [this]() { AppEvents::instance().navigateToYouTubeMusic(); });

    connect(settingsButton, &QPushButton::clicked, this, [this]() { AppEvents::instance().navigateToSettings(); });
}

void NavigationBar::setBackEnabled(bool enabled)
{
    backButton->setEnabled(enabled);
}

void NavigationBar::setForwardEnabled(bool enabled)
{
    forwardButton->setEnabled(enabled);
}
