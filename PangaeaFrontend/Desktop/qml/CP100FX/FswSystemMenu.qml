import QtQuick 2.15
import QtQuick.Controls.Fusion
import QtQuick.Dialogs
import QtCore

import QtQuick.Window 2.15

import Elements 1.0
import ModulesFX 1.0
import StyleSettings 1.0
import Layouts 1.0

import CP100FX 1.0

import QtQuick.Layouts

import CppObjects
import PangaeaBackend

Rectangle{
    id: _fswMenuRect

    color: "transparent"
    border.width: 1
    border.color: Style.borderOn

    property int stringHeight: parent.height/16

    Column{
        width: parent.width * 0.9
        height: parent.height * 0.9
        anchors.centerIn: parent

        spacing: _fswMenuRect.stringHeight

        MLabel{
            height: _fswMenuRect.stringHeight
            anchors.horizontalCenter: parent.horizontalCenter
            text: "Footswitch settings:"
        }

        MBar{
            width: parent.width
            height: _fswMenuRect.stringHeight

            text: "FSW speed: "

            value: UiCore.currentDevice.systemSettings.fswSpeed
        }

        MComboHorizontal{
            id: _comboFswSelect
            width: parent.width
            height: _fswMenuRect.stringHeight

            text: "Footswitch: "

            currentIndex: 0
            model: ["DOWN", "CONFIRM", "UP"]
        }

        MComboHorizontal{
            width: parent.width
            height: _fswMenuRect.stringHeight

            text: "Mode: "

            currentIndex: UiCore.currentDevice.fsw[_comboFswSelect.currentIndex].mode
            model: ["SINGLE", "DOUBLE"]

            onActivated: (index) => {

                UiCore.currentDevice.fsw[_comboFswSelect.currentIndex].mode = currentIndex;
            }
        }

        MComboHorizontal{
            id: _pressType

            width: parent.width
            height: _fswMenuRect.stringHeight

            text: "Press type: "

            currentIndex: UiCore.currentDevice.fsw[_comboFswSelect.currentIndex].pressType
            model: ["DEFAULT", "CONTROLLER", "TUNER",
                    "PRESET SELECT" , "PRESET SEQ 2" , "PRESET SEQ 3" , "PRESET SEQ 4"]

            onActivated: (index) => {
                UiCore.currentDevice.fsw[_comboFswSelect.currentIndex].pressType = currentIndex;
            }
        }

        MComboHorizontal{
            id: _pressController

            visible: UiCore.currentDevice.fsw[_comboFswSelect.currentIndex].pressType === FswFx.Controller

            width: parent.width
            height: _fswMenuRect.stringHeight

            text: "Press CC#: "

            currentIndex: UiCore.currentDevice.fsw[_comboFswSelect.currentIndex].controllerPressNum
            model: UiCore.currentDevice.fsw[_comboFswSelect.currentIndex].ccNames

            onActivated: (index) => {
                UiCore.currentDevice.fsw[_comboFswSelect.currentIndex].controllerPressNum = currentIndex;
            }
        }

        Row{
            id: _pressMap

            width: parent.width
            height: _fswMenuRect.stringHeight

            visible: (UiCore.currentDevice.fsw[_comboFswSelect.currentIndex].pressType >= FswFx.PresetMap1)

            MLabel{
                width: parent.width/6
                text: UiCore.currentDevice.fsw[_comboFswSelect.currentIndex].pressType === FswFx.PresetMap1 ? "Pres.:" : "Seq.:"

                anchors.verticalCenter: parent.verticalCenter
            }

            ComboBox{
                width: parent.width/8
                anchors.verticalCenter: parent.verticalCenter

                model: UiCore.currentDevice.strPresetNumbers

                currentIndex: UiCore.currentDevice.fsw[_comboFswSelect.currentIndex].pressPreset1

                indicator: Item{}

                onActivated: UiCore.currentDevice.fsw[_comboFswSelect.currentIndex].pressPreset1 = currentIndex
            }

            MLabel{
                visible: UiCore.currentDevice.fsw[_comboFswSelect.currentIndex].pressType >= FswFx.PresetMap2
                width: parent.width/10
                anchors.verticalCenter: parent.verticalCenter
                horizontalAlignment: Text.AlignHCenter
                text: "->"
            }

            ComboBox{
                width: parent.width/8
                anchors.verticalCenter: parent.verticalCenter
                visible: UiCore.currentDevice.fsw[_comboFswSelect.currentIndex].pressType >= FswFx.PresetMap2
                model: UiCore.currentDevice.strPresetNumbers

                currentIndex: UiCore.currentDevice.fsw[_comboFswSelect.currentIndex].pressPreset2

                indicator: Item{}

                onActivated: UiCore.currentDevice.fsw[_comboFswSelect.currentIndex].pressPreset2 = currentIndex
            }

            MLabel{
                visible: UiCore.currentDevice.fsw[_comboFswSelect.currentIndex].pressType >= FswFx.PresetMap3
                width: parent.width/10
                anchors.verticalCenter: parent.verticalCenter
                horizontalAlignment: Text.AlignHCenter
                text: "->"
            }

            ComboBox{
                width: parent.width/8
                anchors.verticalCenter: parent.verticalCenter
                visible: UiCore.currentDevice.fsw[_comboFswSelect.currentIndex].pressType >= FswFx.PresetMap3
                model: UiCore.currentDevice.strPresetNumbers

                currentIndex: UiCore.currentDevice.fsw[_comboFswSelect.currentIndex].pressPreset3

                indicator: Item{}

                onActivated: UiCore.currentDevice.fsw[_comboFswSelect.currentIndex].pressPreset3 = currentIndex
            }

            MLabel{
                visible: UiCore.currentDevice.fsw[_comboFswSelect.currentIndex].pressType === FswFx.PresetMap4
                horizontalAlignment: Text.AlignHCenter
                width: parent.width/10
                anchors.verticalCenter: parent.verticalCenter

                text: "->"
            }

            ComboBox{
                width: parent.width/8
                anchors.verticalCenter: parent.verticalCenter
                visible: UiCore.currentDevice.fsw[_comboFswSelect.currentIndex].pressType === FswFx.PresetMap4
                model: UiCore.currentDevice.strPresetNumbers

                currentIndex: UiCore.currentDevice.fsw[_comboFswSelect.currentIndex].pressPreset4

                indicator: Item{}

                onActivated: UiCore.currentDevice.fsw[_comboFswSelect.currentIndex].pressPreset4 = currentIndex
            }
        }

        MComboHorizontal{
            id: _holdType

            visible: UiCore.currentDevice.fsw[_comboFswSelect.currentIndex].mode === FswFx.Double

            width: parent.width
            height: _fswMenuRect.stringHeight

            text: "Hold type: "

            currentIndex: UiCore.currentDevice.fsw[_comboFswSelect.currentIndex].holdType
            model: ["DEFAULT", "CONTROLLER", "TUNER",
                    "PRESET SELECT" , "PRESET SEQ 2" , "PRESET SEQ 3" , "PRESET SEQ 4"]

            onActivated: (index) => {
                currentIndex = index;
                UiCore.currentDevice.fsw[_comboFswSelect.currentIndex].holdType = currentIndex;
            }
        }

        MComboHorizontal{
            id: _holdController

            visible: UiCore.currentDevice.fsw[_comboFswSelect.currentIndex].mode === FswFx.Double
                && UiCore.currentDevice.fsw[_comboFswSelect.currentIndex].holdType === FswFx.Controller

            width: parent.width
            height: _fswMenuRect.stringHeight

            text: "Hold CC#: "

            currentIndex: UiCore.currentDevice.fsw[_comboFswSelect.currentIndex].controllerHoldNum

            model: UiCore.currentDevice.fsw[_comboFswSelect.currentIndex].ccNames

            onActivated: (index) => {
                UiCore.currentDevice.fsw[_comboFswSelect.currentIndex].controllerHoldNum = currentIndex;
            }
        }

        Row{
            id: _holdMap

            width: parent.width
            height: _fswMenuRect.stringHeight

            visible: (UiCore.currentDevice.fsw[_comboFswSelect.currentIndex].holdType >= FswFx.PresetMap1)

            MLabel{
                width: parent.width/6
                anchors.verticalCenter: parent.verticalCenter
                text: UiCore.currentDevice.fsw[_comboFswSelect.currentIndex].holdType === FswFx.PresetMap1 ? "Pres.:" : "Seq.:"
            }

            ComboBox{
                width: parent.width/8
                anchors.verticalCenter: parent.verticalCenter
                model: UiCore.currentDevice.strPresetNumbers

                currentIndex: UiCore.currentDevice.fsw[_comboFswSelect.currentIndex].holdPreset1

                indicator: Item{}

                onActivated: UiCore.currentDevice.fsw[_comboFswSelect.currentIndex].holdPreset1 = currentIndex
            }

            MLabel{
                visible: UiCore.currentDevice.fsw[_comboFswSelect.currentIndex].holdType >= FswFx.PresetMap2
                width: parent.width/10
                anchors.verticalCenter: parent.verticalCenter
                horizontalAlignment: Text.AlignHCenter
                text: "->"
            }

            ComboBox{
                width: parent.width/8
                anchors.verticalCenter: parent.verticalCenter
                visible: UiCore.currentDevice.fsw[_comboFswSelect.currentIndex].holdType >= FswFx.PresetMap2
                model: UiCore.currentDevice.strPresetNumbers

                currentIndex: UiCore.currentDevice.fsw[_comboFswSelect.currentIndex].holdPreset2

                indicator: Item{}

                onActivated: UiCore.currentDevice.fsw[_comboFswSelect.currentIndex].holdPreset2 = currentIndex
            }

            MLabel{
                visible: UiCore.currentDevice.fsw[_comboFswSelect.currentIndex].holdType >= FswFx.PresetMap3
                width: parent.width/10
                anchors.verticalCenter: parent.verticalCenter
                horizontalAlignment: Text.AlignHCenter
                text: "->"
            }

            ComboBox{
                width: parent.width/8
                anchors.verticalCenter: parent.verticalCenter
                visible: UiCore.currentDevice.fsw[_comboFswSelect.currentIndex].holdType >= FswFx.PresetMap3
                model: UiCore.currentDevice.strPresetNumbers

                currentIndex: UiCore.currentDevice.fsw[_comboFswSelect.currentIndex].holdPreset3

                indicator: Item{}

                onActivated: UiCore.currentDevice.fsw[_comboFswSelect.currentIndex].holdPreset3 = currentIndex
            }

            MLabel{
                visible: UiCore.currentDevice.fsw[_comboFswSelect.currentIndex].holdType === FswFx.PresetMap4
                horizontalAlignment: Text.AlignHCenter
                anchors.verticalCenter: parent.verticalCenter
                width: parent.width/10

                text: "->"
            }

            ComboBox{
                width: parent.width/8
                anchors.verticalCenter: parent.verticalCenter
                visible: UiCore.currentDevice.fsw[_comboFswSelect.currentIndex].holdType === FswFx.PresetMap4
                model: UiCore.currentDevice.strPresetNumbers

                currentIndex: UiCore.currentDevice.fsw[_comboFswSelect.currentIndex].holdPreset4

                indicator: Item{}

                onActivated: UiCore.currentDevice.fsw[_comboFswSelect.currentIndex].holdPreset4 = currentIndex
            }
        }
    }
}
