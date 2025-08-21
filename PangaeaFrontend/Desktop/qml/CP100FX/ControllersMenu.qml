import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import Elements
import StyleSettings

import CP100FX
import CppObjects
import PangaeaBackend

RowLayout{

    MComboVertical{
        id: _comboCtrlChoice

        Layout.preferredWidth: parent.width/15
        Layout.fillHeight: true

        text: "Ctrl."

        model: _cntrlsModel
        currentIndex: 0

        ListModel{
            id: _cntrlsModel

            Component.onCompleted: {
                for(let i=1; i<=32; i++){
                    append({value: String(i)})
                }
            }
        }

    }

    MComboVertical{
        id: _srcCombo

        Layout.preferredWidth: parent.width/10
        Layout.fillHeight: true

        text: "Source"

        model: UiCore.currentDevice.controller[0].avaliableSources

        currentIndex: _cntrlsModel.length > 0 ? UiCore.currentDevice.controller[_comboCtrlChoice.currentIndex].source : 0
        // currentIndex: UiCore.currentDevice.controller[_comboCtrlChoice.currentIndex].source

        onActivated: {
            UiCore.currentDevice.controller[_comboCtrlChoice.currentIndex].setSource(_srcCombo.comboText)
        }
    }


    MComboVertical{
        Layout.fillWidth: true
        Layout.fillHeight: true

        text: "Dest."

        model: [/*0*/"Preamp On Off",
            /*1*/"Amplif On Off", "Amplif Volume", "Amplif Slave ",
            /*4*/"CabSim On Off", "Equal On Off ",
            /*6*/"Delay On Off ", "Delay Volume ", "Del. Feedback", "Delay TAP    ",
            /*10*/"Phaser On Off", "Phaser Volume", "Phaser Rate  ",
            /*13*/"Flanger OnOff", "Flang  Volume", "Flang  Rate  ",
            /*16*/"Chorus On Off", "Chorus Volume", "Chorus Rate  ",
            /*19*/"Reverb On Off", "Reverb Volume", "Reverb Time  ",
            /*22*/"Tremolo OnOff", "Tremolo Inten", "Tremolo Rate ",
            /*25*/"Preset Level ",
            /*26*/"Tremolo TAP  ",
            /*27*/"Compr On Off ", "Compr Thresh ", "Compr Volume ",
            /*30*/"Filt  On Off ", "Filt LFOrate ", "Filt freq    ",
            /*33*/"ER On Off    ", "ER Volume    ",
            /*35*/"Filt LFO TAP ",
            /*36*/"Vol Ct On Off",
            /*37*/"Cab1 Volume  ", "Cab2 Volume  ",
            /*39*/"Gate On Off  ", "Gate Thresh  ",
            /*41*/"HPF frequency", "LPF frequency", "Presence val ",
            /*44*/"Preamp Gain  ", "Preamp Volume", "Preamp Low   ", "Preamp Mid   ", "Preamp High  ",
            /*49*/"Eq Band1 Lev ", "Eq Band2 Lev ", "Eq Band3 Lev ", "Eq Band4 Lev ", "Eq Band5 Lev ",
            /*54*/"Reverb Type  "]
        currentIndex: UiCore.currentDevice.controller[_comboCtrlChoice.currentIndex].destination

        onActivated: {
            UiCore.currentDevice.controller[_comboCtrlChoice.currentIndex].destination = currentIndex
        }
    }

    ColumnLayout{
        Layout.preferredWidth: parent.width / 6
        Layout.fillHeight: true

        spacing: parent.height/7

        MBar{
            Layout.preferredWidth: parent.width
            Layout.preferredHeight: parent.height * 2/7
            Layout.topMargin: parent.height * 1/7

            textWidth: width*1/4
            barWidth: width*3/4

            text: "Min:"

            value: UiCore.currentDevice.controller[_comboCtrlChoice.currentIndex].minValue

            onUserChangedValue: {
                UiCore.currentDevice.controller[_comboCtrlChoice.currentIndex].minValue = value
            }
        }
        MBar{
            Layout.preferredWidth: parent.width
            Layout.preferredHeight: parent.height * 2/7
            Layout.bottomMargin: parent.height * 1/7

            textWidth: width*1/4
            barWidth: width*3/4

            text: "Max "

            value: UiCore.currentDevice.controller[_comboCtrlChoice.currentIndex].maxValue

            onUserChangedValue: {
                UiCore.currentDevice.controller[_comboCtrlChoice.currentIndex].maxValue= value
            }
        }
    }

    Rectangle{
        Layout.preferredWidth: 2
        Layout.fillHeight: true
    }

    MComboVertical{
        Layout.preferredWidth: parent.width/10
        Layout.fillHeight: true

        text: "PC Out"

        model: ["MIDI IN", "MAP", "SET"]

        currentIndex: UiCore.currentDevice.cntrlPcOut

        onActivated: {
            UiCore.currentDevice.cntrlPcOut = currentIndex
        }
    }

    MComboVertical{
        Layout.preferredWidth: parent.width/10
        Layout.fillHeight: true

        text: "Set"

        model: _setModel

        currentIndex: UiCore.currentDevice.cntrlSet

        onActivated: {
            UiCore.currentDevice.cntrlSet = currentIndex
        }

        ListModel{
            id: _setModel

            Component.onCompleted: {
                for(let i=1; i<=128; i++){
                    append({value: String(i)})
                }
            }
        }
    }

    Rectangle{
        Layout.preferredWidth: 2
        Layout.fillHeight: true
    }

    Row{
        Layout.preferredWidth: height * 2
        Layout.fillHeight: true

        ParameterDial{
            id: vlControl
            property Volume module: UiCore.currentDevice.presetVolume

            enabled: true

            y: parent.height/10

            width: height
            height: parent.height - parent.height/10

            controlValue: UiCore.currentDevice.presetVolume.volume
         }

        MSwitchVertical{
            id: _volCtrlSwitch

            // Layout.preferredWidth: height
            // Layout.fillHeight: true

            width: height
            height: parent.height

            y: parent.height/10

            text: "Control: "

            checked: !UiCore.currentDevice.presetVolumeControl

            onClicked: {
                UiCore.currentDevice.presetVolumeControl = !checked
            }
        }
    }

}
