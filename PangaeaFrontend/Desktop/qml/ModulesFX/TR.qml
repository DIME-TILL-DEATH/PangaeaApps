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
        id: _column
        anchors.fill: parent


        ParameterDial{
            width: main.dialWidth
            height: main.dialHeight

            controlValue: module.depth
        }


        ParameterDial{
            width: main.dialWidth
            height: main.dialHeight

            controlValue: module.rate
        }

        ParameterDial{
            width: main.dialWidth
            height: main.dialHeight

            controlValue: module.form
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
            width: main.dialWidth * 2
            height: main.dialHeight

            anchors.verticalCenter: parent.verticalCenter

            ctrlValInstance: module.tap

            moduleOn: module.moduleEnabled

            model: ["1/1 (4th notes)", "1/1.5 (8th dotted)", "1/2 (8th notes)", "1/3 (16th dotted)", "1/4 (16th notes)"]
        }
    }
}
