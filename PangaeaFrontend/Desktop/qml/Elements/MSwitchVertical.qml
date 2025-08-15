import QtQuick 2.15
import QtQuick.Controls

import Elements 1.0
import StyleSettings 1.0

import CppObjects
import PangaeaBackend

Switch{
    id: _control

    padding: 0

    topInset: 0
    bottomInset: 0

    spacing: 0

    indicator: Rectangle {
        id: _indicator
        width: _control.width * 2/3
        height: _control.height * 1/3

        x: _control.width * 1/3 / 2
        y:  _control.height/3
        radius: height/2
        color: _control.checked ? Style.currentTheme.barHigh : Style.currentTheme.borderOff
        border.color: Style.currentTheme.borderOn

        Behavior on color{ColorAnimation {duration: 200}}

        Rectangle {
             x: _control.checked ? parent.width - width : 0
             anchors.verticalCenter: parent.verticalCenter
             width: height
             height: _indicator.height - _indicator.border.width*2
             radius: height/2
             color: Style.currentTheme.borderOn
             border.color: Style.currentTheme.borderOff

             Behavior on x{PropertyAnimation{duration: 200}}
         }
     }

    contentItem: MLabel{
        id: _label

        text: _control.text
        horizontalAlignment: Text.AlignHCenter

        // font.pixelSize: 5
        font.bold: true

        elide: Text.ElideMiddle

        color: Style.currentTheme.textEnabled

        width: _control.width
        height: _control.height/3
    }
}
