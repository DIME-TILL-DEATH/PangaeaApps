import QtQuick
import QtQuick.Controls

import Elements

import CppObjects

Rectangle {
    id: root

    border.width: 1

    color: "transparent"

    property real value: 0
    property real from: 0
    property real to: 24//20 * log10(32bit)

    Rectangle{
        id: fill

        color: "green"

        width: parent.width
        height: parent.height - (to - from - value)*parent.height/(to-from)

        z: 1

        anchors.bottom: parent.bottom
    }

    Connections
    {
        target: UiCore

        function onSgSetUiDeviceParameter(paramType, value)
        {
            if(paramType === DeviceParameter.FIRMWARE_CAN_INDICATE)
            {
                root.visible = value;
            }
        }
    }
}
