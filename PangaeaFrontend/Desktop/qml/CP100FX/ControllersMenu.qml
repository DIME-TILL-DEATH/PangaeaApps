import QtQuick
import QtQuick.Layouts

import Elements

import PangaeaFrontend
import PangaeaBackend

RowLayout{
    id: root

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

    ParameterComboBox{
        id: _srcCombo

        Layout.preferredWidth: parent.width/10
        Layout.fillHeight: true

        model: UiCore.currentDevice.controller[0].avaliableSources

        ctrlValInstance: UiCore.currentDevice.controller[_comboCtrlChoice.currentIndex].source
    }

    ParameterComboBox{
        Layout.fillWidth: true
        Layout.fillHeight: true

        model: [
            /*0*/"PR On Off    ",
            /*1*/"PA On Off    ", "PA Master    ", "PA Level     ",
            /*4*/"IR On Off    ", "EQ On Off    ",
            /*6*/"DL On Off    ",  "DL Mix      ", "DL Feedback  ", "DL TAP       ",
            /*10*/"PH On Off    ", "PH Mix      ", "PH Rate      ",
            /*13*/"FL On Off    ", "FL Mix      ", "FL Rate      ",
            /*16*/"CH On Off    ", "CH Mix      ", "CH Rate      ",
            /*19*/"RV On Off    ", "RV Mix      ", "RV Time      ",
            /*22*/"TR On Off    ", "TR Intensity ", "TR Rate      ",
            /*25*/"Preset Level ",
            /*26*/"TR TAP       ",
            /*27*/"CM On Off    ", "CM Threshold ", "CM Volume    ",
            /*30*/"RF On Off    ", "RF LFO rate  ", "RF frequency ",
            /*33*/"ER On Off    ", "ER Mix       ",
            /*35*/"RF LFO TAP   ",
            /*36*/"Vol Ct On Off",
            /*37*/"Cab1 Volume  ", "Cab2 Volume  ",
            /*39*/"GT On Off    ", "GT Threshold ",
            /*41*/"HPF frequency", "LPF frequency", "Presence val ",
            /*44*/"PR Gain      ", "PR Volume    ", "PR Low       ", "PR Mid       ", "PR High      ",
            /*49*/"EQ Band1 Lev ", "EQ Band2 Lev ", "EQ Band3 Lev ", "EQ Band4 Lev ", "EQ Band5 Lev ",
            /*54*/"RV Type      "]
        ctrlValInstance: UiCore.currentDevice.controller[_comboCtrlChoice.currentIndex].destination
    }

    ColumnLayout{
        Layout.preferredWidth: parent.width / 6
        Layout.fillHeight: true

        spacing: parent.height/7

        ParameterBar{
            Layout.preferredWidth: parent.width
            Layout.preferredHeight: parent.height * 2/7
            Layout.topMargin: parent.height * 1/7

            controlValue: UiCore.currentDevice.controller[_comboCtrlChoice.currentIndex].minValue
        }
        ParameterBar{
            Layout.preferredWidth: parent.width
            Layout.preferredHeight: parent.height * 2/7
            Layout.bottomMargin: parent.height * 1/7

            controlValue: UiCore.currentDevice.controller[_comboCtrlChoice.currentIndex].maxValue
        }
    }

    Rectangle{
        Layout.preferredWidth: 2
        Layout.fillHeight: true
    }

    ParameterComboBox{
        Layout.preferredWidth: parent.width/10
        Layout.fillHeight: true

        model: ["MIDI IN", "MAP", "SET"]

        ctrlValInstance: UiCore.currentDevice.controlsPresetFx.cntrlPcOut
    }

    ParameterComboBox{
        Layout.preferredWidth: parent.width/10
        Layout.fillHeight: true

        model: _setModel

        ctrlValInstance: UiCore.currentDevice.controlsPresetFx.cntrlSet

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

            y: parent.height/10

            enabled: true

            width: height
            height: parent.height - parent.height/10

            controlValue: UiCore.currentDevice.controlsPresetFx.presetVolume
         }

        ParameterSwitch{
            id: _volCtrlSwitch

            width: height
            height: parent.height

            invertedValue: 1

            y: parent.height/10

            ctrlValInstance: UiCore.currentDevice.controlsPresetFx.presetCtrlVolume
        }
    }

}
