import QtQuick 2.15

import QtQuick.Controls 2.12
//import QtGraphicalEffects 1.0
import Qt5Compat.GraphicalEffects

import StyleSettings 1.0
import Elements 1.0

CustomizerSliderV
{
    id: root

    property var qSetting
    property int qControlActive

    nameParam: "eq_volume1"

    dispMin: -15
    dispMax: 15

    background: Item
    {
        id: regulator
        anchors.fill: parent

        Rectangle
        {
            anchors.horizontalCenter: parent.horizontalCenter
            height: parent.height
            width: 1
            color: "grey"
        }

        Rectangle
        {
            id: recTop
            visible: yDisp>0
            anchors.bottom: parent.verticalCenter

            gradient:    Gradient{
                orientation: Gradient.Vertical
                GradientStop{
                    position: root.visualPosition;
                    color: (eqsArray.nomActiv === qControlActive) ? Style.currentTheme.colorModulOn : Style.colorModul}
                GradientStop{position: 1.0; color: Style.currentTheme.colorSlider}
            }

            width: parent.width
            height: parent.height*yDisp/35
            border.color: Style.colorFon
            border.width: 1
            opacity: 0.8

            transform: Scale{
                origin.x: root.width/2
                xScale: 1/(qSetting/8+1)
            }
        }
        Rectangle
        {
            id: recBoot
            visible: yDisp<0
            anchors.top: parent.verticalCenter

            gradient:    Gradient{
                orientation: Gradient.Vertical
                GradientStop{
                    position: root.visualPosition;
                    color: (eqsArray.nomActiv === qControlActive) ? Style.currentTheme.colorModulOn : Style.currentTheme.colorModul}
                GradientStop{position: 0; color: Style.currentTheme.colorSlider}
            }

            width: parent.width
            height: parent.height*(-yDisp)/35
            border.color: Style.colorFon
            border.width: 1
            opacity: 0.8

            transform: Scale{
                origin.x: root.width/2
                xScale: 1/(qSetting/8+1)
            }
        }
    }

    handle: Rectangle
    {

        y: root.topPadding + root.availableHeight / 2 - height / 2 - regulator.height*yDisp/35

        implicitWidth:  root.width
        implicitHeight: root.width/5

        color: Style.currentTheme.colorSlider

        layer.enabled: root.hovered | root.pressed
        layer.effect: DropShadow
        {
            transparentBorder: true
            color: "#00A0E3"
            samples: 10
        }

        transform: Scale{
            origin.x: root.width/2
            xScale: 1/(qSetting/8+1)
        }
    }

    MouseArea
    {
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        height: parent.height/100*85
        width:  parent.width

        enabled: (eqsArray.nomActiv!==qControlActive)
        onClicked: {eqsArray.nomActiv = qControlActive;}
    }
}
