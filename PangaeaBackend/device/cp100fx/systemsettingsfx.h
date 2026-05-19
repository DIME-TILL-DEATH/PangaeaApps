#ifndef SYSTEMSETTINGSFX_H
#define SYSTEMSETTINGSFX_H

#include <QObject>
#include <QQmlEngine>

#include "abstractdevice.h"
#include "abstractmodule.h"
#include "controlvalue.h"

enum
{
    CAB_SIM_DISABLED,
    MIDI_CHANNEL,
    CAB_SIM_CONFIG,
    EXPRESSION_TYPE,

    FSW1_PRESS_TYPE, FSW2_PRESS_TYPE, FSW3_PRESS_TYPE,

    /*calibration*/
    EXPR_CAL_MIN_HI, EXPR_CAL_MIN_LO, EXPR_CAL_MAX_HI, EXPR_CAL_MAX_LO,

    /*preset map to foot press*/
    FSW1_PRESS_PR1, FSW1_PRESS_PR2, FSW1_PRESS_PR3, FSW1_PRESS_PR4,
    FSW2_PRESS_PR1, FSW2_PRESS_PR2, FSW2_PRESS_PR3, FSW2_PRESS_PR4,
    FSW3_PRESS_PR1, FSW3_PRESS_PR2, FSW3_PRESS_PR3, FSW3_PRESS_PR4,

    SPDIF_OUT_TYPE,

    /*num CC to express and foot*/
    EXP_CCN,
    FSW1_CTRL_PRESS_CC, FSW2_CTRL_PRESS_CC, FSW3_CTRL_PRESS_CC,

    MIDI_PC_IND, /*service*/
    /*31 - last num preset*/

    /*FSW button mode*/
    FSW1_MODE = 32, FSW2_MODE, FSW3_MODE,

    FSW1_HOLD_TYPE, FSW2_HOLD_TYPE, FSW3_HOLD_TYPE,

    /*preset map to foot hold*/
    FSW1_HOLD_PR1, FSW1_HOLD_PR2, FSW1_HOLD_PR3, FSW1_HOLD_PR4,
    FSW2_HOLD_PR1, FSW2_HOLD_PR2, FSW2_HOLD_PR3, FSW2_HOLD_PR4,
    FSW3_HOLD_PR1, FSW3_HOLD_PR2, FSW3_HOLD_PR3, FSW3_HOLD_PR4,

    FSW1_CTRL_HOLD_CC, FSW2_CTRL_HOLD_CC, FSW3_CTRL_HOLD_CC,

    TAP_TYPE,
    TUNER_EXTERNAL,
    FSW_SPEED,
    TIME_FORMAT,	// Sec, BPM
    TAP_HIGH,           /*global temp hi*/
    EXPR_STORE_LEVEL,    /*save expression level*/
    SWAP_SWITCH,
    TUNER_SPEED,

    MASTER_EQ_ON = 120,
    MASTER_EQ_LOW = 121,
    MASTER_EQ_MID = 122,
    MASTER_EQ_HIGH = 124,

    PHONES_VOLUME = 125,
    MASTER_VOLUME,
    ATTENUATOR,

    MIDI_MAP_START,

    MASTER_EQ_FREQ_VAL_LO = 508,
    MASTER_EQ_FREQ_VAL_HI,

    MASTER_EQ_FREQ_LO = 510,
    MASTER_EQ_FREQ_HI
};

typedef struct
{
    quint8 cabSimDisabled;
    quint8 midiChannel;
    quint8 cabSimConfig;
    quint8 expressionType;

    quint8 fswPressType[3];

    /*calibration*/
    quint8 exprCalMinHi;
    quint8 expCalMinLo;
    quint8 exprCalMaxHi;
    quint8 expCalMaxLo;

    quint8 fswPressPreset[3][4];

    quint8 spdifOutType;

    /*num CC to express and foot*/
    quint8 exprCC;
    quint8 fswControlPressCc[3];

    quint8 midiPcInd;

    quint8 reserved1[2];

    quint8 lastPresetNum;

    quint8 fswMode[3];
    quint8 fswHoldType[3];

    quint8 fswHoldPreset[3][4];

    quint8 fswControlHoldCc[3];


    quint8 tapType;
    quint8 tunerExternal;
    quint8 fswSpeed;
    quint8 timeFormat;
    quint8 tapHigh;
    quint8 storeExprLevel;
    quint8 swapSwitch;
    quint8 tunerSpeed;

    quint8 tapScreenPopup;
    quint8 eqSreenMode;

    quint8 attenuatorMode;

    quint8 reserved2[56];

    quint8 masterEqOn;
    quint8 masterEqLow;
    quint8 masterEqMid;
    quint8 dummy;
    quint8 masterEqHigh;

    quint8 phonesVolume;
    quint8 masterVolume;
    quint8 attenuator;

    quint8 midiMap[380];

    quint16 masterEqFreqVal;
    quint16 masterEqFreq;
}TSystemSettingsFx;



class SystemSettingsFx : public AbstractModule
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("")

    Q_PROPERTY(ControlValue* mode READ mode NOTIFY settingsChanged FINAL)
    Q_PROPERTY(ControlValue* cabNumber READ cabNumber NOTIFY settingsChanged FINAL)
    Q_PROPERTY(ControlValue* midiChannel READ midiChannel NOTIFY settingsChanged FINAL)

    Q_PROPERTY(ControlValue* exprOn READ exprOn NOTIFY settingsChanged FINAL)
    Q_PROPERTY(ControlValue* exprType READ exprType NOTIFY settingsChanged FINAL)
    Q_PROPERTY(ControlValue* exprCC READ exprCC NOTIFY settingsChanged FINAL)
    Q_PROPERTY(ControlValue* exprStoreLevel READ exprStoreLevel NOTIFY settingsChanged FINAL)
    Q_PROPERTY(ControlValue* spdif READ spdif NOTIFY settingsChanged FINAL)
    Q_PROPERTY(ControlValue* tempo READ tempo NOTIFY settingsChanged FINAL)
    Q_PROPERTY(ControlValue* tunerControl READ tunerControl NOTIFY settingsChanged FINAL)
    Q_PROPERTY(ControlValue* tunerCC READ tunerCC NOTIFY settingsChanged FINAL)
    Q_PROPERTY(ControlValue* timeFormat READ timeFormat NOTIFY settingsChanged FINAL)
    Q_PROPERTY(ControlValue* swapConf READ swapConf NOTIFY settingsChanged FINAL)

    Q_PROPERTY(ControlValue* tunerSpeed READ tunerSpeed NOTIFY settingsChanged FINAL)
    Q_PROPERTY(ControlValue* fswSpeed READ fswSpeed NOTIFY settingsChanged FINAL)

    Q_PROPERTY(QList<ControlValue*> midiPcMap READ midiPcMap NOTIFY settingsChanged FINAL)
public:
    explicit SystemSettingsFx(AbstractDevice *owner);

    void setSettings(TSystemSettingsFx settings);

    ControlValue* mode() const;
    ControlValue* midiChannel() const;
    ControlValue* cabNumber() const;
    ControlValue* exprOn() const;
    ControlValue* exprType() const;
    ControlValue* exprCC() const;
    ControlValue* exprStoreLevel() const;
    ControlValue* spdif() const;
    ControlValue* tempo() const;
    ControlValue* tunerControl() const;
    ControlValue* timeFormat() const;
    ControlValue* swapConf() const;
    ControlValue* tunerSpeed() const;
    ControlValue* fswSpeed() const;
    ControlValue* tunerCC() const;

    QList<ControlValue *> midiPcMap() const;
signals:
    void settingsChanged();
private:
    AbstractDevice* m_owner;

    ControlValue* m_mode = nullptr;
    ControlValue* m_midiChannel = nullptr;
    ControlValue* m_cabNumber = nullptr;
    ControlValue* m_exprOn = nullptr;
    ControlValue* m_exprType = nullptr;
    ControlValue* m_exprCC = nullptr;
    ControlValue* m_exprStoreLevel = nullptr;
    ControlValue* m_spdif = nullptr;
    ControlValue* m_tempo = nullptr;
    ControlValue* m_tunerControl = nullptr;
    ControlValue* m_timeFormat = nullptr;
    ControlValue* m_swapConf = nullptr;
    ControlValue* m_tunerSpeed = nullptr;
    ControlValue* m_fswSpeed = nullptr;
    ControlValue* m_tunerCC = nullptr;

    QList<ControlValue*> m_midiPcMap;

    void sendData(const QByteArray& data);

    void exprOnControlSetter(qint32 value);
    void exprOnDisplaySetter(double value);
    void exprTypeControlSetter(qint32 value);

    void tunerCtrlValueSetter(qint32 value);
    void tunerCtrlDisplaySetter(double value);
    void tunerCcSetter(qint32 value);
};

#endif // SYSTEMSETTINGSFX_H
