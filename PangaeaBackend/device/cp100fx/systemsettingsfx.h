#ifndef SYSTEMSETTINGSFX_H
#define SYSTEMSETTINGSFX_H

#include <QObject>
#include <QQmlEngine>

#include "abstractdevice.h"

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



class SystemSettingsFx : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("")

    Q_PROPERTY(quint8 mode READ mode WRITE setMode NOTIFY settingsChanged FINAL)
    Q_PROPERTY(quint8 cabNumber READ cabNumber WRITE setCabNumber NOTIFY settingsChanged FINAL)
    Q_PROPERTY(quint8 midiChannel READ midiChannel WRITE setMidiChannel NOTIFY settingsChanged FINAL)

    Q_PROPERTY(quint8 exprOn READ exprOn WRITE setExprOn NOTIFY settingsChanged FINAL)
    Q_PROPERTY(quint8 exprType READ exprType WRITE setExprType NOTIFY settingsChanged FINAL)
    Q_PROPERTY(quint8 exprCC READ exprCC WRITE setExprCC NOTIFY settingsChanged FINAL)
    Q_PROPERTY(quint8 exprStoreLevel READ exprStoreLevel WRITE setExprStoreLevel NOTIFY settingsChanged FINAL)
    Q_PROPERTY(quint8 spdif READ spdif WRITE setSpdif NOTIFY settingsChanged FINAL)
    Q_PROPERTY(quint8 tempo READ tempo WRITE setTempo NOTIFY settingsChanged FINAL)
    Q_PROPERTY(quint8 tunerControl READ tunerControl WRITE setTunerControl NOTIFY settingsChanged FINAL)
    Q_PROPERTY(quint8 tunerCC READ tunerCC WRITE setTunerCC NOTIFY settingsChanged FINAL)
    Q_PROPERTY(quint8 timeFormat READ timeFormat WRITE setTimeFormat NOTIFY settingsChanged FINAL)
    Q_PROPERTY(quint8 swapConf READ swapConf WRITE setSwapConf NOTIFY settingsChanged FINAL)

    Q_PROPERTY(quint8 tunerSpeed READ tunerSpeed WRITE setTunerSpeed NOTIFY settingsChanged FINAL)
    Q_PROPERTY(quint8 fswSpeed READ fswSpeed WRITE setFswSpeed NOTIFY settingsChanged FINAL)

    Q_PROPERTY(QList<quint8> midiPcMap READ midiPcMap NOTIFY settingsChanged FINAL)
public:
    explicit SystemSettingsFx(AbstractDevice *owner);

    void setSettings(TSystemSettingsFx settings);

    quint8 mode() const;
    void setMode(quint8 newMode);

    quint8 midiChannel() const;
    void setMidiChannel(quint8 newMidiChannel);

    quint8 cabNumber() const;
    void setCabNumber(quint8 newCabNumber);

    quint8 exprOn() const;
    void setExprOn(quint8 newExprOn);

    quint8 exprType() const;
    void setExprType(quint8 newExprType);

    quint8 exprCC() const;
    void setExprCC(quint8 newExprCC);

    quint8 exprStoreLevel() const;
    void setExprStoreLevel(quint8 newExprStoreLevel);

    quint8 spdif() const;
    void setSpdif(quint8 newSpdif);

    quint8 tempo() const;
    void setTempo(quint8 newTempo);

    quint8 tunerControl() const;
    void setTunerControl(quint8 newTunerControl);

    quint8 timeFormat() const;
    void setTimeFormat(quint8 newTimeFormat);

    quint8 swapConf() const;
    void setSwapConf(quint8 newSwapConf);

    quint8 tunerSpeed() const;
    void setTunerSpeed(quint8 newTunerSpeed);

    quint8 fswSpeed() const;
    void setFswSpeed(quint8 newFswSpeed);

    quint8 tunerCC() const;
    void setTunerCC(quint8 newTunerCC);

    QList<quint8> midiPcMap() const;
    Q_INVOKABLE void setMidiPcMap(quint8 pcNumber, quint8 presetNumber);

signals:
    void settingsChanged();
private:
    AbstractDevice* m_owner;

    quint8 m_mode;
    quint8 m_midiChannel;
    quint8 m_cabNumber;
    quint8 m_exprOn;
    quint8 m_exprType;
    quint8 m_exprCC;
    quint8 m_exprStoreLevel;
    quint8 m_spdif;
    quint8 m_tempo;
    quint8 m_tunerControl;
    quint8 m_timeFormat;
    quint8 m_swapConf;
    quint8 m_tunerSpeed;
    quint8 m_fswSpeed;
    quint8 m_tunerCC;

    QList<quint8> m_midiPcMap;

    void sendData(const QByteArray& data);
};

#endif // SYSTEMSETTINGSFX_H
