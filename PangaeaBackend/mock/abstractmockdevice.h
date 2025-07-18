#ifndef ABSTRACTMOCKDEVICE_H
#define ABSTRACTMOCKDEVICE_H

#include <QObject>
#include <QMutex>

#include "parser.h"

enum MockDeviceType
{
    Mock_Abstract = 0,
    Mock_CPLegacy,
    Mock_CPModern,
    Mock_LA3,
    Mock_CP100FX
};

#define FIRMWARE_STRING_SIZE 20
typedef struct system_parameters_cp_t
{
    uint8_t empty[2];
    uint8_t output_mode;
    uint8_t reserved[8];
    uint8_t eol_symb = '\n';
    char firmware_version[FIRMWARE_STRING_SIZE];

    uint8_t la3_cln_preset;
    uint8_t la3_drv_preset;
}system_parameters_cp_t;

typedef struct system_parameters_fx_t
{
    uint8_t mod_proc;   /*cab on off*/
    uint8_t midi_channel;
    uint8_t expression_type;
    uint8_t fs_type_1[3];
    uint8_t calibration[4];
    uint8_t preset_to_fs_1[3][4];
    uint8_t spdif_type;
    uint8_t expression_CC;
    uint8_t fs_CC[3];
    uint8_t midi_pc_ind; /*service*/
    uint8_t fs_mode[3];
    uint8_t fs_type_2[3];
    uint8_t preset_to_fs_2[3][4];
    uint8_t k_cc[3];
    uint8_t tap_type;
    uint8_t tuner_ext_controller;
    uint8_t speed_foot;
    uint8_t time_type;
    uint8_t global_tempo_hi;
    uint8_t save_expression_level;
    uint8_t swap_up_confirm;
    uint8_t speed_tuner;

    // midi data? ([126]=127)
}system_parameters_fx_t;

class AbstractMockDevice : public QObject
{
    Q_OBJECT
public:
    explicit AbstractMockDevice(QMutex* mutex, QByteArray* uartBuffer, QObject *parent = nullptr);

    MockDeviceType mockDeviceType() {return m_mockDeviceType;};
    QString basePath() {return m_basePath;};
signals:
    void answerReady(const QByteArray& answerData);

    // for testing
    void impulseUploaded();
public slots:
    virtual void writeToDevice(const QByteArray& data);
    virtual void newDataRecieved();

protected:
    Parser m_parser{"Mock device recieve"};

    QMutex* m_mutex;
    QByteArray* m_uartBuffer;

    QString m_basePath;

    quint8 m_bank{0};
    quint8 m_preset{0};

    MockDeviceType m_mockDeviceType{MockDeviceType::Mock_Abstract};
};

#endif // ABSTRACTMOCKDEVICE_H
