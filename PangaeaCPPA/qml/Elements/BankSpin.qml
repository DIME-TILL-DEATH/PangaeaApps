import QtQuick
import QtQuick.Controls

import StyleSettings

import CppObjects
import CppEnums

Item
{
    id: main

    function up()
    {
        if(DeviceProperties.bank < (DeviceProperties.banksList.length-1))
        {
            DeviceProperties.bank++;
        }
    }
    function down()
    {
        if(DeviceProperties.bank > 0)
        {
            DeviceProperties.bank--;
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

                anchors.centerIn: parent.Center

                height: parent.height
                width: parent.width
                model: DeviceProperties.banksList

                currentIndex: DeviceProperties.bank
                visibleItemCount: 1

                delegate: Text
                {
                    text: modelData
                    color: DeviceProperties.isLa3Mode? (tumb.currentIndex<2 ? "green" : "red") : "Red"
                    font.pixelSize: DeviceProperties.isLa3Mode ? parent.height*0.6 : parent.height*0.9
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

                onWheel: function wheelHandler(wheel)
                {
                    if(wheel.angleDelta.y>0) main.up();
                    else main.down();
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
                text: qsTr("BANK")
            }
        }
    }
}
