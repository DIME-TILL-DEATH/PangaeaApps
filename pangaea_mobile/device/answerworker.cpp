#include "answerworker.h"
#include "parser.h"

AnswerWorker::AnswerWorker()
{
    Parser* getStatus  = new Parser("gs\r00000000000000000000000000000000000000000000000000000000000000000000000000000000000000\n","111000000000000000000000000000000000000000000000000000000000000000000000000000000000000001");

    Parser* getIr  = new Parser("cc\r0\n0\n", "111X1X1");
    Parser* getIrNameSize  = new Parser("cc\r0\n", "111X1");
    Parser* getIrEmpty  = new Parser("cc\r\n\n", "11111");

    QByteArray sample, mask;
    for(quint8 i=0; i<100; i++)
    {
        sample.append("_\n00\nEND\n");
        mask.append("X10011111");
    }

    Parser* getIrListCP100 = new Parser(sample, mask);

    sample.clear();
    mask.clear();

    sample = "rns *\r";
    mask   = "111111";
    for(quint8 i=0; i<16; i++)
    {
        sample.append("00\n\t*\n\t00\n");
        mask.append("0011X11001");
        //sample.append("*\n00\n");
        //mask.append("X1001");
    }
    sample.append("END\n");
    mask.append("1111");

    Parser* getIrListCP16 = new Parser(sample, mask);

    // TODO: "error: file 0_1.wav not created, 'f_open' result 'FR_OK'\n" при неверном импульсе

    m_devCommandList.append(DeviceAnswer(new Parser("amtdev\rX\nEND\n", "1111111X11111"), AnswerType::getAMTDevType, "get device type"));
    m_devCommandList.append(DeviceAnswer(new Parser("amtver\r0\n","1111111X1"), AnswerType::getFWVersion, "get firmware version"));

    m_devCommandList.append(DeviceAnswer(getStatus, AnswerType::getStatus, "get parameter values from preset"));
    m_devCommandList.append(DeviceAnswer(new Parser("gsEND\n", "111111"), AnswerType::gsEND, "writing data to preset complete"));
    m_devCommandList.append(DeviceAnswer(new Parser("gs 1\r", "11111"), AnswerType::ackStatusSettling, "preset parameters settled"));

    //m_devCommandList.append(DeviceAnswer(new Parser("cc s 1\r", "1111111"), AnswerType::ccAck, "IR download acknowledge"));
    m_devCommandList.append(DeviceAnswer(new Parser("cc * *\r", "111X1X1"), AnswerType::ccAck, "IR upload acknowledge")); //upload save
    m_devCommandList.append(DeviceAnswer(new Parser("ccEND\n", "111111"), AnswerType::ccEND, "IR download finished"));
    m_devCommandList.append(DeviceAnswer(getIr, AnswerType::getIr, "get IR file"));
    m_devCommandList.append(DeviceAnswer(getIrNameSize, AnswerType::getIrNameSize, "get IR file"));
    m_devCommandList.append(DeviceAnswer(getIrEmpty, AnswerType::getIrEmpty, "IR file empty"));

    m_devCommandList.append(DeviceAnswer(new Parser("dcc\rEND\n", "11111111"), AnswerType::ackDcc, "delete wav in preset directory. Depricated version."));
    m_devCommandList.append(DeviceAnswer(new Parser("preset_delete_wavs\rEND\n", "11111111"), AnswerType::ackDcc, "delete wav in preset directory")); //TODO проверить что видит ответ на эту команду

    m_devCommandList.append(DeviceAnswer(new Parser("lcc\rEND\n", "11111111"), AnswerType::ackLcc, "cancel impulse"));
    m_devCommandList.append(DeviceAnswer(new Parser("esc\rEND\n", "11111111"), AnswerType::ackEsc, "return preset to saved state"));


    m_devCommandList.append(DeviceAnswer(new Parser("sp\r", "111"), AnswerType::ackSavePreset, "save preset acknowledgement"));
    m_devCommandList.append(DeviceAnswer(new Parser("gb\r0000\n", "11100001"), AnswerType::getBankPreset, "get current bank and preset number"));


    m_devCommandList.append(DeviceAnswer(new Parser("gm\r00\n", "111001"), AnswerType::getOutputMode, "get output mode(phones, line, balance)"));
    m_devCommandList.append(DeviceAnswer(new Parser("gm 0\r", "11101"), AnswerType::ackOutputMode, "set output mode acknowledgement"));

    m_devCommandList.append(DeviceAnswer(new Parser("rn\r \n", "111X1"), AnswerType::getIrName, "get IR name"));
    m_devCommandList.append(DeviceAnswer(new Parser("pc xx\r", "111001"), AnswerType::ackPresetChange, "preset change acknowledgement"));

    m_devCommandList.append(DeviceAnswer(new Parser("REQUEST_CHUNK_SIZE","111111111111111111"), AnswerType::requestNextChunk, "request next chunk"));
    m_devCommandList.append(DeviceAnswer(new Parser("FR_O","1111"), AnswerType::ackFWUFinished, "firmware update/formatting finished"));

    m_devCommandList.append(DeviceAnswer(new Parser("sw4 disable\rEND\n", "11111111111X"), AnswerType::ackDisableSW4, "hardware buttons off acknowledgment"));
    m_devCommandList.append(DeviceAnswer(new Parser("sw4 enable\rEND\n", "11111111111X"), AnswerType::ackEnableSW4, "hardware buttons on acknowledgment"));

    m_devCommandList.append(DeviceAnswer(new Parser("pwc 00 00\rEND\n", "11110010011111"), AnswerType::ackPwc, "copying IR acknowledgment"));
    m_devCommandList.append(DeviceAnswer(new Parser("pws xx\rEND\n", "11110011111"), AnswerType::ackPws, "temporaly save IR acknowledgemnt"));
    m_devCommandList.append(DeviceAnswer(new Parser("pwl xx\rEND\n", "11110011111"), AnswerType::ackPwl, "load temporaly IR acknowledgemnt"));

    m_devCommandList.append(DeviceAnswer(getIrListCP16, AnswerType::getIrList, "get list of IR names for CP16"));
    m_devCommandList.append(DeviceAnswer(getIrListCP100, AnswerType::getIrList, "get list of IR names for CP100"));

    m_devCommandList.append(DeviceAnswer(new Parser("END\n", "1111"), AnswerType::endOperation, "operation complete"));
}

DeviceAnswer AnswerWorker::parseRawData(const QByteArray &rawData)
{
    foreach(DeviceAnswer command, m_devCommandList)
    {
        if(command.processRawData(rawData))
        {
            return command;
        }
    }
    return DeviceAnswer(nullptr, AnswerType::unknown, "Command without parsing");
}

void AnswerWorker::parseAnswers(const QByteArray &rawData)
{
    foreach(DeviceAnswer command, m_devCommandList)
    {
        if(command.processRawData(rawData))
        {
            m_recievedAnswers.push_back(command);
        }
    }
}

DeviceAnswer AnswerWorker::popAnswer()
{
    if(m_recievedAnswers.isEmpty()) return DeviceAnswer(nullptr, AnswerType::unknown, "No answer!!!"); // protection

    return m_recievedAnswers.takeFirst();
}

bool AnswerWorker::haveAnswer()
{
    return !m_recievedAnswers.isEmpty();
}
