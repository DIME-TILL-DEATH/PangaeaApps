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

    property Cp100fx cp100fx: UiCore.currentDevice as Cp100fx

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

            controlValue: root.cp100fx.systemSettings.fswSpeed
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

            ctrlValInstance: root.cp100fx.fsw[_comboFswSelect.currentIndex].mode
            model: ["SINGLE", "DOUBLE"]
        }

        ParameterComboBox{
            width: parent.width
            height: root.stringHeight
            isHorizontal: true

            ctrlValInstance: root.cp100fx.fsw[_comboFswSelect.currentIndex].pressType
            model: ["DEFAULT", "CONTROLLER", "TUNER",
                    "PRESET SELECT" , "PRESET SEQ 2" , "PRESET SEQ 3" , "PRESET SEQ 4"]
        }

        ParameterComboBox{
            id: _pressController

            visible: root.cp100fx.fsw[_comboFswSelect.currentIndex].pressType.displayValue === FswFx.Controller

            width: parent.width
            height: root.stringHeight
            isHorizontal: true

            ctrlValInstance: root.cp100fx.fsw[_comboFswSelect.currentIndex].controllerPressNum
            model: root.cp100fx.fsw[_comboFswSelect.currentIndex].ccNames
        }

        Row{
            id: _pressMap

            width: parent.width
            height: root.stringHeight

            visible: root.cp100fx.fsw[_comboFswSelect.currentIndex].pressType.displayValue >= FswFx.PresetMap1

            MLabel{
                width: parent.width/6
                text: root.cp100fx.fsw[_comboFswSelect.currentIndex].pressType.displayValue === FswFx.PresetMap1 ? "Pres.:" : "Seq.:"

                anchors.verticalCenter: parent.verticalCenter
            }

            ParameterComboBox{
                width: parent.width/8
                height: root.stringHeight
                isHorizontal: true

                visible: root.cp100fx.fsw[_comboFswSelect.currentIndex].pressType.displayValue >= FswFx.PresetMap1

                model: root.cp100fx.strPresetNumbers

                ctrlValInstance: root.cp100fx.fsw[_comboFswSelect.currentIndex].pressPreset1

                indicator: Item{}
            }

            MLabel{
                visible: root.cp100fx.fsw[_comboFswSelect.currentIndex].pressType.displayValue >= FswFx.PresetMap2
                width: parent.width/10
                anchors.verticalCenter: parent.verticalCenter
                horizontalAlignment: Text.AlignHCenter
                text: "->"
            }

            ParameterComboBox{
                width: parent.width/8
                height: root.stringHeight
                isHorizontal: true

                visible: root.cp100fx.fsw[_comboFswSelect.currentIndex].pressType.displayValue >= FswFx.PresetMap2
                model: root.cp100fx.strPresetNumbers

                ctrlValInstance: root.cp100fx.fsw[_comboFswSelect.currentIndex].pressPreset2

                indicator: Item{}
            }

            MLabel{
                visible: root.cp100fx.fsw[_comboFswSelect.currentIndex].pressType.displayValue >= FswFx.PresetMap3
                width: parent.width/10
                anchors.verticalCenter: parent.verticalCenter
                horizontalAlignment: Text.AlignHCenter
                text: "->"
            }

            ParameterComboBox{
                width: parent.width/8
                height: root.stringHeight
                isHorizontal: true

                visible: root.cp100fx.fsw[_comboFswSelect.currentIndex].pressType.displayValue >= FswFx.PresetMap3
                model: root.cp100fx.strPresetNumbers

                ctrlValInstance: root.cp100fx.fsw[_comboFswSelect.currentIndex].pressPreset3

                indicator: Item{}
            }

            MLabel{
                visible: root.cp100fx.fsw[_comboFswSelect.currentIndex].pressType.displayValue === FswFx.PresetMap4
                horizontalAlignment: Text.AlignHCenter
                width: parent.width/10
                anchors.verticalCenter: parent.verticalCenter

                text: "->"
            }

            ParameterComboBox{
                width: parent.width/8
                height: root.stringHeight
                isHorizontal: true

                visible: root.cp100fx.fsw[_comboFswSelect.currentIndex].pressType.displayValue === FswFx.PresetMap4
                model: root.cp100fx.strPresetNumbers

                ctrlValInstance: root.cp100fx.fsw[_comboFswSelect.currentIndex].pressPreset4

                indicator: Item{}
            }
        }

        ParameterComboBox{
            id: _holdType

            visible: root.cp100fx.fsw[_comboFswSelect.currentIndex].mode.displayValue === FswFx.Double

            width: parent.width
            height: root.stringHeight
            isHorizontal: true

            ctrlValInstance: root.cp100fx.fsw[_comboFswSelect.currentIndex].holdType
            model: ["DEFAULT", "CONTROLLER", "TUNER",
                    "PRESET SELECT" , "PRESET SEQ 2" , "PRESET SEQ 3" , "PRESET SEQ 4"]

        }

        ParameterComboBox{
            id: _holdController

            visible: root.cp100fx.fsw[_comboFswSelect.currentIndex].mode.displayValue === FswFx.Double &&
                     root.cp100fx.fsw[_comboFswSelect.currentIndex].holdType.displayValue === FswFx.Controller

            width: parent.width
            height: root.stringHeight
            isHorizontal: true

            ctrlValInstance: root.cp100fx.fsw[_comboFswSelect.currentIndex].controllerHoldNum

            model: root.cp100fx.fsw[_comboFswSelect.currentIndex].ccNames

        }

        Row{
            id: _holdMap

            width: parent.width
            height: root.stringHeight

            visible: (root.cp100fx.fsw[_comboFswSelect.currentIndex].mode.displayValue === FswFx.Double &&
                      root.cp100fx.fsw[_comboFswSelect.currentIndex].holdType.displayValue >= FswFx.PresetMap1)

            MLabel{
                width: parent.width/6
                anchors.verticalCenter: parent.verticalCenter
                text: root.cp100fx.fsw[_comboFswSelect.currentIndex].holdType === FswFx.PresetMap1 ? "Pres.:" : "Seq.:"
            }

            ParameterComboBox{
                width: parent.width/8
                height: root.stringHeight
                isHorizontal: true

                model: root.cp100fx.strPresetNumbers

                ctrlValInstance: root.cp100fx.fsw[_comboFswSelect.currentIndex].holdPreset1

                indicator: Item{}
            }

            MLabel{
                visible: root.cp100fx.fsw[_comboFswSelect.currentIndex].holdType.displayValue  >= FswFx.PresetMap2
                width: parent.width/10
                anchors.verticalCenter: parent.verticalCenter
                horizontalAlignment: Text.AlignHCenter
                text: "->"
            }

            ParameterComboBox{
                width: parent.width/8
                height: root.stringHeight
                isHorizontal: true

                visible: root.cp100fx.fsw[_comboFswSelect.currentIndex].holdType.displayValue >= FswFx.PresetMap2

                model: root.cp100fx.strPresetNumbers

                ctrlValInstance: root.cp100fx.fsw[_comboFswSelect.currentIndex].holdPreset2

                indicator: Item{}
            }

            MLabel{
                visible: root.cp100fx.fsw[_comboFswSelect.currentIndex].holdType.displayValue >= FswFx.PresetMap3
                width: parent.width/10
                anchors.verticalCenter: parent.verticalCenter
                horizontalAlignment: Text.AlignHCenter
                text: "->"
            }

            ParameterComboBox{
                width: parent.width/8
                height: root.stringHeight
                isHorizontal: true

                visible: root.cp100fx.fsw[_comboFswSelect.currentIndex].holdType.displayValue >= FswFx.PresetMap3
                model: root.cp100fx.strPresetNumbers

                ctrlValInstance: root.cp100fx.fsw[_comboFswSelect.currentIndex].holdPreset3

                indicator: Item{}
            }

            MLabel{
                visible: root.cp100fx.fsw[_comboFswSelect.currentIndex].holdType.displayValue === FswFx.PresetMap4
                horizontalAlignment: Text.AlignHCenter
                anchors.verticalCenter: parent.verticalCenter
                width: parent.width/10

                text: "->"
            }

            ParameterComboBox{
                width: parent.width/8
                height: root.stringHeight
                isHorizontal: true

                visible: root.cp100fx.fsw[_comboFswSelect.currentIndex].holdType.displayValue === FswFx.PresetMap4
                model: root.cp100fx.strPresetNumbers

                ctrlValInstance: root.cp100fx.fsw[_comboFswSelect.currentIndex].holdPreset4

                indicator: Item{}
            }
        }
    }
}
