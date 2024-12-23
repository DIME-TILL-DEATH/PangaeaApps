#include <QMetaEnum>

#include "abstractinterface.h"

AbstractInterface::AbstractInterface(QObject *parent)
    : QObject{parent}
{

}

AbstractInterface::~AbstractInterface()
{
//    qDebug() << "Absract interface deletion";
}

void AbstractInterface::setState(AbstractInterface::InterfaceState newState)
{
    QMetaEnum enumStates = QMetaEnum::fromType<AbstractInterface::InterfaceState >();

    qDebug() << "State changed" << m_connectionType << ": " << enumStates.valueToKey(newState);

    if (m_state == newState)
            return;

    m_prevState = m_state;
    m_state = newState;
    emit sgChangedState(newState);
}

AbstractInterface::InterfaceState AbstractInterface::state() const
{
    return m_state;
}

DeviceDescription AbstractInterface::connectedDevice() const
{
    return m_connectedDevice;
}

DeviceConnectionType AbstractInterface::connectionType() const
{
    return m_connectionType;
}

void AbstractInterface::prevState()
{
    m_state = m_prevState;

    QMetaEnum enumStates = QMetaEnum::fromType<AbstractInterface::InterfaceState >();
    qDebug() << "State changed. Return previous state" << m_description << ": " << enumStates.valueToKey(m_state);
}
