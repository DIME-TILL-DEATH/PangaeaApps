import QtQuick 2.15
import QtQuick.Controls 2.15

import StyleSettings 1.0

import CppObjects

Rectangle
{
    id: main

    width:  parent.width*1/6
    height: parent.height

    property int    value:     _tumbler.currentIndex
    property int maxMapRow: 4
    property string text:  "TEXT"
    property string nameValue:  ""
    property bool editable: true

    property bool softUpdate: true

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
                onClicked: {
                    UiCore.setParameter("open_preset_list", 1);
                }
            }
        }

        Tumbler
        {
            id: _tumbler

            width:  parent.width
            height: parent.height*6/10

            model: maxMapRow

            visibleItemCount: 1

            wrap: false

            delegate: MText
            {
                text: modelData
                opacity: 0.1 + Math.max(0, 1 - Math.abs(Tumbler.displacement)) * 0.6
                color: Style.colorText
                font.pixelSize: _tumbler.height*0.75 // /(Math.abs(Tumbler.displacement*2)+1)
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment:   Text.AlignVCenter
                font.bold: true
                font.family: "Arial Black"
            }

            onCurrentIndexChanged:
            {
                if(!softUpdate)
                    timer.restart();

                softUpdate = false;
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

            text: main.text
        }
    }

    Timer
    {
        id: timer
        interval: 700
        repeat: false
        onTriggered:
        {
            if( !softUpdate )
            {
                UiCore.setParameter("set_preset_change", presetNom);
            }
        }
    }


    function up()
    {
        _tumbler.currentIndex++;
    }

    function down()
    {
        _tumbler.currentIndex--;
    }

    Connections
    {
        target: UiCore
        function onSgSetUIParameter(nameParam, nameValue)
        {
            if((main.nameValue.length>0)&&(nameParam === main.nameValue))
            {
                softUpdate = true;
                _tumbler.currentIndex=nameValue;
                softUpdate = false;
            }

            if(nameParam === "set_max_map")
            {
                maxMapRow = nameValue
            }
        }
    }
}
