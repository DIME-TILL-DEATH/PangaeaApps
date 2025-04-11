#ifndef CP1__FX_H
#define CP1__FX_H

#include <QObject>
#include "abstractdevice.h"

class Cp100fx : public AbstractDevice
{
    Q_OBJECT
public:
    Cp100fx(Core *parent);
    ~Cp100fx();

    void updateOutputModeNames() override;

    quint16 processingUsed() override;

    void initDevice(DeviceType deviceType) override;
    void readFullState() override;

    Q_INVOKABLE void saveChanges() override;
    Q_INVOKABLE void changePreset(quint8 newBank, quint8 newPreset, bool ignoreChanges = false) override;

    Q_INVOKABLE void comparePreset() override;
    Q_INVOKABLE void copyPreset() override;
    Q_INVOKABLE void pastePreset() override;
    Q_INVOKABLE void importPreset(QString filePath, QString fileName) override;
    Q_INVOKABLE void exportPreset(QString filePath, QString fileName) override;

    Q_INVOKABLE void restoreValue(QString name) override;

    Q_INVOKABLE void previewIr(QString srcFilePath) override;
    Q_INVOKABLE void startIrUpload(QString srcFilePath, QString dstFilePath = "", bool trimFile = false) override;

    Q_INVOKABLE void setFirmware(QString fullFilePath) override;
    Q_INVOKABLE void formatMemory() override;

public slots:
    QList<QByteArray> parseAnswers(QByteArray& baAnswer) override;

private:
    QList<PresetAbstract*> m_presetsList;
};

#endif // CP1__FX_H
