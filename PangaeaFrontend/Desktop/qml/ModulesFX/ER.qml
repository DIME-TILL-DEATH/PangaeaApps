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
        id: _column
        anchors.fill: parent


        ParameterDial{
            width: main.dialWidth
            height: main.dialHeight

            controlValue: module.reflectionsVolume
        }

        ParameterComboBox{
            width: main.dialWidth * 2
            height: main.dialHeight

            anchors.verticalCenter: parent.verticalCenter

            ctrlValInstance: module.reflectionsType

            moduleOn: module.moduleEnabled

            model: ["Short M",
                    "Short S",
                    "Medium M",
                    "Medium S",
                    "Long M",
                    "Long S"]
        }
    }
}
