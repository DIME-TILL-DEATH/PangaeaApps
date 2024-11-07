import QtQuick 2.15
import QtQuick.Controls 2.15

import StyleSettings 1.0

import CppObjects
import CppEnums

Rectangle
{
    id: main

    width:  parent.width*1/6
    height: parent.height

    property string text
    property bool deviceUpdatingValues: false

    signal openPresetsList()

    color: "transparent"
    border.width: 1
    border.color: Style.currentTheme.colorBorderOn
    radius: Style.baseRadius

    Column
    {
        anchors.fill:  parent
        Rectangle{
            id: _mapBtn

            width: parent.width
            height: parent.height*2.5/10

            color: _mapBtnMa.pressed ? Style.colorItemHighlight : Style.colorFon

            border.width: 1
            border.color: Style.currentTheme.colorBorderOn
            radius: Style.baseRadius

            MText{
                text: qsTr("MAP")

                color:  Style.colorText

                anchors.centerIn: parent

                font.bold: true
                font.pixelSize: main.width/5
            }

            MouseArea{
                id: _mapBtnMa

                anchors.fill: parent
                onClicked: openPresetsList()
            }
        }

        Tumbler
        {
            id: _tumbler

            width:  parent.width
            height: parent.height*6/10

            model: 4//DeviceProperties.banksList
            // currentIndex: CurrentDevice.bank

            visibleItemCount: 1

            wrap: false

            delegate: MText{
                text: modelData
                opacity: 0.1 + Math.max(0, 1 - Math.abs(Tumbler.displacement)) * 0.6

                color: Style.colorText

                font.pixelSize: _tumbler.height*0.75
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment:   Text.AlignVCenter
                font.bold: true
                font.family: "Arial Black"
            }

            onCurrentIndexChanged:
            {
                if(!main.deviceUpdatingValues)
                {
                    timer.restart();
                }
            }
        }

        MText
        {
            width:  parent.width
            height: parent.height*1/10

            color:  Style.colorText

            horizontalAlignment: Text.AlignHCenter

            font.bold: true
            font.pixelSize: main.width/5

            text: qsTr("BANK")
        }
    }

    // set only last value
    Timer
    {
        id: timer
        interval: 500
        repeat: false
        onTriggered:
        {
            UiCore.sgQmlRequestChangePreset(_tumbler.currentIndex, UiCore.currentDevice.preset);
        }
    }

    Connections{
        target: UiCore.currentDevice

        function onDeviceUpdatingValues()
        {
            main.deviceUpdatingValues = true;
            _tumbler.currentIndex = UiCore.currentDevice.bank;
            main.deviceUpdatingValues = false;
        }
    }
}
