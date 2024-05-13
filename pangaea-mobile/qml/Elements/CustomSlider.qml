import QtQuick 2.15
import QtQuick.Controls 2.0
import Qt5Compat.GraphicalEffects

import StyleSettings 1.0

import CppObjects

Slider
{
    id: root

    implicitWidth: parent.width
    implicitHeight: parent.height/12

    required property ControlValue controlValue

    property string name: controlValue.name
    property string units: controlValue.units
    property int precision: 0


    from: controlValue.minValue
    to: controlValue.maxValue
    value: controlValue.value

    property bool edited: controlValue.isModified

    property bool moduleOn: true

    property bool inverse: false
    property bool bottomLineEnabled: true

    onMoved:
    {
        controlValue.value = value;
    }

    leftPadding: 0
    rightPadding: 0

    ToolTip
    {
        parent: root.handle
        visible: root.pressed
        text: value.toFixed(precision)
        y : -40
    }

    background: Rectangle
    {
        x: root.leftPadding
        width: root.availableWidth
        height: root.height
        color: Style.colorFon

        Rectangle
        {
            id: blueRect
            width: root.inverse?(root.width-root.handle.x+root.handle.width/2):(root.visualPosition==0?0:root.handle.x+root.handle.width/2)
            x: root.inverse?(root.handle.x):(root.x)

            height: parent.height
            gradient: Gradient{
                orientation: Gradient.Horizontal
                GradientStop{
                    position: root.inverse ? 1.0 : 1.0-root.visualPosition;
                    color: Style.colorModul}
                GradientStop{position: root.inverse? root.visualPosition - 1.0 : 1.0; color: moduleOn ? Style.currentTheme.colorModulOn : Style.currentTheme.colorModulOff}
            }
            layer.enabled: root.hovered | root.pressed
        }

        Rectangle
        {
            y: root.height-1
            width: parent.width
            height: 1
            color: "#999999"
            z: 1
            visible: bottomLineEnabled
        }
    }

    handle: Rectangle
    {
        x: root.leftPadding + root.visualPosition * (root.availableWidth - width)

        color:  Style.currentTheme.colorSlider

        width: 2
        height: root.height


        layer.enabled: root.hovered | root.pressed
        layer.effect: DropShadow
        {
            transparentBorder: true
            color: "#00A0E3"
            samples: 10
        }
    }

    MText
    {
        id: textValue
        anchors.fill: parent
        text: value.toFixed(precision) + " " + units + " "
        horizontalAlignment: Text.AlignRight
        verticalAlignment: Text.AlignVCenter
        color: moduleOn ? Style.currentTheme.colorTextEnabled : Style.currentTheme.colorTextDisabled
        z:1
    }

    MText
    {
        id: modulName
        anchors.fill: parent
        text: edited ? ("  "+ name +"*") : ("  " + name)
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignVCenter
        leftPadding: 4
        color: moduleOn ? Style.colorText : Style.currentTheme.colorTextDisabled
        z:1
    }
}
