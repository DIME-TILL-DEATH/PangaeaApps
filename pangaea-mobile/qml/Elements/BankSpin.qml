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

    property bool editable: true
    property bool isLA3: DeviceProperties.isLa3Mode

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
                onClicked: openPresetsList() //{
                    // UiCore.setParameter("open_preset_list", 1);
                // }
            }
        }

        Tumbler
        {
            id: _tumbler

            width:  parent.width
            height: parent.height*6/10

            model: DeviceProperties.banksList
            currentIndex: DeviceProperties.bank

            visibleItemCount: 1

            wrap: false

            property bool isLA3: ((DeviceProperties.deviceType === DeviceType.LA3PA) | (DeviceProperties.deviceType === DeviceType.LA3RV))

            delegate: MText{
                text: modelData
                opacity: 0.1 + Math.max(0, 1 - Math.abs(Tumbler.displacement)) * 0.6

                color: isLA3 ? (_tumbler.currentIndex < 2) ? "lightgreen" : "red"
                             : Style.colorText

                font.pixelSize: isLA3 ? _tumbler.height*0.3 : _tumbler.height*0.75

                horizontalAlignment: Text.AlignHCenter
                verticalAlignment:   Text.AlignVCenter
                font.bold: true
                font.family: "Arial Black"
            }

            onCurrentIndexChanged:
            {
                timer.restart();
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
        interval: 250
        repeat: false
        onTriggered:
        {
            DeviceProperties.bank = _tumbler.currentIndex;
        }
    }
}
