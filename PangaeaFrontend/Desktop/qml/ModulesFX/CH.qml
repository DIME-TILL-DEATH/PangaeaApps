import QtQuick 2.15
import QtQuick.Controls 2.15

import Elements 1.0

import CppObjects 1.0
import PangaeaBackend

BaseModule{
    id: main

    property Chorus module: _moduleLoader.selectedModuleInstance

    contentItem: Row
    {
        width: main.dialWidth * 7
        height: main.dialHeight

        ParameterDial{
            width: main.dialWidth
            height: main.dialHeight

            controlValue: module.mix
        }


        ParameterDial{
            width: main.dialWidth
            height: main.dialHeight

            name: (module.type.displayValue === 4) ? "Detune" : module.rate.name

            visible: module.type.displayValue !== 5

            controlValue: module.rate
        }

        ParameterDial{
            width: main.dialWidth
            height: main.dialHeight

            visible: (module.type.displayValue !== 4) & (module.type.displayValue !== 5)

            controlValue: module.width
        }

        ParameterDial{
            width: main.dialWidth
            height: main.dialHeight

            visible: (module.type.displayValue !== 4)

            controlValue: module.delay
        }

        ParameterComboBox{
            id: _comboType

            width: main.dialWidth * 2
            height: main.dialHeight

            anchors.verticalCenter: parent.verticalCenter

            ctrlValInstance: module.type

            moduleOn: module.moduleEnabled

            model: ["Chorus", "Chorus S", "Chorus x 3", "Chorus x3 S", "Detune", "MidSide Dub"]
        }

        ParameterDial{
            width: main.dialWidth
            height: main.dialHeight

            controlValue: module.hpf
        }
    }
}
