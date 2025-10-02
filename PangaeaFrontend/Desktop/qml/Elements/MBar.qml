import QtQuick 2.15
import QtQuick.Controls

import Elements 1.0

import StyleSettings 1.0

import CppObjects
import PangaeaBackend

Row{
    id: _root

    property alias text: _label.text

    property int minValue: 0
    property int maxValue: 127

    property int value: 40

    property alias textWidth: _label.width
    property alias barWidth: _barRect.width

    signal userChangedValue(var calcVal);

    MLabel{
        id: _label

        width: parent.width/2
        anchors.verticalCenter: parent.verticalCenter
    }

    Rectangle{
        id: _barRect

        width: parent.width / 2
        height: parent.height
        anchors.verticalCenter: parent.verticalCenter

        color: "transparent"

        border.width: 2
        border.color: Style.currentTheme.borderOn

        MText{
            anchors.centerIn: parent

            z: _bar.z + 1

            text: _root.value
            color: Style.currentTheme.barText
        }

        Rectangle{
            id: _bar
            anchors.verticalCenter: parent.verticalCenter

            gradient: Gradient{
                orientation: Gradient.Horizontal
                GradientStop{position: 0; color: Style.currentTheme.barLow}
                GradientStop{position: 1; color: Style.currentTheme.barHigh}
            }

            x: parent.border.width

            height: parent.height - parent.border.width * 2
            width: parent.width * _root.value/(_root.maxValue-_root.minValue) - parent.border.width*2
        }

        MouseArea{
            anchors.fill: parent

            function changeValue(mouseX){
                var calcVal = (_root.maxValue - _root.minValue) * mouseX/parent.width
                if(calcVal > _root.maxValue) calcVal = _root.maxValue
                if(calcVal < _root.minValue) calcVal = _root.minValue

                // _root.value = calcVal

                _root.userChangedValue(calcVal);
            }


            onPressed: (event) => {
                changeValue(event.x);
            }

            onPositionChanged: (event) => {
                changeValue(event.x);
            }
        }
    }
}
