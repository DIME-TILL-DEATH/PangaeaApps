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

            model: DeviceProperties.presetsList
            currentIndex: DeviceProperties.bank*4 + DeviceProperties.preset


            visibleItemCount: 1

            wrap: false

            property color currentTextColor: Style.colorText
            property int opacityBlink: 1.0

            function setTumblerColor(){
                if(currentIndex === DeviceProperties.la3CleanPreset & DeviceProperties.la3Channel === 0)
                {
                    currentTextColor = "lightgreen";
                    opacityBlink = 1.0
                }
                else if(currentIndex === DeviceProperties.la3DrivePreset & DeviceProperties.la3Channel === 1)
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
                font.pixelSize: DeviceProperties.isLa3Mode ? _tumbler.height*0.6 : _tumbler.height*0.75
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
                timer.restart();

                setTumblerColor();
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
            if(DeviceProperties.isLa3Mode)
            {
                DeviceProperties.changePreset(Math.floor(_tumbler.currentIndex/4), _tumbler.currentIndex%4);
            }
            else
            {
                DeviceProperties.preset = _tumbler.currentIndex;
            }
        }
    }

    Connections{
        target: DeviceProperties

        function onLa3ChannelChanged(){
            _tumbler.setTumblerColor();
        }
    }
}
