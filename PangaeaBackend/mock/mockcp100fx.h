#ifndef MOCKCP100_FX_H
#define MOCKCP100_FX_H

#include <QObject>
#include <QDir>

#include "abstractmockdevice.h"

#include "hardwarefxpreset.h"
#include "systemsettingsfx.h"

class MockCP100fx : public AbstractMockDevice
{
    Q_OBJECT
public:
    explicit MockCP100fx(QMutex *mutex, QByteArray *uartBuffer, QObject *parent = nullptr);

    void writeToDevice(const QByteArray& data) override;
    void newDataRecieved() override;

    static QString mockName() {return "offline CP100FX";};

    preset_data_fx_t currentPresetData;
    preset_data_fx_t defaultPresetData;

    TSystemSettingsFx currentSystemData;
    // controller_fx_t controller[controllersCount];

    QString ir1Name;
    QString ir2Name;
    QByteArray ir1Data;
    QByteArray ir2Data;
private:
    QDir m_currentDir;

    QString m_lastPath;

    quint32 delayTime;

    QMap<QString, quint8*> paramsByteMap;
    QMap<QString, quint16*> paramsWordMap;

    QMap<QString, quint8*> sysParamsByteMap;

    QByteArray intToBa(quint8 val);

    void initFolders();
    bool loadSysParameters();
    bool saveSysParameters();
    void loadPresetData(quint8 presetNumber);
    void savePresetData(quint8 prPreset);

    void setParamsHandler(QString commStr, quint8 *commPtr);
    void setParamsHandler(QString commStr, quint16 *commPtr);
    void setSysParamsHandler(QString commStr, quint8 *commPtr);

    void parametersByteCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data);
    void parametersWordCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data);
    void sysParamsByteCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data);

    void eqfCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data);
    void eqgCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data);
    void eqqCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data);

    void amtDevCommHandler(const QString &command, const QByteArray& arguments, const QByteArray &data);
    void amtVerCommHandler(const QString &command, const QByteArray& arguments, const QByteArray &data);

    void sysSettingsCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data);
    void stateCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data);

    void irCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data);
    void lsCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data);
    void cdCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data);

    void uploadCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data);
    void mkdirCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data);
    void renameCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data);
    void removeCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data);

    void cntrlsCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data);
    void cntrlsPcCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data);
    void cntrlsSetCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data);

    void psaveCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data);
    void pchangeCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data);
    void plistCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data);
    void pnumCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data);
    void pnameCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data);
    void pcommentCommHandler(const QString &command, const QByteArray &arguments, const QByteArray &data);



    static constexpr quint8 stringDataSize = 15;
};

#endif // MOCKCP100_FX_H
