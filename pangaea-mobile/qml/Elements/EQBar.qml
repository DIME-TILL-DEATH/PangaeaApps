import QtQuick 2.15
import Qt5Compat.GraphicalEffects

import StyleSettings 1.0

Rectangle
{
    id: _root
    property bool isOn

    property int eqSetting
    property int qSetting

    height: parent.height
    width: parent.width*band_width_mult-4
    color: Style.colorFon

    Rectangle
    {
        visible: eqSetting>0
        anchors.bottom: parent.verticalCenter    
        gradient:  Gradient{
            orientation: Gradient.Vertical
            GradientStop{
                position: 0;
                color: isOn ? Style.currentTheme.colorModulOn : Style.currentTheme.colorModulOff}
            GradientStop{position: 1.0; color: Style.colorModul}
        }
        width: parent.width
        height: parent.height*eqSetting/15/2

        transform: Scale{
            origin.x: _root.width/2
            xScale: 1/(qSetting/8+1)
        }
    }
    Rectangle
    {
        anchors.bottom: parent.verticalCenter
        color: Style.currentTheme.colorSlider
        width: parent.width
        height: 1
    }

    Rectangle
    {
        visible: eqSetting<0
        anchors.top: parent.verticalCenter
        gradient:  Gradient{
            orientation: Gradient.Vertical
            GradientStop{
                position: 1.0;
                color: isOn ? Style.currentTheme.colorModulOn : Style.currentTheme.colorModulOff}
            GradientStop{position: 0; color: Style.colorModul}
        }
        width: parent.width
        height: parent.height*(-eqSetting)/15/2

        transform: Scale{
            origin.x: _root.width/2
            xScale: 1/(qSetting/8+1)
        }
    }
}
