import QtQuick 2.15
import QtQuick.Controls 2.15

import Elements 1.0

import CppObjects 1.0
import PangaeaBackend

BaseModule{
    id: main

    property EarlyReflections module: _moduleLoader.selectedModuleInstance

    contentItem: Row
    {
        anchors.fill: parent


        ParameterDial{
            width: main.dialWidth
            height: main.dialHeight

            controlValue: module.reflectionsVolume
        }

        ParameterDial{
            width: main.dialWidth
            height: main.dialHeight

            controlValue: module.reflectionsType
        }
    }
}
