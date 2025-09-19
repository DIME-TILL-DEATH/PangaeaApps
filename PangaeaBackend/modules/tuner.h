#ifndef TUNER_H
#define TUNER_H

#include <QObject>
#include <QQmlEngine>
#include "abstractmodule.h"

class Tuner : public AbstractModule
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("")

    Q_PROPERTY(QString note READ note NOTIFY tuneChanged FINAL)
    Q_PROPERTY(qint16 cents READ cents NOTIFY tuneChanged FINAL)

    Q_PROPERTY(quint16 refFreq READ refFreq NOTIFY refFreqChanged FINAL)
public:
    Tuner(AbstractDevice *owner);
    ~Tuner();

    Q_INVOKABLE void getSamples(quint16 samplesCount);
    Q_INVOKABLE void increaseRefFreq();
    Q_INVOKABLE void decreaseRefFreq();

    void setTune(QString note, qint16 cents);
    QString note() const;
    qint16 cents() const;
    quint16 refFreq() const;

signals:
    void tuneChanged();
    void refFreqChanged();

private:
    QString m_note;
    qint16 m_cents;
    quint16 m_refFreq;
};

#endif // TUNER_H
