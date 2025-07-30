import QtQuick 2.15
import QtQuick.Controls 2.15

import Elements 1.0

import CppObjects 1.0
import PangaeaBackend

BaseModule{
    id: main

    property Flanger module: _moduleLoader.selectedModuleInstance

    contentItem: Row
    {
        anchors.fill: parent


        ParameterDial{
            width: main.dialWidth
            height: main.dialHeight

            controlValue: module.mix
        }


        ParameterComboBox{
            width: main.dialWidth*2
            height: main.dialHeight

            anchors.verticalCenter: parent.verticalCenter

            ctrlValInstance: module.lfo

            moduleOn: module.moduleEnabled

            model: ["Triangle", "Sinus"]
        }

        ParameterDial{
            width: main.dialWidth
            height: main.dialHeight

            controlValue: module.rate
        }

        ParameterDial{
            width: main.dialWidth
            height: main.dialHeight

            controlValue: module.width
        }

        ParameterDial{
            width: main.dialWidth
            height: main.dialHeight

            controlValue: module.delay
        }

        ParameterDial{
            width: main.dialWidth
            height: main.dialHeight

            controlValue: module.feedback
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

            invertedValue: 1

            textLeft: "PRE"
            textRight: "POST"

            anchors.verticalCenter: parent.verticalCenter

            ctrlValInstance: module.position

            moduleOn: module.moduleEnabled
        }
    }
}
