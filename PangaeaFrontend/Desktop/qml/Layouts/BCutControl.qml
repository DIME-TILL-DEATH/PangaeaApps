import QtQuick 2.15
import QtQuick.Controls as Controls

import StyleSettings

import Elements

import CppObjects
import PangaeaBackend

Rectangle{
    id: bandInfoRect

    anchors.fill: parent

    z:-1

    color: "transparent"

    border.width: 2
    border.color: (main.on) ? "white" : "darkgrey"

    property EqBand eqBand: module.EqBands[0];
    property bool bandOn: true
    property var enabledController: module

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

        Column
        {
            width: parent.width/4
            height: parent.height*0.9

            spacing: parent.height/10

            Item
            {
                width: parent.width
                height: parent.height/100*20
                MText
                {
                    anchors.fill: parent
                    text: qsTr("CUT")

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

                value: eqBand.enabled

                onChValue: val => {
                               eqBand.enabled = !eqBand.enabled
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
                    text: "OFF"
                    check: mSwitch.value===0
                    onClicked: eqBand.enabled = 0

                }

                CheckText
                {
                    width: parent.width/2
                    height: parent.height
                    text: "ON"
                    check: mSwitch.value==1
                    onClicked: eqBand.enabled = 1
                }
            }

        }
    }
}
