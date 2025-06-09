import QtQuick 2.15

import Elements 1.0

import CppObjects 1.0
import PangaeaBackend

BaseModule{
    id: main

    property PowerAmp module: _moduleLoader.selectedModuleInstance

    contentItem: Row
    {
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

        ParameterComboBox{
            width: main.dialWidth * 3
            height: main.dialHeight

            anchors.verticalCenter: parent.verticalCenter

            ctrlValInstance: module.ampType

            moduleOn: module.moduleEnabled

            model: ["1. Push-pull 6L6",
                "2. Push-pull EL34",
                "3. Single-ended 6L6",
                "4. Single-ended EL34",
                "5. AMT Tube cake",
                "6. California",
                "7. British M",
                "8. British L",
                /*"Flat",
                "Califonia modern",
                "California vintage",
                "PVH Presence 01",
                "PVH Presence 02",
                "PVH Presence 03",
                "PVH Presence 04"*/]
        }
    }
}
