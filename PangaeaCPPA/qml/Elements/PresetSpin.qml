import QtQuick
import QtQuick.Controls

import StyleSettings

import CppObjects
import CppEnums

Item
{
    id: main

    property bool editable: true

    function up()
    {
        if(DeviceProperties.preset < (DeviceProperties.presetsList.length-1))
        {
            DeviceProperties.preset++;
        }
    }
    function down()
    {
        if(DeviceProperties.preset > 0)
        {
            DeviceProperties.preset--;
        }
    }

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

                height: parent.height
                width: parent.width
                anchors.centerIn: parent.Center

                model: DeviceProperties.presetsList
                currentIndex: DeviceProperties.preset

                visibleItemCount: 1
                delegate: Text
                {
                    text: modelData

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
                    if(mouse.modifiers & Qt.ControlModifier) main.down();
                    else
                    {
                        if(mouse.button & Qt.LeftButton)
                       {
                           main.up();
                       }
                        else main.down();
                    }
                }
                onEntered: tp.visible = true
                onExited:  tp.visible = false
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
                text: qsTr("PRESET")
            }
        }
    }
}
