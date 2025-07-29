#include "BaseWindow.h"

#include "../service/LanguageService.h"

BaseWindow::BaseWindow(QWidget* parent) : QWidget(parent)
{
    connectLanguageService();
}

BaseWindow::~BaseWindow() {}

void BaseWindow::connectLanguageService()
{
    connect(&LanguageService::instance(), &LanguageService::languageChanged, this, &BaseWindow::onLanguageChanged);
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
