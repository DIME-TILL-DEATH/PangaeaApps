import QtQuick 2.15
import QtQuick.Controls 2.15

import StyleSettings 1.0

import CppObjects
import PangaeaBackend

Rectangle
{
    id: main

    width:  parent.width*1/6
    height: parent.height

    property string text

    // property bool isModified: false
    property bool isDeviceUpdatingValues: false

    color: "transparent"
    border.width: 1
    border.color: Style.currentTheme.colorBorderOn
    radius: Style.baseRadius

    Column
    {
        anchors.fill:  parent


        Rectangle{
            id: _setBtn

            width: parent.width
            height: parent.height*2.5/10

            enabled: main.isModified

            color: Style.colorFon

            border.width: 1
            border.color: Style.currentTheme.colorBorderOn
            radius: Style.baseRadius

            MText{
                text: qsTr("Set SW.")

                color:  Style.colorText

                anchors.centerIn: parent

                font.bold: true
                font.pixelSize: main.width/5
            }

            // MouseArea{
            //     id: _mapBtnMa


            //     anchors.fill: parent
            //     onClicked: {
            //         main.isModified = false;
            //         UiCore.currentDevice.setLa3Mappings(_tumblerCln.currentIndex, _tumblerDst.currentIndex);
            //     }
            // }
        }

        Row{
            width:  parent.width
            height: parent.height*3.75/10
            Item{
                width:  parent.width*1/4
                height: parent.height
                MText{
                    text: "CLN"
                    color: "lightgreen"
                    rotation: 90
                    font.pixelSize:  _tumblerCln.height*0.2

                    anchors.centerIn: parent
                }
            }

            Tumbler{
                id: _tumblerCln

                width:  parent.width*3/4
                height: parent.height

                model: UiCore.currentDevice.maxPresetCount
                currentIndex: UiCore.currentDevice.clnPresetMap

                visibleItemCount: 1

                wrap: false

                delegate: MText{
                    text: modelData
                    opacity: 0.1 + Math.max(0, 1 - Math.abs(Tumbler.displacement)) * 0.6

                    color: "lightgreen"
                    font.pixelSize: _tumblerDst.height*0.7

                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment:   Text.AlignVCenter
                    font.bold: true
                    font.family: "Arial Black"
                }

                onCurrentIndexChanged: {
                    if(!main.isDeviceUpdatingValues){
                        timer.restart();
                    }
                }
            }
        }

        Row{
            width:  parent.width
            height: parent.height*3.75/10
            Item{
                width:  parent.width*1/4
                height: parent.height
                MText{
                    text: "DST"
                    color: "red"
                    rotation: 90
                    font.pixelSize:  _tumblerCln.height*0.2
                    anchors.centerIn: parent
                }
            }

            Tumbler{
                id: _tumblerDst

                width:  parent.width*3/4
                height: parent.height

                model: UiCore.currentDevice.maxPresetCount
                currentIndex: UiCore.currentDevice.drvPresetMap

                visibleItemCount: 1

                wrap: false

                delegate: MText{
                    text: modelData
                    opacity: 0.1 + Math.max(0, 1 - Math.abs(Tumbler.displacement)) * 0.6

                    color: "red"
                    font.pixelSize: _tumblerDst.height*0.7

                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment:   Text.AlignVCenter
                    font.bold: true
                    font.family: "Arial Black"
                }

                onCurrentIndexChanged: {
                    if(!main.isDeviceUpdatingValues){
                        timer.restart();
                    }
                }
            }
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
            UiCore.currentDevice.setLa3Mappings(_tumblerCln.currentIndex, _tumblerDst.currentIndex);
        }
    }

    Connections{
        target: UiCore.currentDevice

        function onPresetMapChanged()
        {
            main.isDeviceUpdatingValues = true;
            _tumblerCln.currentIndex = UiCore.currentDevice.clnPresetMap
            _tumblerDst.currentIndex = UiCore.currentDevice.drvPresetMap
            main.isDeviceUpdatingValues = false;
        }
    }
}
