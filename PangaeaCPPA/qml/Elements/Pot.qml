import QtQuick

import StyleSettings

import CppObjects 1.0

Item
{
    id: main

    property int value:     15
    property int valueLast: 0
    property int valueLastSend: 0
    property int valueMin:  0
    property int valueMax:  31
    property int dispMin:   -15
    property int dispMax:   15

    property int paramType

    property double k2: (dispMin-dispMax)/(valueMin-valueMax)
    property double k1:  dispMin-(valueMin*k2)
    property int dispValue: k1 + value*k2

    Item
    {
        anchors.centerIn: parent
        anchors.fill: parent

        Item
        {
            id: valueArea
            anchors.centerIn: parent
            width:  parent.width
            height: parent.height*0.8


            Rectangle
            {
                id: pot
                anchors.centerIn: parent
                anchors.verticalCenterOffset: ((valueMax-valueMin)/2-value) * (valueArea.height/valueMax)
                width:  parent.width*0.7
                height: parent.width*0.7/2
                radius: parent.width*0.2
                color: Style.backgroundColor
                MText
                {
                    color: Style.mainEnabledColor
                    anchors.centerIn: parent
                    font.pixelSize: parent.height/2.5
                    text: dispValue
                }
                opacity: main.enabled?1:0.5
                Behavior on opacity  {NumberAnimation { duration: 200 }}
                Behavior on anchors.verticalCenterOffset {NumberAnimation { duration: 200 }}
            }

            MouseArea
            {
                anchors.fill: parent
                hoverEnabled: true
                cursorShape: pressed?Qt.ClosedHandCursor:Qt.OpenHandCursor
                onPositionChanged:
                {
                    if (pressed)
                        valueUpdate(mouseY);
                }
                onPressed:
                {
                    valueUpdate(mouseY);
                }

                onWheel:
                {
                    value += (wheel.angleDelta.y/120);
                    normal();
                }
            }
        }
    }

    function valueUpdate(valMouseY)
    {
        value = (valueArea.height-valMouseY)/valueArea.height*valueMax;
        normal();
    }

    function normal()
    {
        value = value<=valueMin?valueMin:value;
        value = value>=valueMax?valueMax:value;
        if(main.valueLastSend!=main.value)
        {
            UiCore.setDeviceParameter(main.paramType, main.value);
            main.valueLastSend=main.value;
        }
    }

    Connections
    {
        target: UiCore

        function onSgSetUiDeviceParameter(paramType, value)
        {
            if(paramType === main.paramType)
            {
                main.value=value;
            }
        }
    }
}
