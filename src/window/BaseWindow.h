#ifndef BASEWINDOW_H
#define BASEWINDOW_H

#include <QWidget>

class BaseWindow : public QWidget
{
    Q_OBJECT

  public:
    explicit BaseWindow(QWidget* parent = nullptr);
    ~BaseWindow();

    virtual void setupUI() = 0;
    virtual void setupConnections() = 0;
    virtual void setupEvents() = 0;
    virtual void retranslateUI() {}
    void updateTranslations();

  private slots:
    void onLanguageChanged(const QString& language);

  private:
    void connectLanguageManager();
};

#endif // BASEWINDOW_H
