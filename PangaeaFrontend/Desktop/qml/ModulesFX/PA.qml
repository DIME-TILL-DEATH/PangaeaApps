import QtQuick 2.15

import Elements 1.0

import CppObjects 1.0
import PangaeaBackend

BaseModule{
    id: main

    property PowerAmp module: _moduleLoader.selectedModuleInstance

    contentItem: Row
    {
        id: _column
        anchors.fill: parent


        ParameterDial{
            width: main.dialWidth
            height: main.dialHeight

            controlValue: module.volume
        }

        ParameterDial{
            width: main.dialWidth
            height: main.dialHeight

            controlValue: module.presence
        }

        ParameterDial{
            width: main.dialWidth
            height: main.dialHeight

            controlValue: module.slave
        }

        TypePA
        {
            width:  main.dialWidth
            height: parent.height

            controlValue: module.ampType
        }
    }
}
