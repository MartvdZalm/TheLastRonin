#include "BaseWindow.h"

#include "../manager/LanguageManager.h"

BaseWindow::BaseWindow(QWidget* parent) : QWidget(parent)
{
    connectLanguageManager();
}

BaseWindow::~BaseWindow() {}

void BaseWindow::connectLanguageManager()
{
    connect(&LanguageManager::instance(), &LanguageManager::languageChanged, this, &BaseWindow::onLanguageChanged);
}

void BaseWindow::onLanguageChanged(const QString& language)
{
    Q_UNUSED(language)
    updateTranslations();
}

void BaseWindow::updateTranslations()
{
    retranslateUI();
}
