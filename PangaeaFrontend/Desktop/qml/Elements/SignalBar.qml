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
    property real to: 32//20 * log10(32bit)

    clip: true

    Rectangle{
        id: fill

        color: "green"
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom

        width: parent.width - parent.border.width
        height: parent.height - (to - from - value)*parent.height/(to-from)

        z: root.z-1
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
