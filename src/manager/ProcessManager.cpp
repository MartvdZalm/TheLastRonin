#include "ProcessManager.h"

ProcessManager& ProcessManager::instance()
{
    static ProcessManager instance;
    return instance;
}

QProcess* ProcessManager::createProcess(const QString& program, const QStringList& args)
{
    QProcess* process = new QProcess(this);
    processes << process;

    connect(process, &QProcess::finished, this,
            [this, process]()
            {
                processes.removeOne(process);
                process->deleteLater();
            });

    process->setProgram(program);
    process->setArguments(args);
    return process;
}

void ProcessManager::cancelAll()
{
    for (QProcess* process : processes)
    {
        if (process->state() == QProcess::Running)
        {
            process->terminate();
            if (!process->waitForFinished(2000))
            {
                process->kill();
            }
        }
    }
    processes.clear();
}
