import QtQuick 2.15
import QtQuick.Controls 2.15

import Elements

CheckBox {
    id: control
    checked: true

    contentItem: MText {
         text: control.text
         // font: control.font
         opacity: enabled ? 1.0 : 0.3
         // color: control.down ? "#17a81a" : "#21be2b"
         verticalAlignment: Text.AlignVCenter
         leftPadding: control.indicator.width + control.spacing
     }
}
