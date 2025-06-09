import QtQuick 2.15

import Elements 1.0

import CppObjects 1.0
import PangaeaBackend

BaseModule{
    id: main

    property Preamp module: _moduleLoader.selectedModuleInstance

    contentItem: Row
    {
        anchors.fill: parent

        ParameterDial{
            width: main.dialWidth
            height: main.dialHeight

            controlValue: module.gain
        }


        Rectangle{
            width: 1
            height: parent.height
        }

        ParameterDial{
            width: main.dialWidth
            height: main.dialHeight

            controlValue: module.low
        }

        ParameterDial{
            width: main.dialWidth
            height: main.dialHeight

            controlValue: module.mid
        }

        ParameterDial{
            width: main.dialWidth
            height: main.dialHeight

            controlValue: module.high
        }

        Rectangle{
            width: 1
            height: parent.height
        }

        ParameterDial{
            width: main.dialWidth
            height: main.dialHeight

            controlValue: module.volume
        }
    }
}


