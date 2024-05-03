#ifndef DEVICEPARAMETER_H
#define DEVICEPARAMETER_H

#include <QString>
#include <QVariant>

class DeviceParameter
{
    Q_GADGET
public:
    enum class Type
    {
        EQ_VOLUME1 = 0,
        EQ_VOLUME2 = 1,
        EQ_VOLUME3 = 2,
        EQ_VOLUME4 = 3,
        EQ_VOLUME5 = 4,

        EARLY_VOLUME = 5,
        EARLY_TYPE = 6,

        MASTER_VOLUME = 7,

        CABINET_ENABLE = 8,

        EQ_ON = 9,

        EARLY_ON = 10,

        AMP_ON = 11,
        AMP_VOLUME = 12,
        AMP_SLAVE = 13,
        AMP_TYPE = 14,

        PREAMP_ON = 15,
        PREAMP_VOLUME = 16,
        PREAMP_LOW = 17,
        PREAMP_MID = 18,
        PREAMP_HIGH = 19,

        GATE_ON = 20,
        GATE_THRESHOLD = 21,
        GATE_DECAY = 22,

        COMPRESSOR_ON = 23,
        COMPRESSOR_SUSTAIN = 24,
        COMPRESSOR_VOLUME = 25,

        EQ_FREQ1 = 26,
        EQ_FREQ2 = 27,
        EQ_FREQ3 = 28,
        EQ_FREQ4 = 29,
        EQ_FREQ5 = 30,

        EQ_Q1 = 31,
        EQ_Q2 = 32,
        EQ_Q3 = 33,
        EQ_Q4 = 34,
        EQ_Q5 = 35,

        LPF_VOLUME = 36,
        HPF_VOLUME = 37,

        HPF_ON = 38,
        LPF_ON = 39,

        PRESENCE_ON = 40,
        PRESENCE_VOLUME = 41,

        EQ_PRE = 42,

        //=======Not in status command
        DEVICE_TYPE = 64,
        MAP_SIZE,
        BANK,
        PRESET,
        OUTPUT_MODE
    };
    Q_ENUM(Type);

    DeviceParameter(Type parameterType);

    static QString sendString(Type parameterType, quint8 value);

    quint8 value() const;
    void setValue(quint8 newValue);

    static bool isSigned(Type parameterType);

    Type parameterType() const;

private:
    Type m_parameterType;
    quint8 m_value;
};

#endif // DEVPARAM_H
