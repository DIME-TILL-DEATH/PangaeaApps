#ifndef SYSTEMSETTINGSFX_H
#define SYSTEMSETTINGSFX_H

#include <QObject>

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
    STORE_EXP_LEVEL,    /*save expression level*/
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

    quint8 fsw1PressPreset[4];
    quint8 fsw2PressPreset[4];
    quint8 fsw3PressPreset[4];

    quint8 spdifOutType;

        /*num CC to express and foot*/
    quint8 exprCC;
    quint8 fswControlPressCc[3];

    quint8 midiPcInd;

    quint8 reserved1[3];

    quint8 fswMode[3];
    quint8 fswHoldType[3];

    quint8 fsw1HoldPreset[4];
    quint8 fsw2HoldPreset[4];
    quint8 fsw3HoldPreset[4];

    quint8 fswControlHoldCc[3];


    quint8 tapType;
    quint8 tunerExternal;
    quint8 fswSpeed;
    quint8 timeFormat;
    quint8 tapHigh;
    quint8 storeExprLevel;
    quint8 swapSwitch;
    quint8 tunerSpeed;

    quint8 reserved2[59];

    quint8 masterEqOn;
    qint8 masterEqLow;
    qint8 masterEqMid;
    quint8 dummy;
    qint8 masterEqHigh;

    quint8 phonesVolume;
    quint8 masterVolume;
    quint8 attenuator;

    quint8 midiMap[380];

    quint16 masterEqFreqVal;
    quint16 masterEqFreq;
}TSystemSettingsFx;


#endif // SYSTEMSETTINGSFX_H
