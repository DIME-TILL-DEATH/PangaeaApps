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
    }
}
