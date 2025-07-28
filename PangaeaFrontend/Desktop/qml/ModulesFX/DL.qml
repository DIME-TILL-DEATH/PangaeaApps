import QtQuick 2.15
import QtQuick.Controls 2.15

import Elements 1.0

import CppObjects 1.0
import PangaeaBackend

BaseModule{
    id: main

    property Delay module: _moduleLoader.selectedModuleInstance

    contentItem: Flickable{
        anchors.fill: parent

        contentWidth: _row.width
        contentHeight: _row.height

        flickableDirection: Flickable.HorizontalFlick
        boundsBehavior: Flickable.StopAtBounds
    Row{
        id: _row
        // anchors.fill: parent
        width: main.dialWidth * 14.5
        height: main.dialHeight

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

        ParameterComboBox{
            width: main.dialWidth
            height: main.dialHeight

            anchors.verticalCenter: parent.verticalCenter

            ctrlValInstance: module.tap

            moduleOn: module.moduleEnabled

            model: ["1/1", "1/1.5", "1/2", "1/3", "1/4", "2/1"]
        }

        ParameterDial{
            width: main.dialWidth
            height: main.dialHeight

            controlValue: module.feedback
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

            controlValue: module.panFirst
        }

        ParameterDial{
            width: main.dialWidth
            height: main.dialHeight

            controlValue: module.mixSecond
        }

        ParameterDial{
            width: main.dialWidth
            height: main.dialHeight

            controlValue: module.panSecond
        }

        ParameterDial{
            width: main.dialWidth
            height: main.dialHeight

            controlValue: module.offset
        }

        ParameterDial{
            width: main.dialWidth
            height: main.dialHeight

            controlValue: module.modulation
        }

        ParameterDial{
            width: main.dialWidth
            height: main.dialHeight

            controlValue: module.rate
        }

        ParameterComboBox{
            width: main.dialWidth * 1.5
            height: main.dialHeight

            anchors.verticalCenter: parent.verticalCenter

            ctrlValInstance: module.direction

            moduleOn: module.moduleEnabled

            model: ["Forward", "Reverse"]
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
}
