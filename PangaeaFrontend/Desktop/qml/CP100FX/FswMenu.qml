import QtQuick
import QtQuick.Controls

import Elements
import StyleSettings

import PangaeaFrontend
import PangaeaBackend

Rectangle{
    id: root

    color: "transparent"
    border.width: 1
    border.color: Style.currentTheme.borderOn

    property int stringHeight: parent.height/16

    Column{
        width: parent.width * 0.9
        height: parent.height * 0.9
        anchors.centerIn: parent

        spacing: root.stringHeight

        MLabel{
            height: root.stringHeight
            anchors.horizontalCenter: parent.horizontalCenter
            text: "Footswitch settings:"
        }

        ParameterBar{
            width: parent.width
            height: root.stringHeight

            controlValue: UiCore.currentDevice.systemSettings.fswSpeed
        }

        MComboHorizontal{
            id: _comboFswSelect
            width: parent.width
            height: root.stringHeight

            text: "Footswitch: "

            currentIndex: 0
            model: ["DOWN", "CONFIRM", "UP"]
        }

        ParameterComboBox{
            width: parent.width
            height: root.stringHeight
            isHorizontal: true

            ctrlValInstance: UiCore.currentDevice.fsw[_comboFswSelect.currentIndex].mode
            model: ["SINGLE", "DOUBLE"]
        }

        ParameterComboBox{
            width: parent.width
            height: root.stringHeight
            isHorizontal: true

            ctrlValInstance: UiCore.currentDevice.fsw[_comboFswSelect.currentIndex].pressType
            model: ["DEFAULT", "CONTROLLER", "TUNER",
                    "PRESET SELECT" , "PRESET SEQ 2" , "PRESET SEQ 3" , "PRESET SEQ 4"]
        }

        ParameterComboBox{
            id: _pressController

            visible: UiCore.currentDevice.fsw[_comboFswSelect.currentIndex].pressType.displayValue === FswFx.Controller

            width: parent.width
            height: root.stringHeight
            isHorizontal: true

            ctrlValInstance: UiCore.currentDevice.fsw[_comboFswSelect.currentIndex].controllerPressNum
            model: UiCore.currentDevice.fsw[_comboFswSelect.currentIndex].ccNames
        }

        Row{
            id: _pressMap

            width: parent.width
            height: root.stringHeight

            visible: UiCore.currentDevice.fsw[_comboFswSelect.currentIndex].pressType.displayValue >= FswFx.PresetMap1

            MLabel{
                width: parent.width/6
                text: UiCore.currentDevice.fsw[_comboFswSelect.currentIndex].pressType.displayValue === FswFx.PresetMap1 ? "Pres.:" : "Seq.:"

                anchors.verticalCenter: parent.verticalCenter
            }

            ParameterComboBox{
                width: parent.width/8
                height: root.stringHeight
                isHorizontal: true

                visible: UiCore.currentDevice.fsw[_comboFswSelect.currentIndex].pressType.displayValue >= FswFx.PresetMap1

                model: UiCore.currentDevice.strPresetNumbers

                ctrlValInstance: UiCore.currentDevice.fsw[_comboFswSelect.currentIndex].pressPreset1

                indicator: Item{}
            }

            MLabel{
                visible: UiCore.currentDevice.fsw[_comboFswSelect.currentIndex].pressType.displayValue >= FswFx.PresetMap2
                width: parent.width/10
                anchors.verticalCenter: parent.verticalCenter
                horizontalAlignment: Text.AlignHCenter
                text: "->"
            }

            ParameterComboBox{
                width: parent.width/8
                height: root.stringHeight
                isHorizontal: true

                visible: UiCore.currentDevice.fsw[_comboFswSelect.currentIndex].pressType.displayValue >= FswFx.PresetMap2
                model: UiCore.currentDevice.strPresetNumbers

                ctrlValInstance: UiCore.currentDevice.fsw[_comboFswSelect.currentIndex].pressPreset2

                indicator: Item{}
            }

            MLabel{
                visible: UiCore.currentDevice.fsw[_comboFswSelect.currentIndex].pressType.displayValue >= FswFx.PresetMap3
                width: parent.width/10
                anchors.verticalCenter: parent.verticalCenter
                horizontalAlignment: Text.AlignHCenter
                text: "->"
            }

            ParameterComboBox{
                width: parent.width/8
                height: root.stringHeight
                isHorizontal: true

                visible: UiCore.currentDevice.fsw[_comboFswSelect.currentIndex].pressType.displayValue >= FswFx.PresetMap3
                model: UiCore.currentDevice.strPresetNumbers

                ctrlValInstance: UiCore.currentDevice.fsw[_comboFswSelect.currentIndex].pressPreset3

                indicator: Item{}
            }

            MLabel{
                visible: UiCore.currentDevice.fsw[_comboFswSelect.currentIndex].pressType.displayValue === FswFx.PresetMap4
                horizontalAlignment: Text.AlignHCenter
                width: parent.width/10
                anchors.verticalCenter: parent.verticalCenter

                text: "->"
            }

            ParameterComboBox{
                width: parent.width/8
                height: root.stringHeight
                isHorizontal: true

                visible: UiCore.currentDevice.fsw[_comboFswSelect.currentIndex].pressType.displayValue === FswFx.PresetMap4
                model: UiCore.currentDevice.strPresetNumbers

                ctrlValInstance: UiCore.currentDevice.fsw[_comboFswSelect.currentIndex].pressPreset4

                indicator: Item{}
            }
        }

        ParameterComboBox{
            id: _holdType

            visible: UiCore.currentDevice.fsw[_comboFswSelect.currentIndex].mode.displayValue === FswFx.Double

            width: parent.width
            height: root.stringHeight
            isHorizontal: true

            ctrlValInstance: UiCore.currentDevice.fsw[_comboFswSelect.currentIndex].holdType
            model: ["DEFAULT", "CONTROLLER", "TUNER",
                    "PRESET SELECT" , "PRESET SEQ 2" , "PRESET SEQ 3" , "PRESET SEQ 4"]

        }

        ParameterComboBox{
            id: _holdController

            visible: UiCore.currentDevice.fsw[_comboFswSelect.currentIndex].mode.displayValue === FswFx.Double &&
                     UiCore.currentDevice.fsw[_comboFswSelect.currentIndex].holdType.displayValue === FswFx.Controller

            width: parent.width
            height: root.stringHeight
            isHorizontal: true

            ctrlValInstance: UiCore.currentDevice.fsw[_comboFswSelect.currentIndex].controllerHoldNum

            model: UiCore.currentDevice.fsw[_comboFswSelect.currentIndex].ccNames

        }

        Row{
            id: _holdMap

            width: parent.width
            height: root.stringHeight

            visible: (UiCore.currentDevice.fsw[_comboFswSelect.currentIndex].mode.displayValue === FswFx.Double &&
                      UiCore.currentDevice.fsw[_comboFswSelect.currentIndex].holdType.displayValue >= FswFx.PresetMap1)

            MLabel{
                width: parent.width/6
                anchors.verticalCenter: parent.verticalCenter
                text: UiCore.currentDevice.fsw[_comboFswSelect.currentIndex].holdType === FswFx.PresetMap1 ? "Pres.:" : "Seq.:"
            }

            ParameterComboBox{
                width: parent.width/8
                height: root.stringHeight
                isHorizontal: true

                model: UiCore.currentDevice.strPresetNumbers

                ctrlValInstance: UiCore.currentDevice.fsw[_comboFswSelect.currentIndex].holdPreset1

                indicator: Item{}
            }

            MLabel{
                visible: UiCore.currentDevice.fsw[_comboFswSelect.currentIndex].holdType.displayValue  >= FswFx.PresetMap2
                width: parent.width/10
                anchors.verticalCenter: parent.verticalCenter
                horizontalAlignment: Text.AlignHCenter
                text: "->"
            }

            ParameterComboBox{
                width: parent.width/8
                height: root.stringHeight
                isHorizontal: true

                visible: UiCore.currentDevice.fsw[_comboFswSelect.currentIndex].holdType.displayValue >= FswFx.PresetMap2

                model: UiCore.currentDevice.strPresetNumbers

                ctrlValInstance: UiCore.currentDevice.fsw[_comboFswSelect.currentIndex].holdPreset2

                indicator: Item{}
            }

            MLabel{
                visible: UiCore.currentDevice.fsw[_comboFswSelect.currentIndex].holdType.displayValue >= FswFx.PresetMap3
                width: parent.width/10
                anchors.verticalCenter: parent.verticalCenter
                horizontalAlignment: Text.AlignHCenter
                text: "->"
            }

            ParameterComboBox{
                width: parent.width/8
                height: root.stringHeight
                isHorizontal: true

                visible: UiCore.currentDevice.fsw[_comboFswSelect.currentIndex].holdType.displayValue >= FswFx.PresetMap3
                model: UiCore.currentDevice.strPresetNumbers

                ctrlValInstance: UiCore.currentDevice.fsw[_comboFswSelect.currentIndex].holdPreset3

                indicator: Item{}
            }

            MLabel{
                visible: UiCore.currentDevice.fsw[_comboFswSelect.currentIndex].holdType.displayValue === FswFx.PresetMap4
                horizontalAlignment: Text.AlignHCenter
                anchors.verticalCenter: parent.verticalCenter
                width: parent.width/10

                text: "->"
            }

            ParameterComboBox{
                width: parent.width/8
                height: root.stringHeight
                isHorizontal: true

                visible: UiCore.currentDevice.fsw[_comboFswSelect.currentIndex].holdType.displayValue === FswFx.PresetMap4
                model: UiCore.currentDevice.strPresetNumbers

                ctrlValInstance: UiCore.currentDevice.fsw[_comboFswSelect.currentIndex].holdPreset4

                indicator: Item{}
            }
        }
    }
}
