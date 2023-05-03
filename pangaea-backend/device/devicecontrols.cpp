#include "devicecontrols.h"

DeviceControls::DeviceControls()
{
    m_parametersList.append({"eq_volume1",      "eqv 0", 0});
    m_parametersList.append({"eq_volume2",      "eqv 1", 1});
    m_parametersList.append({"eq_volume3",      "eqv 2", 2});
    m_parametersList.append({"eq_volume4",      "eqv 3", 3});
    m_parametersList.append({"eq_volume5",      "eqv 4", 4});

    m_parametersList.append({"early_volume",    "ev", 5});
    m_parametersList.append({"early_type",      "et", 6});

    m_parametersList.append({"master_volume",   "mv", 7});

    m_parametersList.append({"cabinet_enable",  "ce", 8});

    m_parametersList.append({"eq_on",           "eqo", 9});

    m_parametersList.append({"early_on",        "eo", 10});

    m_parametersList.append({"amp_on",          "ao", 11});
    m_parametersList.append({"amp_volume",      "av", 12});
    m_parametersList.append({"amp_slave",       "as", 13});
    m_parametersList.append({"amp_type",        "at", 14});

    m_parametersList.append({"preamp_on",       "pro", 15});
    m_parametersList.append({"preamp_volume",   "prv", 16});
    m_parametersList.append({"preamp_low",      "prl", 17, true});
    m_parametersList.append({"preamp_mid",      "prm", 18, true});
    m_parametersList.append({"preamp_high",     "prh", 19, true});

    m_parametersList.append({"gate_on",         "go", 20});
    m_parametersList.append({"gate_threshold",  "gt", 21});
    m_parametersList.append({"gate_decay",      "gd", 22});

    m_parametersList.append({"compressor_on",           "co", 23});
    m_parametersList.append({"compressor_sustain",      "cs", 24});
    m_parametersList.append({"compressor_volume",       "cv", 25});

    m_parametersList.append({"eq_freq1",       "eqf 0", 26, true});
    m_parametersList.append({"eq_freq2",       "eqf 1", 27, true});
    m_parametersList.append({"eq_freq3",       "eqf 2", 28, true});
    m_parametersList.append({"eq_freq4",       "eqf 3", 29, true});
    m_parametersList.append({"eq_freq5",       "eqf 4", 30, true});

    m_parametersList.append({"eq_q1",       "eqq 0", 31, true});
    m_parametersList.append({"eq_q2",       "eqq 1", 32, true});
    m_parametersList.append({"eq_q3",       "eqq 2", 33, true});
    m_parametersList.append({"eq_q4",       "eqq 3", 34, true});
    m_parametersList.append({"eq_q5",       "eqq 4", 35, true});

    m_parametersList.append({"lpf_volume",       "lv", 36});
    m_parametersList.append({"hpf_volume",       "hv", 37});

    m_parametersList.append({"hpf_on",       "ho", 38});
    m_parametersList.append({"lpf_on",       "lo", 39});

    m_parametersList.append({"presence_on",       "po", 40});
    m_parametersList.append({"presence_volume",   "pv", 41});

    m_parametersList.append({"eq_pre",       "eqp", 42});
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

quint8 DeviceControls::getParameterValue(QString parameterName)
{
    auto parameter = findParameter(parameterName);

    return parameter->value();
}


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
        if(it->isSigned()) emit sgSetInterfaceValue(it->paramName(), (qint8)it->value());
        else emit sgSetInterfaceValue(it->paramName(), it->value());

    }
}

void DeviceControls::setParameterValue(QString parameterName, quint8 value)
{
    auto parameter = findParameter(parameterName);

    if(parameter != nullptr)
        parameter->setValue(value);
    else
        qWarning() << "Error setting parameter " << parameterName << "with value " << value;
}

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
