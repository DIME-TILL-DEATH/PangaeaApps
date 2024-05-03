#include "devicecontrols.h"

DeviceControls::DeviceControls()
{
    m_parametersList.append({DeviceParameter::Type::EQ_VOLUME1, "eq_volume1"});
    m_parametersList.append({DeviceParameter::Type::EQ_VOLUME2, "eq_volume2"});
    m_parametersList.append({DeviceParameter::Type::EQ_VOLUME3, "eq_volume3"});
    m_parametersList.append({DeviceParameter::Type::EQ_VOLUME4, "eq_volume4"});
    m_parametersList.append({DeviceParameter::Type::EQ_VOLUME5, "eq_volume5"});

    m_parametersList.append({DeviceParameter::Type::EARLY_VOLUME, "early_volume"});
    m_parametersList.append({DeviceParameter::Type::EARLY_TYPE, "early_type"});

    m_parametersList.append({DeviceParameter::Type::MASTER_VOLUME, "master_volume"});

    m_parametersList.append({DeviceParameter::Type::CABINET_ENABLE, "cabinet_enable"});

    m_parametersList.append({DeviceParameter::Type::EQ_ON, "eq_on"});

    m_parametersList.append({DeviceParameter::Type::EARLY_ON, "early_on"});

    m_parametersList.append({DeviceParameter::Type::AMP_ON, "amp_on"});
    m_parametersList.append({DeviceParameter::Type::AMP_VOLUME, "amp_volume"});
    m_parametersList.append({DeviceParameter::Type::AMP_SLAVE, "amp_slave"});
    m_parametersList.append({DeviceParameter::Type::AMP_TYPE, "amp_type"});

    m_parametersList.append({DeviceParameter::Type::PREAMP_ON, "preamp_on"});
    m_parametersList.append({DeviceParameter::Type::PREAMP_VOLUME, "preamp_volume"});
    m_parametersList.append({DeviceParameter::Type::PREAMP_LOW, "preamp_low"});
    m_parametersList.append({DeviceParameter::Type::PREAMP_MID, "preamp_mid"});
    m_parametersList.append({DeviceParameter::Type::PREAMP_HIGH, "preamp_high"});

    m_parametersList.append({DeviceParameter::Type::GATE_ON, "gate_on"});
    m_parametersList.append({DeviceParameter::Type::GATE_THRESHOLD, "gate_threshold"});
    m_parametersList.append({DeviceParameter::Type::GATE_DECAY, "gate_decay"});

    m_parametersList.append({DeviceParameter::Type::COMPRESSOR_ON, "compressor_on"});
    m_parametersList.append({DeviceParameter::Type::COMPRESSOR_SUSTAIN, "compressor_sustain"});
    m_parametersList.append({DeviceParameter::Type::COMPRESSOR_VOLUME, "compressor_volume"});

    m_parametersList.append({DeviceParameter::Type::EQ_FREQ1, "eq_freq1"});
    m_parametersList.append({DeviceParameter::Type::EQ_FREQ2, "eq_freq2"});
    m_parametersList.append({DeviceParameter::Type::EQ_FREQ3, "eq_freq3"});
    m_parametersList.append({DeviceParameter::Type::EQ_FREQ4, "eq_freq4"});
    m_parametersList.append({DeviceParameter::Type::EQ_FREQ5, "eq_freq5"});

    m_parametersList.append({DeviceParameter::Type::EQ_Q1, "eq_q1"});
    m_parametersList.append({DeviceParameter::Type::EQ_Q2, "eq_q2"});
    m_parametersList.append({DeviceParameter::Type::EQ_Q3, "eq_q3"});
    m_parametersList.append({DeviceParameter::Type::EQ_Q4, "eq_q4"});
    m_parametersList.append({DeviceParameter::Type::EQ_Q5, "eq_q5"});

    m_parametersList.append({DeviceParameter::Type::LPF_VOLUME, "lpf_volume"});
    m_parametersList.append({DeviceParameter::Type::HPF_VOLUME, "hpf_volume"});

    m_parametersList.append({DeviceParameter::Type::HPF_ON, "hpf_on"});
    m_parametersList.append({DeviceParameter::Type::LPF_ON, "lpf_on"});

    m_parametersList.append({DeviceParameter::Type::PRESENCE_ON, "presence_on"});
    m_parametersList.append({DeviceParameter::Type::PRESENCE_VOLUME, "presence_volume"});

    m_parametersList.append({DeviceParameter::Type::EQ_PRE, "eq_pre"});
}

bool DeviceControls::containsParameter(QString paramName)
{
    return findParameter(paramName) != nullptr;
}

QString DeviceControls::getParameterSendString(QString parameterName, quint8 value)
{
    auto parameter = findParameter(parameterName);

    return parameter->sendString(value);
}

QString DeviceControls::getParameterSendString(DeviceParameter::Type parameterType, quint8 value)
{
    auto parameter = std::find_if(m_parametersList.begin(),
                           m_parametersList.end(),
                           [parameterType](DeviceParameter parameter)
                           {return parameter.parameterType() == parameterType;});

    return parameter->sendString(value);
}

// quint8 DeviceControls::getParameterValue(QString parameterName)
// {
//     auto parameter = findParameter(parameterName);

//     return parameter->value();
// }


void DeviceControls::setParametersFromRaw(QByteArray ba)
{
    QString sss;
    quint8 count=0;
    quint8 nomByte=0;

    foreach (QChar val, ba) //quint8
    {
        if((nomByte&1)==0)
        {
            sss.clear();
            sss.append(val);
        }
        else
        {
            sss.append(val);
            setParameterValue(count, sss.toInt(nullptr, 16));

            count++;
        }
        nomByte++;
    }
}

void DeviceControls::setParameterValue(quint8 numberInArray, quint8 value)
{
    auto parameter = findParameter(numberInArray);

    if(parameter != nullptr)
        parameter->setValue(value);
    else
        qWarning() << "Error setting parameter with number in array" << numberInArray << "with value " << value;
}

void DeviceControls::setAllUIValues()
{
    for(auto it=m_parametersList.begin(); it!=m_parametersList.end(); ++it)
    {
        if(it->isSigned())
        {
            //emit sgSetInterfaceValue(it->paramName(), (qint8)it->value());
            emit sgSetUiDeviceParameter(it->parameterType(), (qint8)it->value());
        }
        else
        {
           // emit sgSetInterfaceValue(it->paramName(), it->value());
            emit sgSetUiDeviceParameter(it->parameterType(), it->value());
        }
    }
}

// void DeviceControls::setParameterValue(QString parameterName, quint8 value)
// {
//     auto parameter = findParameter(parameterName);

//     if(parameter != nullptr)
//         parameter->setValue(value);
//     else
//         qWarning() << "Error setting parameter " << parameterName << "with value " << value;
// }

DeviceParameter* DeviceControls::findParameter(QString parameterName)
{
    auto it = std::find_if(m_parametersList.begin(),
                      m_parametersList.end(),
                      [parameterName](DeviceParameter parameter)
                    {return parameter.paramName() == parameterName;});

    if(it == m_parametersList.end())
    {
        //qWarning() << "Parameter with name " << parameterName << "not found!!!";
        return nullptr;
    }

    return &(*it);
}

DeviceParameter* DeviceControls::findParameter(quint8 numberInArray)
{
    auto it = std::find_if(m_parametersList.begin(),
                      m_parametersList.end(),
                      [numberInArray](DeviceParameter parameter)
                    {return parameter.positionInArray() == numberInArray;});

    if(it == m_parametersList.end())
    {
        qWarning() << "Parameter with number " << numberInArray << "not found!!!";
        return nullptr;
    }

    return &(*it);
}
