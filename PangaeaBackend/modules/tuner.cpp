#include "tuner.h"
#include "abstractdevice.h"

Tuner::Tuner(AbstractDevice *owner)
    :AbstractModule{owner, ModuleType::TUNER, "TN", "tn enable"}
{
    m_moduleEnabled = new bool;
    *m_moduleEnabled = false;

    m_note = "-";
    m_cents = 0;
    m_refFreq = 440;
}

Tuner::~Tuner()
{
    delete m_moduleEnabled;
}

void Tuner::getSamples(quint16 samplesCount)
{
    QByteArray command;
    command = "tn get " + QByteArray::number(samplesCount) + "\r\n";

    if(m_owner) emit m_owner->sgWriteToInterface(command, true);
}

void Tuner::setTune(QString note, qint16 cents)
{
    m_note = note;
    m_cents = cents;

    emit tuneChanged();
}

void Tuner::increaseRefFreq()
{
    if(m_refFreq < 450)
    {
        m_refFreq++;
        emit refFreqChanged();

        QByteArray command;
        command = "tn ref " + QByteArray::number(m_refFreq) + "\r\n";

        if(m_owner) emit m_owner->sgWriteToInterface(command, true);
    }
}

void Tuner::decreaseRefFreq()
{
    if(m_refFreq > 430)
    {
        m_refFreq--;
        emit refFreqChanged();

        QByteArray command;
        command = "tn ref " + QByteArray::number(m_refFreq) + "\r\n";

        if(m_owner) emit m_owner->sgWriteToInterface(command, true);
    }
}

QString Tuner::note() const
{
    return m_note;
}

qint16 Tuner::cents() const
{
    return m_cents;
}

quint16 Tuner::refFreq() const
{
    return m_refFreq;
}
