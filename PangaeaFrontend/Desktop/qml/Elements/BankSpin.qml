import QtQuick
import QtQuick.Controls

import StyleSettings

import CppObjects
import PangaeaBackend

Item
{
    id: main

    property bool deviceUpdatingValues: true

    function up()
    {
        if(UiCore.currentDevice.bank < (UiCore.currentDevice.maxBankCount-1))
        {
            tumb.currentIndex++;
            UiCore.sgQmlRequestChangePreset(tumb.currentIndex, UiCore.currentDevice.preset);
        }
    }
    function down()
    {
        if(UiCore.currentDevice.bank > 0)
        {
            tumb.currentIndex--;
            UiCore.sgQmlRequestChangePreset(tumb.currentIndex, UiCore.currentDevice.preset);
        }
    }

    Column
    {
        anchors.fill:  parent
        Item
        {
            width:  parent.width
            height: parent.height/100*75
            Tumbler
            {
                id: tumb

                anchors.centerIn: parent.Center

                height: parent.height
                width: parent.width

                model: UiCore.currentDevice.maxBankCount
                currentIndex: UiCore.currentDevice.bank
                visibleItemCount: 1

                delegate: Text
                {
                    text: modelData

                    color: Style.currentTheme.spin
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

                onWheel: function wheelHandler(wheel){
                    if(wheel.angleDelta.y>0)
                    {
                        if(UiCore.currentDevice.bank < (UiCore.currentDevice.maxBankCount-1))
                        {
                            tumb.currentIndex++;
                            timer.restart();
                        }
                    }
                    else
                    {
                        if(UiCore.currentDevice.bank > 0)
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

            Timer
            {
                id: timer
                interval: 1000
                repeat: false
                onTriggered:
                {
                    UiCore.sgQmlRequestChangePreset(tumb.currentIndex, UiCore.currentDevice.preset);
                }
            }

            Connections{
                target: UiCore.currentDevice

                function onBankPresetChanged()
                {
                    main.deviceUpdatingValues = true;
                    tumb.currentIndex = UiCore.currentDevice.bank
                    main.deviceUpdatingValues = false;
                }
            }
        }

        Item
        {
            width:  parent.width
            height: parent.height/100*20
            MText
            {
                anchors.fill:  parent
                color:  Style.currentTheme.textInverted
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: Math.min(parent.height * 0.9, parent.width*0.7)

                width: parent.width
                elide: Text.ElideMiddle
                text: qsTr("BANK")
            }
        }
    }

    Connections{
        target: UiCore.currentDevice

        function onDeviceRestoreValues(){
            main.deviceUpdatingValues = true;
            tumb.currentIndex = UiCore.currentDevice.bank
            main.deviceUpdatingValues = false;
        }
    }
}
