import QtQuick 2.15
import QtQuick.Controls 2.15

import StyleSettings 1.0

import CppObjects

Rectangle
{
    id: main

    width:  parent.width*1/6
    height: parent.height

    property string text
    property bool deviceUpdatingValues: true //disable timer on startup

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

            color: _mapBtnMa.pressed ? Style.currentTheme.colorItemHighlight : Style.colorFon

            border.width: 1
            border.color: Style.currentTheme.colorBorderOn
            radius: Style.baseRadius

            MText{
                text: qsTr("MAP")

                color:  Style.colorText

                anchors.centerIn: parent

                font.bold: true
                font.pixelSize: Math.min(main.width/5, main.height/5)
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

            model: UiCore.currentDevice.maxPresetCount
            // currentIndex: DeviceProperties.isLa3Mode ? (DeviceProperties.bank*4 + DeviceProperties.preset)
            //                                          : CurrentDevice.preset

            visibleItemCount: 1

            wrap: false

            delegate: MText
            {
                text: modelData
                opacity: 0.1 + Math.max(0, 1 - Math.abs(Tumbler.displacement)) * 0.6
                color: Style.colorText
                font.pixelSize: Math.min(_tumbler.height*0.75, _tumbler.width*0.75)
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
            font.pixelSize: Math.min(main.width/5, main.height/5)

            text: qsTr("PRESET")
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
            UiCore.sgQmlRequestChangePreset(UiCore.currentDevice.bank, _tumbler.currentIndex);
        }
    }

    Connections{
        target: UiCore.currentDevice

        function onBankPresetChanged()
        {
            main.deviceUpdatingValues = true;
            _tumbler.currentIndex = UiCore.currentDevice.preset
            main.deviceUpdatingValues = false;
        }
    }
}
