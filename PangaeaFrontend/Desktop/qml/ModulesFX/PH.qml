import QtQuick 2.15
import QtQuick.Controls 2.15

import StyleSettings 1.0
import Elements 1.0

import CppObjects
import PangaeaBackend

BaseModule{
    id: main

    property Phaser module: _moduleLoader.selectedModuleInstance

    contentItem: Row
    {
        anchors.fill: parent


        ParameterDial{
            width: main.dialWidth
            height: main.dialHeight

            controlValue: module.mix
        }


        ParameterDial{
            width: main.dialWidth
            height: main.dialHeight

            controlValue: module.rate
        }

        ParameterDial{
            width: main.dialWidth
            height: main.dialHeight

            controlValue: module.center
        }

        ParameterDial{
            width: main.dialWidth
            height: main.dialHeight

            controlValue: module.width
        }

        ParameterDial{
            width: main.dialWidth
            height: main.dialHeight

            controlValue: module.feedback
        }

        // ParameterComboBox{
        //     width: main.dialWidth
        //     height: main.dialHeight

        //     anchors.verticalCenter: parent.verticalCenter

        //     ctrlValInstance: module.stages

        //     moduleOn: module.moduleEnabled

        //     model: ["4", "6", "8"]
        // }

        // Кастомный из того что значения Stages в устройстве 0, 2, 4
        Item{
            id: _stagesCombo

            width: main.dialWidth
            height: main.dialHeight

            anchors.verticalCenter: parent.verticalCenter

            property bool moduleOn: module.moduleEnabled
            property ControlValue ctrlValInstance: module.stages

            Column{
                anchors.fill: parent

                Item{
                    id: _editValueItem


                    height: parent.height/3
                    width: parent.width

                    Text{
                        id: textValue
                        anchors.fill: parent

                        text: _stagesCombo.ctrlValInstance.name
                        font.pixelSize: 5
                        font.bold: true

                        color: Style.currentTheme.textEnabled

                        horizontalAlignment: TextInput.AlignHCenter
                        verticalAlignment: TextInput.AlignVCenter

                        opacity: _stagesCombo.moduleOn ? 1.0 : 0.5
                    }
                }

                ComboBox
                {
                    id: _comboBox

                    property bool deviceUpdatingValues: false

                    model: ["4", "6", "8"]

                    opacity: _stagesCombo.moduleOn ? 1.0 : 0.5

                    height: parent.height / 3
                    width: parent.width * 0.9

                    anchors.horizontalCenter: parent.horizontalCenter

                    currentIndex: _stagesCombo.ctrlValInstance.displayValue/2

                    onActivated:
                    {
                        if(!_comboBox.deviceUpdatingValues)
                            _stagesCombo.ctrlValInstance.displayValue = currentIndex * 2;
                    }


                    Connections{
                        target: UiCore.currentDevice

                        function onDeviceUpdatingValues()
                        {
                            _comboBox.deviceUpdatingValues = true;
                            _comboBox.currentIndex =  _stagesCombo.ctrlValInstance.displayValue / 2;
                            _comboBox.deviceUpdatingValues = false;
                        }
                    }
                }
            }
        }

        ParameterDial{
            width: main.dialWidth
            height: main.dialHeight

            controlValue: module.hpf
        }

        Rectangle{
            width: 1
            height: parent.height
        }

        ParameterSwitch{
            width: main.dialWidth
            height: main.dialHeight

            anchors.verticalCenter: parent.verticalCenter

            invertedValue: 1

            textLeft: "PRE"
            textRight: "POST"

            ctrlValInstance: module.position

            moduleOn: module.moduleEnabled
        }
    }
}
