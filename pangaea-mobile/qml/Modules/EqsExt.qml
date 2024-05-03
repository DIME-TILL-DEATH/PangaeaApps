import QtQuick 2.15
import QtQuick.Controls 2.12

import StyleSettings 1.0
import Elements 1.0

import CppObjects

Item
{
    id: main

    property string name:     "PA"
    property bool on: true

    property int eq1: eq1.yDisp
    property int eq2: eq2.yDisp
    property int eq3: eq3.yDisp
    property int eq4: eq4.yDisp
    property int eq5: eq5.yDisp

    property int q1: _q1Control.yDisp
    property int q2: _q2Control.yDisp
    property int q3: _q3Control.yDisp
    property int q4: _q4Control.yDisp
    property int q5: _q5Control.yDisp

    property bool extVisible: false
    signal hide


    Rectangle
    {
        color: Style.colorFon
        anchors.fill: parent
        border.width: 3
        border.color: "black"
        radius: 5

        Column
        {
            width: parent.width/10*8
            height: parent.height/10*8
            anchors.centerIn: parent

            Item
            {
                width: parent.width
                height: parent.height*1/12
                CustomizerSlider
                {
                    id: _q1Control

                    name: "Q-Factor"
                    anchors.fill: parent
                    dispMin: 1
                    dispMax: 200
                    delitel: 10

                    valueMin: -100
                    valueMax: 100

                    paramType: DeviceParameter.EQ_Q1
                    visible: (eqsArray.nomActiv==0)
                }
                CustomizerSlider
                {
                    id: _q2Control

                    name: "Q-Factor"
                    anchors.fill: parent
                    dispMin: 1
                    dispMax: 200
                    delitel: 10

                    valueMin: -100
                    valueMax: 100

                    paramType: DeviceParameter.EQ_Q2

                    visible: (eqsArray.nomActiv==1)
                }
                CustomizerSlider
                {
                    id: _q3Control

                    name: "Q-Factor"
                    anchors.fill: parent
                    dispMin: 1
                    dispMax: 200
                    delitel: 10

                    valueMin: -100
                    valueMax: 100

                    paramType: DeviceParameter.EQ_Q3

                    visible: (eqsArray.nomActiv==2)
                }
                CustomizerSlider
                {
                    id: _q4Control

                    name: "Q-Factor"
                    anchors.fill: parent
                    dispMin: 1
                    dispMax: 200
                    delitel: 10

                    valueMin: -100
                    valueMax: 100

                    paramType: DeviceParameter.EQ_Q4

                    visible: (eqsArray.nomActiv==3)
                }
                CustomizerSlider
                {
                    id: _q5Control

                    name: "Q-Factor"
                    anchors.fill: parent
                    dispMin: 1
                    dispMax: 200
                    delitel: 10

                    valueMin: -100
                    valueMax: 100

                    paramType: DeviceParameter.EQ_Q5

                    visible: (eqsArray.nomActiv==4)
                }
            }
            Item
            {
                width: parent.width
                height: parent.height*1/12
            }

            Item
            {
                anchors.left: parent.left
                anchors.leftMargin: parent.width/20
                width: parent.width
                height: parent.height/2
                Rectangle{
                    anchors.right: parent.right
                    anchors.rightMargin: -parent.width*2/100
                    width: parent.width*115/100
                    height: parent.height
                    radius: parent.height/20
                    color: "#20201F"
                }
                Rectangle{
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.top: parent.top
                    anchors.topMargin: parent.height*8/100
                    width: parent.width*90/100
                    height: 1
                    color: "grey"
                    MText {
                        text: "+15"
                        color: "white"
                        anchors.verticalCenter: parent.top
                        anchors.right: parent.left
                        anchors.rightMargin: parent.width*3/100
                    }
                }
                Rectangle{
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.top: parent.top
                    anchors.topMargin: parent.height/100*34
                    width: parent.width*90/100
                    height: 1
                    color: "grey"
                    MText {
                        text: "+6"
                        color: "white"
                        anchors.verticalCenter: parent.top
                        anchors.right: parent.left
                        anchors.rightMargin: parent.width*3/100
                    }
                }
                Rectangle{
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.top: parent.top
                    anchors.topMargin: parent.height*50/100
                    width: parent.width*90/100
                    height: 1
                    color: "grey"
                    Text {
                        text: "0dB"
                        font.pixelSize: parent.height/100
                        color: "white"
                        anchors.verticalCenter: parent.top
                        anchors.right: parent.left
                        anchors.rightMargin: parent.width*3/100
                    }
                }
                Rectangle{
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: parent.height*58/1000
                    width: parent.width*90/100
                    height: 1
                    color: "grey"
                    MText {
                        text: "-15"
                        color: "white"
                        anchors.verticalCenter: parent.top
                        anchors.right: parent.left
                        anchors.rightMargin: parent.width*3/100
                    }
                }
                Rectangle{
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.top: parent.top
                    anchors.topMargin: parent.height/100*68
                    width: parent.width*90/100
                    height: 1
                    color: "grey"
                    MText {
                        text: "-6"
                        color: "white"
                        anchors.verticalCenter: parent.top
                        anchors.right: parent.left
                        anchors.rightMargin: parent.width*3/100
                    }
                }

                Item
                {
                    id: eqsArray

                    property int nomActiv: 0
                    property int maxWidth: parent.width/6

                    width: parent.width
                    height: parent.height

                    EQExtBar
                    {
                        id: eq1

                        x: eqsArray.maxWidth/2

                        height: parent.height
                        width: eqsArray.maxWidth * 0.9

                        paramType: DeviceParameter.EQ_VOLUME1
                        qSetting: main.q1
                        qControlActive: 0
                    }

                    EQExtBar
                    {
                        id: eq2

                        x: eqsArray.maxWidth + eqsArray.maxWidth/2

                        height: parent.height
                        width: eqsArray.maxWidth * 0.9

                        paramType: DeviceParameter.EQ_VOLUME2
                        qSetting: main.q2
                        qControlActive: 1
                    }

                    EQExtBar
                    {
                        id: eq3

                        x: eqsArray.maxWidth*2 + eqsArray.maxWidth/2

                        height: parent.height
                        width: eqsArray.maxWidth * 0.9

                        paramType: DeviceParameter.EQ_VOLUME3
                        qSetting: main.q3
                        qControlActive: 2
                    }

                    EQExtBar
                    {
                        id: eq4

                        x: eqsArray.maxWidth*3 + eqsArray.maxWidth/2

                        height: parent.height
                        width: eqsArray.maxWidth * 0.9

                        paramType: DeviceParameter.EQ_VOLUME4
                        qSetting: main.q4
                        qControlActive: 3
                    }

                    EQExtBar
                    {
                        id: eq5

                        x: eqsArray.maxWidth*4 + eqsArray.maxWidth/2

                        height: parent.height
                        width: eqsArray.maxWidth * 0.95

                        paramType: DeviceParameter.EQ_VOLUME5
                        qSetting: main.q5
                        qControlActive: 4
                    }
                }
            }

            Item
            {
                width: parent.width
                height: parent.height*1/12
            }

            Item
            {
                width: parent.width
                height: parent.height*1/12
                CustomizerSlider
                {
                    name: "Central Frequency"
                    anchors.fill: parent
                    paramType: DeviceParameter.EQ_FREQ1
                    dispMin: 20
                    dispMax: 220

                    valueMin: -100
                    valueMax: 100
                    units: "Hz"
                    visible: (eqsArray.nomActiv==0)
                }
                CustomizerSlider
                {
                    name: "Central Frequency"
                    anchors.fill: parent
                    paramType: DeviceParameter.EQ_FREQ2
                    dispMin: 260
                    dispMax: 460

                    valueMin: -100
                    valueMax: 100
                    units: "Hz"
                    visible: (eqsArray.nomActiv==1)
                }
                CustomizerSlider
                {
                    name: "Central Frequency"
                    anchors.fill: parent
                    paramType: DeviceParameter.EQ_FREQ3
                    dispMin: 600
                    dispMax: 1000

                    valueMin: -100
                    valueMax: 100
                    units: "Hz"
                    visible: (eqsArray.nomActiv==2)
                }
                CustomizerSlider
                {
                    name: "Central Frequency"
                    anchors.fill: parent
                    paramType: DeviceParameter.EQ_FREQ4
                    dispMin: 1000
                    dispMax: 3000

                    valueMin: -100
                    valueMax: 100
                    units: "Hz"
                    visible: (eqsArray.nomActiv==3)
                }
                CustomizerSlider
                {
                    name: "Central Frequency"
                    anchors.fill: parent
                    paramType: DeviceParameter.EQ_FREQ5
                    dispMin: 1000
                    dispMax: 11000

                    valueMin: -100
                    valueMax: 100
                    units: "Hz"
                    visible: (eqsArray.nomActiv==4)
                }
            }

            Item
            {
                width: parent.width
                height: parent.height*1/12
            }

            Button
            {
                id: btn

                width: parent.width
                height: parent.height*1/6

                text: qsTr("HIDE")

                onClicked:
                {
                    hide();
                }
            }
        }
    }

    MouseArea
    {   z: -1
        anchors.fill: parent
    }
}
