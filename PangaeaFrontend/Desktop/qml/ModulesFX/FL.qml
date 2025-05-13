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

            controlValue: module.lfo
        }

        ParameterDial{
            width: main.dialWidth
            height: main.dialHeight

            floatDigits: 3
            step: 0.05

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
    }
}
