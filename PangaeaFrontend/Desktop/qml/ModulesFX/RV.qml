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
        width: main.dialWidth * 12
        height: main.dialHeight


        ParameterDial{
            width: main.dialWidth
            height: main.dialHeight

            controlValue: module.mix
        }

        ParameterComboBox{
            id: _comboType

            width: main.dialWidth * 2
            height: main.dialHeight

            anchors.verticalCenter: parent.verticalCenter

            ctrlValInstance: module.type

            moduleOn: module.moduleEnabled

            model: ["Default", "Hall", "Room", "Plate", "Spring", "Gate", "Reverse"]
        }


        ParameterDial{
            width: main.dialWidth
            height: main.dialHeight

            visible: (module.type.displayValue!== 5) & (module.type.displayValue !== 6)

            controlValue: module.time
        }

        ParameterDial{
            width: main.dialWidth
            height: main.dialHeight

            visible: (module.type.displayValue !== 4)

            controlValue: module.size
        }

        ParameterDial{
            width: main.dialWidth
            height: main.dialHeight

            visible: (module.type.displayValue !== 4)

            controlValue: module.damping
        }

        ParameterDial{
            width: main.dialWidth
            height: main.dialHeight

            controlValue: module.lpf
        }

        ParameterDial{
            width: main.dialWidth
            height: main.dialHeight

            controlValue: module.hpf
        }


        ParameterDial{
            width: main.dialWidth
            height: main.dialHeight

            visible: (module.type.displayValue !== 4)
                     & (module.type.displayValue !== 5)
                     & (module.type.displayValue !== 6)

            controlValue: module.detune
        }

        ParameterDial{
            width: main.dialWidth
            height: main.dialHeight

            visible: (module.type.displayValue !== 0) & (module.type.displayValue !== 4)

            controlValue: module.diffusion
        }

        ParameterDial{
            width: main.dialWidth
            height: main.dialHeight

            controlValue: module.preDelay
        }

        ParameterSwitch{
            width: main.dialWidth
            height: main.dialHeight

            anchors.verticalCenter: parent.verticalCenter

            ctrlValInstance: module.tail

            textLeft: "ON"
            textRight: "OFF"

            moduleOn: module.moduleEnabled
        }
    }
}
