import QtQuick 2.15
import QtQuick.Controls 2.12

import StyleSettings 1.0
import Elements 1.0
import Tutorials

import CppObjects

BaseModule
{
    id: main

    property LowPassFilter module

    moduleDescription: qsTr("Low-pass filter")

    showDescription: false

    contentItem: Row
    {
        anchors.fill: parent
        spacing: 0

        Column
        {
            height: parent.height
            width: parent.width
            CustomSlider
            {
                // units: "Hz"
                // dispMax: 20000
                // dispMin: 1000
                // valueMax: 0
                // valueMin: 195
                fontSize: 8 * Style.dip

                height: parent.height
                width: parent.width

                ctrlValInstance: module.lpf

                bottomLineEnabled: false
                moduleOn: on
            }
        }
    }

    tutorialItem: SimpleTutorialMessage{
        buttons: Dialog.Ok

        headerText: qsTr("Low pass filter")

        text: qsTr("Low-pass filter passes through frequencies below the cutoff frequency and attenuates everything above that point");
    }
}
