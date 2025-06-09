import QtQuick 2.15
import QtQuick.Controls 2.15

import Elements 1.0

import CppObjects 1.0
import PangaeaBackend

BaseModule{
    id: main

    property Tremolo module: _moduleLoader.selectedModuleInstance

    contentItem: Row
    {
        anchors.fill: parent


        ParameterDial{
            width: main.dialWidth
            height: main.dialHeight

            controlValue: module.depth

            name: "Intensity"
        }


        ParameterDial{
            width: main.dialWidth
            height: main.dialHeight

            controlValue: module.rate
        }

        ParameterComboBox{
            width: main.dialWidth * 2
            height: main.dialHeight

            anchors.verticalCenter: parent.verticalCenter

            ctrlValInstance: module.form

            moduleOn: module.moduleEnabled

            model: ["Sin", "Square", "Sawtooth"]
        }

        ParameterDial{
            width: main.dialWidth
            height: main.dialHeight

            controlValue: module.lfoMod
        }

        ParameterComboBox{
            width: main.dialWidth * 2
            height: main.dialHeight

            anchors.verticalCenter: parent.verticalCenter

            ctrlValInstance: module.ms

            moduleOn: module.moduleEnabled

            model: ["Mono", "Stereo"]
        }

        ParameterComboBox{
            width: main.dialWidth
            height: main.dialHeight

            anchors.verticalCenter: parent.verticalCenter

            ctrlValInstance: module.tap

            moduleOn: module.moduleEnabled

            model: ["1/1", "1/1.5", "1/2", "1/3", "1/4"]
        }
    }
}
