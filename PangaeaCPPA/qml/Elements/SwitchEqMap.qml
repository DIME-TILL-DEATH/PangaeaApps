import QtQuick
import QtQuick.Controls

import StyleSettings

import CppObjects 1.0

Item
{
    id: main

    property bool   map: false
    property bool   eqOn: false

    Canvas
    {
        id: eqCanvas
        anchors.fill: parent 
        onPaint:
        {
            var ctx = getContext("2d")
            var colorEqTab = main.map ? Style.backgroundColor
                                      : (eqOn) ? Style.mainEnabledColor : Style.mainDisabledColor
            var colorMapTab = main.map ? Style.mainEnabledColor : Style.backgroundColor

            ctx.lineWidth = 4

            ctx.strokeStyle = colorEqTab
            ctx.fillStyle = colorEqTab

            ctx.beginPath()
            ctx.moveTo(0, 0)
            main.map ? ctx.lineTo(parent.width/2+parent.width/30, 0)
                     : ctx.lineTo(parent.width/2-parent.width/30, 0)
            main.map ? ctx.lineTo(parent.width/2-parent.width/30, parent.height)
                     : ctx.lineTo(parent.width/2+parent.width/30, parent.height)
            ctx.lineTo(0, parent.height)
            ctx.closePath()
            ctx.fill()
            ctx.stroke()

            ctx.strokeStyle = colorMapTab
            ctx.fillStyle =   colorMapTab

            ctx.beginPath()
            ctx.moveTo(parent.width, 0)
            ctx.lineTo(parent.width, parent.height)
            main.map ? ctx.lineTo(parent.width/2-parent.width/30, parent.height)
                     : ctx.lineTo(parent.width/2+parent.width/30, parent.height)
            main.map ? ctx.lineTo(parent.width/2+parent.width/30, 0)
                     : ctx.lineTo(parent.width/2-parent.width/30, 0)
            ctx.closePath()
            ctx.fill()
            ctx.stroke()
        }
    }
    MText
    {
        width:  parent.width/2
        height: parent.height
        anchors.left: parent.left
        text: "EQ"
        font.pixelSize: parent.height/1.8
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment:   Text.AlignVCenter
        color: eqOn ? Style.highlightColor : (main.map ? Style.mainEnabledColor : Style.backgroundColor)
        MouseArea
        {
            anchors.fill: parent
            hoverEnabled: true
            cursorShape:  Qt.PointingHandCursor
            onClicked:
            {
                if( main.map)
                    main.map=false;
                else
                {
                    UiCore.setParameter("eq_on", !eqOn);
                }
                redraw();
            }
        }
    }

    MText
    {
        width:  parent.width/2
        height: parent.height
        anchors.right: parent.right
        text: "MAP"
        font.pixelSize: parent.height/1.8
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment:   Text.AlignVCenter
        color: main.map ? Style.backgroundColor : Style.mainEnabledColor
        MouseArea
        {
            anchors.fill: parent
            hoverEnabled: true
            cursorShape:  Qt.PointingHandCursor
            onClicked:
            {
                main.map=true;
                redraw();
            }
        }
    }

    Connections
    {
        target: UiCore
        function onSgSetUIParameter(nameParam, value)
        {
            if(nameParam === "eq_on")
            {
                main.eqOn = value;
                redraw();
            }
        }
        function onSgSetParameter(nameParam, value)
        {
            if(nameParam === "eq_on")
            {
                main.eqOn = value;
                redraw();
            }
        }
    }

    function redraw()
    {
        eqCanvas.requestPaint();
    }
}
