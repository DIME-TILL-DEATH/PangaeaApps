import QtQuick 2.15
import QtQuick.Controls 2.15

import Elements

CheckBox {
    id: control
    checked: true

    contentItem: MText {
         text: control.text

         opacity: enabled ? 1.0 : 0.3

         verticalAlignment: Text.AlignVCenter
         leftPadding: control.indicator.width + control.spacing
     }
}
