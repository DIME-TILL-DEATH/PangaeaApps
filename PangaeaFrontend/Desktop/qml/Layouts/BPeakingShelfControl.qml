import QtQuick 2.15
import QtQuick.Controls as Controls

import StyleSettings

import Elements

import CppObjects
import PangaeaBackend

Rectangle{
    id: bandInfoRect

    z:-1

    color: "transparent"

    border.width: 2
    border.color: (main.on) ? Style.currentTheme.borderOn : Style.currentTheme.borderOff

    property EqBand eqBand: module.EqBands[0];
    property bool bandOn: true
    property var enabledController: module
    property var shelfType

    Row{
        width: parent.width * 0.9
        height: parent.height * 0.9
        anchors.centerIn: parent

        ParameterDial{
            id: dialFreq
            width: parent.width/4
            height: parent.height*0.9
            anchors.verticalCenter: parent.verticalCenter

            property EqParametric module: main.module

            controlValue: module.EqBands[currentBandIndex].Fc
        }
        ParameterDial{
            id: dialGain
            width: parent.width/4
            height: parent.height*0.9
            anchors.verticalCenter: parent.verticalCenter

            property EqParametric module: main.module

            controlValue: module.EqBands[currentBandIndex].gain
        }
        ParameterDial{
            id: dialQ
            width: parent.width/4
            height: parent.height*0.9
            anchors.verticalCenter: parent.verticalCenter

            floatDigits: 2

            property EqParametric module: main.module

            step: 0.1
            controlValue: module.EqBands[currentBandIndex].Q
        }

        Column
        {
            width: parent.width/4
            height: parent.height*0.9

            spacing: parent.height/10

            opacity: main.module.moduleEnabled ? 1 : 0.5

            Item
            {
                width: parent.width
                height: parent.height/100*20
                MText
                {
                    anchors.fill: parent
                    text: qsTr("Band type")

                    font.pixelSize: parent.height/1.2
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignTop
                }
            }

            Switch2
            {
                id:    mSwitch

                width: parent.width
                height: parent.height * 0.3

                value: eqBand.filterType === FilterType.PEAKING ? 0 : 1

                onChValue: val => {
                               eqBand.filterType = (val === 0 ? FilterType.PEAKING : shelfType)
                           }
            }

            Row
            {
                width: parent.width
                height: parent.height/100*20
                CheckText
                {
                    width: parent.width/2
                    height: parent.height
                    text: "PEAK"
                    check: mSwitch.value===0
                    onClicked: eqBand.filterType = FilterType.PEAKING

                }

                CheckText
                {
                    width: parent.width/2
                    height: parent.height
                    text: "SHELF"
                    check: mSwitch.value==1
                    onClicked: eqBand.filterType = shelfType
                }
            }

        }
    }
}
