import QtQuick 2.15

import Elements 1.0
import CppObjects

Item
{
    id: main

    property bool on: true
    property int paramType: DeviceParameter.EQ_ON
    Row
    {
        anchors.fill: parent

        Eq
        {
            on: main.on
            dialFreq: "20 Hz 220"
            nomBand: 1
        }

        Eq
        {
            on: main.on
            dialFreq: "260 Hz 460"
            nomBand: 2
        }

        Eq
        {
            on: main.on
            dialFreq: "600 Hz 1k"
            nomBand: 3
        }

        Eq
        {
            on: main.on
            dialFreq: "1k Hz 3k"
            nomBand: 4
        }

        Eq
        {
            on: main.on
            dialFreq: "1k Hz 11k"
            nomBand: 5
        }
    }
}
