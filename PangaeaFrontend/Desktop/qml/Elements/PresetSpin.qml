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
        if(UiCore.currentDevice.preset < (UiCore.currentDevice.maxPresetCount-1))
        {
            tumb.currentIndex++;
            UiCore.sgQmlRequestChangePreset(UiCore.currentDevice.bank, tumb.currentIndex);
        }
    }
    function down()
    {
        if(UiCore.currentDevice.preset > 0)
        {
            tumb.currentIndex--;
            UiCore.sgQmlRequestChangePreset(UiCore.currentDevice.bank, tumb.currentIndex);
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

                model: UiCore.currentDevice.strPresetNumbers
                currentIndex: UiCore.currentDevice.preset

                visibleItemCount: 1
                delegate: Text
                {
                    text: modelData

                    color: Style.currentTheme.spin
                    font.pixelSize: Math.min(parent.height*0.9, parent.width*0.9)

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
                        if(UiCore.currentDevice.preset < (UiCore.currentDevice.maxPresetCount-1))
                        {
                            tumb.currentIndex++;
                            timer.restart();
                        }
                    }
                    else
                    {
                        if(UiCore.currentDevice.preset > 0)
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
                    UiCore.sgQmlRequestChangePreset(UiCore.currentDevice.bank, tumb.currentIndex);
                }
            }

            Connections{
                target: UiCore.currentDevice

                function onBankPresetChanged()
                {
                    main.deviceUpdatingValues = true;
                    tumb.currentIndex = UiCore.currentDevice.preset
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
                color:  Style.currentTheme.mainEnabledColor
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: parent.height/1.1
                text: qsTr("PRESET")
            }
        }
    }

    Connections{
        target: UiCore.currentDevice

        function onDeviceRestoreValues(){
            main.deviceUpdatingValues = true;
            tumb.currentIndex = UiCore.currentDevice.preset
            main.deviceUpdatingValues = false;
        }
    }
}
