#ifndef TASKS_CHEDULER_H
#define TASKS_CHEDULER_H

#include <QObject>

class TaskScheduler : public QObject
{
    Q_OBJECT
public:
    explicit TaskScheduler(QObject *parent = nullptr);

    bool createTask();
    bool removeTask();
};

#endif // TASKS_CHEDULER_H
