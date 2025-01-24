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

            model: UiCore.currentDevice.maxPresetCount
            // currentIndex: DeviceProperties.bank*4 + DeviceProperties.preset


            visibleItemCount: 1

            wrap: false

            property color currentTextColor: Style.colorText
            property int opacityBlink: 1.0

            function setTumblerColor(){
                if((currentIndex === UiCore.currentDevice.clnPresetMap) & (UiCore.currentDevice.la3Channel === 0))
                {
                    currentTextColor = "lightgreen";
                    opacityBlink = 1.0
                }
                else if((currentIndex === UiCore.currentDevice.drvPresetMap) & (UiCore.currentDevice.la3Channel === 1))
                {
                    currentTextColor = "red";
                    opacityBlink = 1.0
                }
                else
                {
                    currentTextColor = Style.colorText;
                    opacityBlink = 0.5
                }
            }

            delegate: MText
            {
                text: modelData
                opacity: 0.1 + Math.max(0, 1 - Math.abs(Tumbler.displacement)) * 0.6
                color: _tumbler.currentTextColor
                font.pixelSize: _tumbler.height*0.6
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment:   Text.AlignVCenter
                font.bold: true
                font.family: "Arial Black"


                NumberAnimation on opacity{
                    loops: Animation.Infinite
                    from: 1
                    to: _tumbler.opacityBlink
                    duration: 500
                }
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
            _tumbler.setTumblerColor();
        }
    }
}
