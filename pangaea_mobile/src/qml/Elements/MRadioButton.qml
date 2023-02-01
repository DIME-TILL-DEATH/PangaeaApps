import QtQuick 2.15
import QtQuick.Controls 2.12 //2.5
import StyleSettings 1.0

import Elements 1.0

RadioButton {
    id: control

    property color colorCheck : Style.currentTheme.colorLedOn


//    indicator: Rectangle {
//             implicitWidth: 26
//             implicitHeight: 26
//             x: control.leftPadding
//             y: parent.height / 2 - height / 2
//             radius: 13
//             border.color: control.down ? "#17a81a" : colorCheck

//             Rectangle {
//                 width: 14
//                 height: 14
//                 x: 6
//                 y: 6
//                 radius: 7
//                 color: control.down ? "#17a81a" : colorCheck
//                 visible: control.checked
//             }
//         }

    contentItem: MText{
        text: control.text

        color: colorCheck
        verticalAlignment: Text.AlignVCenter
        leftPadding: control.indicator.width + control.spacing
   }

}
