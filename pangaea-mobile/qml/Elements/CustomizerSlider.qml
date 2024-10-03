import QtQuick 2.15
import QtQuick.Controls 2.0
import Qt5Compat.GraphicalEffects

import StyleSettings 1.0

import CppObjects

Slider
{
    id: root

    property string name: "BAR"
    property string units: " "

    property var paramType

    property int valueMin:  0   //Y1
    property int valueMax:  31  //Y2

    property int dispMin:   0  //Y1
    property int dispMax:   31 //Y2

    property int x1val: 0
    property int x2val: 100

    property bool softUpdate: false

    property bool moduleOn: true

    property bool inverse: false
    property bool edited: false

    property bool bottomLineEnabled: true

    property int maximumValue: 1
    property int minimumValue: 2

    property int delitel: 1  //Делитель, если нужны десятичные занки

    property double kDisp: -((dispMax-dispMin)/(x1val-x2val))
    property double bDisp: -(((x2val*dispMin)-(x1val*dispMax))/(x1val-x2val))
    property double yDisp: Math.round((kDisp * root.value * 100 + bDisp))/delitel

    property double kVal: -((valueMax-valueMin)/(x1val-x2val))
    property double bVal: -(((x2val*valueMin)-(x1val*valueMax))/(x1val-x2val))
    property double yVal: Math.round(kVal * root.value * 100 + bVal)

    property int rawValue
    value:  (rawValue - bVal) / 100 / kVal

    leftPadding: 0
    rightPadding: 0

    ToolTip
    {
        parent: root.handle
        visible: root.pressed
        text: yDisp
        scale: 1.2
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
        text: yDisp + " " + units + " "
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

    onValueChanged:
    {
        if(!softUpdate)
        {
            UiCore.setDeviceParameter(paramType, Math.round(kVal * root.value * 100 + bVal));
            edited = true;
        }
        else
        {
            edited = false;
        }
    }

    Connections
    {
        target: UiCore

        function onSgSetUiDeviceParameter(paramType, value)
        {
            if(paramType === root.paramType)
            {
                softUpdate = true;
                root.rawValue = value;
                softUpdate = false;
            }
        }
    }

    Connections
    {
        target: DeviceProperties

        function onPresetModifiedChanged()
        {
            if(!DeviceProperties.presetModified)
            {
                edited = false;
            }
        }
    }
}
