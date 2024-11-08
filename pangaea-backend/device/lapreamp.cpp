#include "lapreamp.h"

LAPreamp::LAPreamp() {}
// m_devCommandList.append(DeviceAnswer(new MaskedParser("sm0*END\n", "111X1111"), AnswerType::la3CleanPreset, "la3 clean preset mappings"));
// m_devCommandList.append(DeviceAnswer(new MaskedParser("sm1*END\n", "111X1111"), AnswerType::la3DrivePreset, "la3 drive preset mappings"));
// m_devCommandList.append(DeviceAnswer(new MaskedParser("umx\rEND\n", "11011111"), AnswerType::la3ModeChange, "la3 hardware switch occured"));
// m_devCommandList.append(DeviceAnswer(new MaskedParser("sw1\r*\nEND\n", "1111X11111"), AnswerType::la3CurrentChannel, "la3 current mode"));

/*
            case AnswerType::la3CleanPreset:
            {
                if(parseResult.size()==1)
                {
                    quint8 bankPreset = parseResult.at(0).toUInt();

                    qInfo() << "LA3 clean preset mapping: " << bankPreset;
                    // emit sgRecieveDeviceParameter(DeviceParameter::Type::LA3_CLEAN_PRESET, bankPreset);
                }
                break;
            }

            case AnswerType::la3DrivePreset:
            {
                if(parseResult.size()==1)
                {
                    quint8 bankPreset = parseResult.at(0).toUInt();
                    qInfo() << "LA3 drive preset mapping: " << bankPreset;
                    // emit sgRecieveDeviceParameter(DeviceParameter::Type::LA3_DRIVE_PRESET, bankPreset);
                }
                break;
            }

            case AnswerType::la3ModeChange:
            {
                qInfo() << recievedCommand.description();

                // pushReadPresetCommands();
                // pushCommandToQueue("rns\n");
                pushCommandToQueue("sw1\n");
                processCommands();
                break;
            }

            case AnswerType::la3CurrentChannel:
            {
                if(parseResult.size()==1)
                {
                    QString strCurrentLa3Mode = QString::fromStdString(parseResult.at(0).toStdString());
                    quint8 currentLa3Mode = 0;
                    if(strCurrentLa3Mode == "high")
                    {
                        currentLa3Mode = 0;
                        qInfo() << "LA3 current mode: " << "clean channel";
                    }
                    else
                    {
                        currentLa3Mode = 1;
                        qInfo() << "LA3 current mode: " << "clean channel";
                    }
                    // emit sgRecieveDeviceParameter(DeviceParameter::Type::LA3_CURRENT_CHANNEL, currentLa3Mode);
                }
                break;
            }
*/

// if(controlledDevice.deviceType() == DeviceType::LA3)
// {
//     // request LA3 maps
//     pushCommandToQueue("sm0");
//     pushCommandToQueue("sm1");
//     pushCommandToQueue("sw1");
// }


// void Core::slRecieveAppAction(AppAction appParameterType, QVariantList parameters)
// {


//     case SET_LA3_MAPPINGS:
//     {
//         quint8 cleanPreset = parameters.at(0).toInt();
//         quint8 drivePreset = parameters.at(1).toInt();
//         pushCommandToQueue(QString("sm0 %2").arg(cleanPreset).toUtf8());
//         pushCommandToQueue(QString("sm1 %2").arg(drivePreset).toUtf8());
//         processCommands();
//         break;
//     }

// }
