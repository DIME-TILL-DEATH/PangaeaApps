import QtQuick 2.15
import QtQuick.Controls 2.15

import Elements 1.0

import CppObjects 1.0
import PangaeaBackend

BaseModule{
    id: main

    property Chorus module: _moduleLoader.selectedModuleInstance

    contentItem: Row
    {
        id: _column
        anchors.fill: parent


        ParameterDial{
            width: main.dialWidth
            height: main.dialHeight

            controlValue: module.mix
        }


        ParameterDial{
            width: main.dialWidth
            height: main.dialHeight

            floatDigits: 3
            step: 0.001

            controlValue: module.rate
        }

        ParameterDial{
            width: main.dialWidth
            height: main.dialHeight

            floatDigits: 1
            step: 0.5

            controlValue: module.width
        }

        ParameterDial{
            width: main.dialWidth
            height: main.dialHeight

            floatDigits: 1
            step: 0.5

            controlValue: module.delay
        }

        ParameterComboBox{
            width: main.dialWidth * 2
            height: main.dialHeight

            anchors.verticalCenter: parent.verticalCenter

            ctrlValInstance: module.type

            moduleOn: module.moduleEnabled

            model: ["Chorus", "Chorus S", "Chorus x 3", "Chorus x3 S"]
        }

        ParameterDial{
            width: main.dialWidth
            height: main.dialHeight

            controlValue: module.hpf
        }
    }
}
