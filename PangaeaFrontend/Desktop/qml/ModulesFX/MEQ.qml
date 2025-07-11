import QtQuick 2.15

import Elements 1.0

import CppObjects 1.0
import PangaeaBackend

BaseModule{
    id: main

    property MasterEq module: UiCore.currentDevice.masterEq

    contentItem: Row
    {
        anchors.fill: parent


        ParameterDial{
            width: main.dialWidth
            height: main.dialHeight

            step: 0.5

            controlValue: module.lowGain

            floatDigits: 1
        }


        ParameterDial{
            width: main.dialWidth
            height: main.dialHeight

            controlValue: module.midGain

            step: 0.5

            floatDigits: 1
        }

        ParameterDial{
            width: main.dialWidth
            height: main.dialHeight

            controlValue: module.midFrequency
        }

        ParameterDial{
            width: main.dialWidth
            height: main.dialHeight

            step: 0.5

            controlValue: module.highGain

            floatDigits: 1
        }
    }
}
