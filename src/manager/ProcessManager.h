#ifndef PROCESSMANAGER_H
#define PROCESSMANAGER_H

#include <QList>
#include <QObject>
#include <QProcess>

class ProcessManager : public QObject
{
    Q_OBJECT
  public:
    explicit ProcessManager(QObject* parent = nullptr) : QObject(parent) {}

    static ProcessManager& instance();

    QProcess* createProcess(const QString& program, const QStringList& args = {});
    void cancelAll();

  private:
    QList<QProcess*> processes;
};

#endif // PROCESSMANAGER_H