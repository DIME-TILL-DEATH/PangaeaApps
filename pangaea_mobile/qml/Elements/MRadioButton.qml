import QtQuick 2.15
import QtQuick.Controls 2.12 //2.5
import StyleSettings 1.0

import Elements 1.0

RadioButton {
    id: control

    property color colorCheck : Style.currentTheme.colorLedOn

    contentItem: MText{
        text: control.text

        color: colorCheck
        verticalAlignment: Text.AlignVCenter
        leftPadding: control.indicator.width + control.spacing
   }

}
