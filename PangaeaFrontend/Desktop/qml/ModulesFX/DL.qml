import QtQuick 2.15
import QtQuick.Controls 2.15

import Elements 1.0

import CppObjects 1.0
import PangaeaBackend

BaseModule{
    id: main

    property Delay module: _moduleLoader.selectedModuleInstance

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

            // floatDigits: 3
            // step: 0.001

            controlValue: module.time
        }

        ParameterComboBox{
            width: main.dialWidth * 2
            height: main.dialHeight

            anchors.verticalCenter: parent.verticalCenter

            ctrlValInstance: module.tap

            moduleOn: module.moduleEnabled

            model: ["1/1 (4th notes)", "1/1.5 (8th dotted)", "1/2 (8th notes)", "1/3 (16th dotted)", "1/4 (16th notes)"]
        }

        ParameterDial{
            width: main.dialWidth
            height: main.dialHeight

            // floatDigits: 1
            // step: 0.5

            controlValue: module.feedback
        }

        ParameterDial{
            width: main.dialWidth
            height: main.dialHeight

            floatDigits: 1
            step: 0.5

            controlValue: module.lpf
        }

        ParameterDial{
            width: main.dialWidth
            height: main.dialHeight

            controlValue: module.hpf
        }

        ParameterDial{
            width: main.dialWidth
            height: main.dialHeight

            controlValue: module.panFirst
        }

        ParameterDial{
            width: main.dialWidth
            height: main.dialHeight

            controlValue: module.mixSecond
        }

        ParameterDial{
            width: main.dialWidth
            height: main.dialHeight

            controlValue: module.panSecond
        }

        ParameterDial{
            width: main.dialWidth
            height: main.dialHeight

            controlValue: module.offset
        }

        ParameterDial{
            width: main.dialWidth
            height: main.dialHeight

            controlValue: module.modulation
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

            ctrlValInstance: module.direction

            moduleOn: module.moduleEnabled

            model: ["Forward", "Backward"]
        }
    }
}
