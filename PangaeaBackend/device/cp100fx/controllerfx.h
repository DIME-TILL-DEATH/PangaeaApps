#ifndef CONTROLLERFX_H
#define CONTROLLERFX_H

#include <QObject>
#include <QQmlEngine>

#include <abstractdevice.h>

typedef struct
{
    uint8_t src;
    uint8_t dst;
    uint8_t minVal;
    uint8_t maxVal;
}TController;

class ControllerFx : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("")

    Q_PROPERTY(quint8 destination READ destination WRITE setDestination NOTIFY controllerChanged FINAL)
    Q_PROPERTY(quint8 source READ source NOTIFY controllerChanged FINAL)
    Q_PROPERTY(quint8 minValue READ minValue WRITE setMinValue NOTIFY controllerChanged FINAL)
    Q_PROPERTY(quint8 maxValue READ maxValue WRITE setMaxValue NOTIFY controllerChanged FINAL)

    Q_PROPERTY(QStringList avaliableSources READ avaliableSourcesList NOTIFY avaliableSourcesChanged)
public:
    explicit ControllerFx(TController* controllerData, quint8 num, AbstractDevice* owner);

    quint8 destination() const;
    void setDestination(quint8 newDestination);

    quint8 source();
    Q_INVOKABLE void setSource(const QString& srcName);

    quint8 minValue() const;
    void setMinValue(quint8 newMinValue);

    quint8 maxValue() const;
    void setMaxValue(quint8 newMaxValue);

    QStringList sourcesList();
    QStringList avaliableSourcesList();

    quint8 dataFromSourceName(const QString& source);

    static constexpr quint8 controllersCount = 32;

signals:
    void controllerChanged();
    void avaliableSourcesChanged();

private:
    AbstractDevice* m_owner;
    TController* m_controllerData;

    quint8 m_num;

    void sendData(const QByteArray& data);
};

#endif // CONTROLLERFX_H
