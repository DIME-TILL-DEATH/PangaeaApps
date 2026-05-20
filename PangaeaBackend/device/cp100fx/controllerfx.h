#ifndef CONTROLLERFX_H
#define CONTROLLERFX_H

#include <QObject>
#include <QQmlEngine>

#include <abstractdevice.h>
#include <abstractmodule.h>
#include "hardwarefxpreset.h"
#include "controlvalue.h"

class ControlValue;

class ControllerFx : public AbstractModule
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("")

    Q_PROPERTY(ControlValue* destination READ destination NOTIFY dataChanged FINAL)
    Q_PROPERTY(ControlValue* source READ source NOTIFY dataChanged FINAL)
    Q_PROPERTY(ControlValue* minValue READ minValue NOTIFY dataChanged FINAL)
    Q_PROPERTY(ControlValue* maxValue READ maxValue NOTIFY dataChanged FINAL)

    Q_PROPERTY(QStringList avaliableSources READ avaliableSourcesList NOTIFY avaliableSourcesChanged)
public:
    explicit ControllerFx(AbstractDevice* owner, quint8 num);

    ControlValue* destination() const {return m_destination;}
    ControlValue* source() const {return m_source;}
    ControlValue* minValue() const {return m_minValue;}
    ControlValue* maxValue() const {return m_maxValue;}

    QStringList sourcesList();
    QStringList avaliableSourcesList();

    quint8 dataFromSourceName(const QString& source);

    void setData(const controller_fx_t& m_controllerData);

signals:
    void avaliableSourcesChanged();

private:
    quint8 m_num;

    ControlValue* m_destination;
    ControlValue* m_source;
    ControlValue* m_minValue;
    ControlValue* m_maxValue;

    void srcControlValueSetter(qint32 value);
    void srcDisplaySetter(double value);
};

#endif // CONTROLLERFX_H
