#ifndef THREADCONTROLLER_H
#define THREADCONTROLLER_H

#include <QObject>
#include <QThread>

class ThreadController:public QObject
{
    Q_OBJECT
public:
    explicit ThreadController(QThread* mainThread);
    ~ThreadController();
    QThread *mainThread() const;
    QThread *backendThread() const;

    QThread *connectionsThread() const;

private:
    QThread* m_mainThread;
    QThread* m_backendThread;
    QThread* m_connectionsThread;
};

#endif // THREADCONTROLLER_H
