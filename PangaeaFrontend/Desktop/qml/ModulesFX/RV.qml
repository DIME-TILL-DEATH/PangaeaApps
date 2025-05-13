import QtQuick 2.15
import QtQuick.Controls 2.15

import Elements 1.0

import CppObjects 1.0
import PangaeaBackend

BaseModule{
    id: main

    property Reverb module: _moduleLoader.selectedModuleInstance

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

            controlValue: module.time
        }

        ParameterDial{
            width: main.dialWidth
            height: main.dialHeight

            controlValue: module.size
        }

        ParameterDial{
            width: main.dialWidth
            height: main.dialHeight

            controlValue: module.damping
        }

        ParameterDial{
            width: main.dialWidth
            height: main.dialHeight

            controlValue: module.hpf
        }

        ParameterDial{
            width: main.dialWidth
            height: main.dialHeight

            controlValue: module.lpf
        }

        ParameterDial{
            width: main.dialWidth
            height: main.dialHeight

            controlValue: module.detune
        }
    }
}
