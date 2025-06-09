import QtQuick 2.15
import QtQuick.Controls 2.15

import Elements 1.0

import StyleSettings 1.0

import CppObjects 1.0
import PangaeaBackend

BaseModule{
    id: main

    property DualCabSim module: _moduleLoader.selectedModuleInstance

    contentItem: Row
    {
        anchors.fill: parent

        Column{

            width: main.dialWidth
            height: main.dialHeight

            opacity: module.moduleEnabled ? 1.0 : 0.5

            Item{
                id: _Text


                height: parent.height/3
                width: parent.width

                Text{
                    id: textValue
                    anchors.fill: parent

                    text: "Cab num"
                    font.pixelSize: 5 * Style.dip
                    font.bold: true

                    color: Style.textEnabled

                    horizontalAlignment: TextInput.AlignHCenter
                    verticalAlignment: TextInput.AlignVCenter
                }
            }

            ComboBox
            {
                id: _chooseCabCombo

                height: parent.height / 3
                width: parent.width * 0.75

                model: ["1", "2"]

                anchors.horizontalCenter: parent.horizontalCenter

                currentIndex: 0
            }
        }


        Rectangle{
            width: 1
            height: parent.height
        }

        ParameterDial{
            width: main.dialWidth
            height: main.dialHeight

            controlValue: (_chooseCabCombo.currentIndex === 0) ? module.firstCabVolume : module.secondCabVolume
        }


        ParameterDial{
            width: main.dialWidth
            height: main.dialHeight

            controlValue: (_chooseCabCombo.currentIndex === 0) ? module.firstCabPan : module.secondCabPan
        }

        ParameterDial{
            width: main.dialWidth
            height: main.dialHeight

            controlValue: (_chooseCabCombo.currentIndex === 0) ? module.firstCabDelay : module.secondCabDelay
        }

        Rectangle{
            width: 1
            height: parent.height
        }
    }
}
