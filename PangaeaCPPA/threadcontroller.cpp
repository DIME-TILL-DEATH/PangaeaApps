#include <QDebug>
#include "threadcontroller.h"

ThreadController::ThreadController(QThread *mainThread)
    : QObject(nullptr),
      m_mainThread{mainThread}
{
    m_backendThread = new QThread();

    m_backendThread->setObjectName("Backend thread");
    m_backendThread->start();

    m_connectionsThread = new QThread();

    m_connectionsThread->setObjectName("Connections thread");
    m_connectionsThread->start();
}

ThreadController::~ThreadController()
{
    m_backendThread->quit();
    m_backendThread->wait();

    m_connectionsThread->quit();
    m_connectionsThread->wait();
}

QThread *ThreadController::mainThread() const
{
    return m_mainThread;
}

QThread *ThreadController::backendThread() const
{
    return m_backendThread;
}

QThread *ThreadController::connectionsThread() const
{
    return m_connectionsThread;
}

