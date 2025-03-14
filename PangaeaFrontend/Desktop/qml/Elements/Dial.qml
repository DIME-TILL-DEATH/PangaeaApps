import QtQuick

import StyleSettings

import CppObjects

Item
{
    id: main

    property string name: "DIAL"
    property int paramType

    property bool checkable: true
    property bool on: true

    property int value:     15
    property int valueLast: 0
    property int valueLastSend: 0
    property int valueMin:  0
    property int valueMax:  31
    property int dispMin:   0
    property int dispMax:   31
    property int angleMin: -140
    property int angleMax:  140

    property double k2: (dispMin-dispMax)/(valueMin-valueMax)
    property double k1:  dispMin-(valueMin*k2)
    property double dispValue: Math.round(k1 + value*k2)

    property double a2: (angleMin-angleMax)/(valueMin-valueMax)
    property double a1:  angleMin-(valueMin*a2)
    property int dispAngle: a1 + value*a2
    signal chValue(int value)

    Column
    {
        anchors.fill: parent
        opacity: main.enabled?1:0.5
        Behavior on opacity  {NumberAnimation { duration: 200 }}
        Item
        {
            height: Math.min(parent.height, parent.width)*0.76
            width : Math.min(parent.height, parent.width)*0.76
            anchors.horizontalCenter: parent.horizontalCenter
            Rectangle
            {
                anchors.fill: parent
                radius: parent.width
                color: Style.backgroundColor
                Item
                {
                    height: parent.height
                    width:  parent.width/7
                    anchors.centerIn: parent
                    Rectangle
                    {
                        anchors.centerIn: parent
                        anchors.verticalCenterOffset: -parent.height/5*2
                        height: Math.min(parent.height, parent.width)
                        width : Math.min(parent.height, parent.width)
                        radius: width
                        color: Style.mainEnabledColor
                    }
                    rotation:  dispAngle
                    Behavior on rotation  {NumberAnimation { duration: 200 }}
                }
                MText
                {
                    anchors.centerIn: parent
                    color: Style.mainEnabledColor
                    font.pixelSize: parent.width/5
                    text: dispValue
                }

                MouseArea
                {
                    id: mArea
                    property int startAngle
                    property int lastX
                    property int lastY
                    property int angle
                    property int curAngle
                    property int lastAngle
                    property bool changeZnalAngle
                    anchors.fill: parent
                    enabled: main.on
                    hoverEnabled: true
                    cursorShape: main.on?(pressed?Qt.ClosedHandCursor:Qt.OpenHandCursor):Qt.ArrowCursor
                    onPositionChanged:
                    {
                        if (pressed)
                        {
                            angle = (angleFromPoint(mouseX, mouseY));
                            if(angle*lastAngle<0)
                            {
                                if(Math.abs(angle)>100)
                                    changeZnalAngle^=1;
                            }
                            if(!changeZnalAngle)
                                curAngle = angle;
                            else
                                curAngle = (angle<0)?(360+angle):(-360+angle);
                            valueUpdate(main.valueLast + curAngle/((angleMax-angleMin)/(valueMax-valueMin)));
                            lastAngle = angle;
                        }
                    }
                    onPressed:
                    {
                        changeZnalAngle = false;
                        lastX = mouseX;
                        lastY = mouseY;
                        main.valueLast = main.value;
                    }
                    onWheel: function(wheel)
                    {
                        valueUpdate(main.value + wheel.angleDelta.y/120);
                    }

                    MouseArea
                    {
                        property int lastY
                        width:  parent.width/2
                        height: parent.height/2
                        anchors.centerIn: parent
                        cursorShape: Qt.SizeVerCursor
                        onPositionChanged:
                        {
                            if (pressed)
                            {
                                valueUpdate(value +(lastY - mouseY));
                                lastY = mouseY;
                            }
                        }
                        onPressed: lastY = mouseY;
                    }
                }
            }
        }

        Item
        {
            height: Math.min(parent.height, parent.width)*0.24
            width : Math.min(parent.height, parent.width)*0.76
            anchors.horizontalCenter: parent.horizontalCenter
            MText
            {
                anchors.verticalCenter:   parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenterOffset: -parent.width/30
                color: main.checkable ? (main.on ? "Salmon"  :"Blue") : Style.backgroundColor
                font.pixelSize: parent.width/6
                text: main.name
                MouseArea
                {
                    anchors.fill: parent
                    hoverEnabled: true
                    cursorShape: main.checkable?Qt.PointingHandCursor:Qt.ArrowCursor
                    onClicked:
                    {
                        if(main.checkable)
                            main.on = (!main.on)
                    }
                }
            }
        }
        Timer
        {
            id: timer
            interval: 100
            running: false
            repeat: false
            onTriggered:
            {
            }
        }
    }

    function angleFromPoint(x, y)
    {
        var angle;
        var centerX = mArea.width  / 2;
        var centerY = mArea.height / 2;
        var vectorAX = mArea.lastX - centerX;
        var vectorAY = mArea.lastY - centerY;
        var vectorBX = x - centerX;
        var vectorBY = y - centerY;
        angle = Math.atan2(vectorAX*vectorBY - vectorAY*vectorBX, vectorAX*vectorBX + vectorAY*vectorBY) ;
        angle *= 180;
        angle /= Math.PI;
        return angle;
    }

    function valueUpdate(addValue)
    {
        valueUpdateSoft(addValue)
        if(main.valueLastSend!=main.value)
        {
            main.valueLastSend=main.value;
            main.chValue(main.value);

            UiCore.setDeviceParameter(main.paramType, main.value)
        }
    }

    function valueUpdateSoft(addValue)
    {
        main.value  = addValue;
        main.value = main.value<=valueMin ? valueMin : main.value;
        main.value = main.value>=valueMax ? valueMax : main.value;
    }

    Connections
    {
        target: UiCore

        function onSgSetUiDeviceParameter(paramType, value)
        {
            if(paramType === main.paramType)
            {
                valueUpdateSoft(value);
            }
        }
    }
}
