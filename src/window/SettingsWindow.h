#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H
#include "../repository/SettingsRepository.h"
#include "BaseWindow.h"
#include <QComboBox>
#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QListWidget>
#include <QPushButton>
#include <QScrollArea>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QWidget>

class SettingsWindow : public BaseWindow
{
    Q_OBJECT

  public:
    explicit SettingsWindow(QWidget* parent = nullptr);

  private slots:
    void onSidebarItemChanged(int index);
    void onCancelClicked();
    void onApplyClicked();

  private:
    void setupUI() override;
    void setupConnections() override;
    void setupEvents() override;
    void retranslateUI() override;
    void setupSidebar();
    void setupContentArea();
    void setupButtonArea();
    void loadSettings();
    void saveSettings();

    QWidget* createGeneralPage();
    QWidget* createAudioPage();
    QWidget* createLibraryPage();
    QWidget* createPlaybackPage();
    QWidget* createInterfacePage();
    QWidget* createHotkeysPage();
    QWidget* createAdvancedPage();

    QHBoxLayout* mainLayout;
    QVBoxLayout* leftLayout;
    QVBoxLayout* rightLayout;

    QListWidget* sidebar;
    QFrame* sidebarFrame;

    QStackedWidget* contentStack;
    QScrollArea* scrollArea;
    QFrame* contentFrame;

    QFrame* buttonFrame;
    QHBoxLayout* buttonLayout;
    QPushButton* okButton;
    QPushButton* cancelButton;
    QPushButton* applyButton;

    QWidget* generalPage;
    QWidget* audioPage;
    QWidget* libraryPage;
    QWidget* playbackPage;
    QWidget* interfacePage;
    QWidget* hotkeysPage;
    QWidget* advancedPage;

    QComboBox* languageCombo;
    SettingsRepository settingsRepository;
};

#endif // SETTINGSWINDOW_H
