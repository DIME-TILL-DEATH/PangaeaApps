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

    property bool isModified: false

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

            color: _mapBtnMa.pressed ? Style.currentTheme.colorItemHighlight : Style.colorFon

            border.width: 1
            border.color: Style.currentTheme.colorBorderOn
            radius: Style.baseRadius

            MText{
                text: qsTr("SET")

                color:  main.isModified ? Style.colorText : Style.colorBtnDisabled

                anchors.centerIn: parent

                font.bold: true
                font.pixelSize: main.width/5
            }

            MouseArea{
                id: _mapBtnMa


                anchors.fill: parent
                onClicked: {
                    main.isModified = false;
                    DeviceProperties.setLa3Mappings(_tumblerCln.currentIndex, _tumblerDst.currentIndex);
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

                model: DeviceProperties.presetsList
                //currentIndex: DeviceProperties.la3CleanPreset

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
                    main.isModified = true;
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

                model: DeviceProperties.presetsList
                //currentIndex: DeviceProperties.la3DrivePreset

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
                    main.isModified = true;
                }
            }
        }
    }

    Connections{
        target: DeviceProperties

        function onLa3CleanPresetChanged()
        {
            _tumblerCln.currentIndex = DeviceProperties.la3CleanPreset
            main.isModified = false;
        }

        function onLa3DrivePresetChanged()
        {
            _tumblerDst.currentIndex = DeviceProperties.la3DrivePreset
            main.isModified = false;
        }
    }
}
