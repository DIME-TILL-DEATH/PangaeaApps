import QtQuick 2.15
import QtQuick.Controls 2.15

import Elements 1.0

import CppObjects 1.0
import PangaeaBackend

BaseModule{
    id: main

    property Phaser module: _moduleLoader.selectedModuleInstance

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

            controlValue: module.rate
        }

        ParameterDial{
            width: main.dialWidth
            height: main.dialHeight

            controlValue: module.center
        }

        ParameterDial{
            width: main.dialWidth
            height: main.dialHeight

            controlValue: module.width
        }

        ParameterDial{
            width: main.dialWidth
            height: main.dialHeight

            controlValue: module.feedback
        }

        ParameterComboBox{
            width: main.dialWidth
            height: main.dialHeight

            anchors.verticalCenter: parent.verticalCenter

            ctrlValInstance: module.stages

            moduleOn: module.moduleEnabled

            model: ["4", "6", "8"]
        }

        ParameterDial{
            width: main.dialWidth
            height: main.dialHeight

            controlValue: module.hpf
        }
    }
}
