import QtQuick 2.15
import QtQuick.Controls

import Elements 1.0
import StyleSettings 1.0

import CppObjects
import PangaeaBackend

Switch{
    id: _exprSwitch

    leftPadding: 0
    leftInset: 0
    spacing: 0

    indicator: Rectangle {
        id: _indicator
        width: height*2
        height: _exprSwitch.height

        x: _exprSwitch.width/2
        y: parent.height / 2 - height / 2
        radius: _exprSwitch.height/2
        color: _exprSwitch.checked ? Style.barHigh : Style.borderOff
        border.color: Style.borderOn

        Behavior on color{ColorAnimation {duration: 200}}

        Rectangle {
             x: _exprSwitch.checked ? parent.width - width : 0
             anchors.verticalCenter: parent.verticalCenter
             width: height
             height: _indicator.height - _indicator.border.width*2
             radius: height/2
             color: Style.borderOn
             border.color: Style.borderOff

             Behavior on x{PropertyAnimation{duration: 200}}
         }
     }

    contentItem: MLabel{
        text: _exprSwitch.text
        verticalAlignment: Text.AlignVCenter

        width: _exprSwitch.width/2
        elide: Label.ElideRight
    }
}
