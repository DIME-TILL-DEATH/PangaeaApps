import QtQuick 2.15

import Elements 1.0

import CppObjects 1.0
import PangaeaBackend

BaseModule{
    id: main

    property Compressor module: _moduleLoader.selectedModuleInstance

    contentItem: Row
    {
        anchors.fill: parent


        ParameterDial{
            width: main.dialWidth
            height: main.dialHeight

            controlValue: module.threshold
        }


        ParameterDial{
            width: main.dialWidth
            height: main.dialHeight

            controlValue: module.ratio
        }

        ParameterDial{
            width: main.dialWidth
            height: main.dialHeight

            controlValue: module.volume
        }

        ParameterDial{
            width: main.dialWidth
            height: main.dialHeight

            controlValue: module.attack
        }

        ParameterDial{
            width: main.dialWidth
            height: main.dialHeight

            controlValue: module.knee
        }
    }
}
