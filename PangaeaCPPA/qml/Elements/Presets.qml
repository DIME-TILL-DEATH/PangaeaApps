import QtQuick
import QtQuick.Controls

import StyleSettings

import CppObjects
import CppEnums

Item
{
    id: main

    property string fonColor: "#EBECEC"
    property string devColor: "#5E5971"
    property int    value:     tumb.currentIndex
    property int maxMapRow: 10
    property string text:  "TEXT"
    property string nameValue:  ""
    property bool editable: true

    signal chPreset()

    Column
    {
        anchors.fill:  parent
        Item
        {
            width:  parent.width
            height: parent.height/100*80
            Tumbler
            {
                id: tumb
                model: 10
                anchors.horizontalCenter: parent.horizontalCenter
                visibleItemCount: 1
                height: parent.height
                width: parent.width
                anchors.centerIn: parent.Center
                delegate: Text
                {
                    text: modelData
                    opacity: 0.1 + Math.max(0, 1 - Math.abs(Tumbler.displacement)) * 0.6
                    color: "Red"
                    font.pixelSize: parent.height*0.9
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment:   Text.AlignVCenter
                    font.bold: true
                    font.family: "Arial Black"
                }
            }
            MouseArea
            {
                anchors.fill: parent
                hoverEnabled: true

                onWheel:
                {
                    if(wheel.angleDelta.y>0)
                    {
                        if(value < (maxMapRow-1))
                        {
                            tumb.currentIndex++;
                            timer.restart();
                        }
                    }
                    else
                    {
                        if(value>0)
                        {
                            tumb.currentIndex--;
                            timer.restart();
                        }
                    }
                }
                acceptedButtons: Qt.LeftButton | Qt.RightButton

                onClicked: (mouse)=>
                {
                    if( mouse.modifiers & Qt.ControlModifier )
                    {
                        if(value>0)
                        {
                            tumb.currentIndex--;
                            timer.restart();
                        }
                    }
                    else
                    {
                        if(mouse.button & Qt.LeftButton)
                        {
                            if(value < (maxMapRow-1))
                            {
                                tumb.currentIndex++;
                                timer.restart();
                            }
                        }
                        else
                        {
                            if(value>0)
                            {
                                tumb.currentIndex--;
                                timer.restart();
                            }
                        }
                    }
                }
                onEntered: tp.visible = true
                onExited:  tp.visible = false

                Timer
                {
                    id: timer
                    interval: 700
                    repeat: false
                    onTriggered: main.chPreset()
                }
            }

            ToolTip
            {
                id: tp
                text: (Qt.platform.os==="windows") ? qsTr("Up - Mouse Click") + "\r\n" + qsTr("Down - Click + Ctrl")
                                                   : qsTr("Up - Mouse Click") + "\r\n" + qsTr("Down - Click + cmd")
                visible: false
                timeout: 2000
            }
        }

        Item
        {
            width:  parent.width
            height: parent.height/100*20
            MText
            {
                anchors.fill:  parent
                color:  Style.mainEnabledColor
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: parent.height/1.1
                text: main.text
            }
        }
    }

    function up()
    {
        tumb.currentIndex++;
    }

    function down()
    {
        tumb.currentIndex--;
    }

    Connections
    {
        target: UiCore
        function onSgSetUIParameter(nameParam, value)
        {
            if((main.nameValue.length>0)&&(nameParam===main.nameValue))
                tumb.currentIndex=value;

            if(nameParam===("type_dev"))
            {
                switch (value)
                {
                case DeviceType.CP100: maxMapRow = 10;  break;
                case DeviceType.CP100PA: maxMapRow = 10;  break;
                case DeviceType.CP16: maxMapRow = 4; break;
                case DeviceType.CP16PA: maxMapRow = 4; break;
                }
            }
        }
    }
}
