import QtQuick
import QtQuick.Controls

import CppObjects
import PangaeaBackend

Item
{
    id: main

    Image
    {
        id: _image

        anchors.centerIn: parent
        width:  Math.min(parent.width, parent.height)*0.9
        height: Math.min(parent.width, parent.height)*0.9

        fillMode: Image.PreserveAspectFit
        smooth: true

        source: {
            switch(UiCore.currentDevice.deviceType)
            {
                case DeviceType.LEGACY_CP100: return "qrc:/Images/cp_100.svg";
                case DeviceType.LEGACY_CP100PA: return "qrc:/Images/cp_100_pa.svg";
                case DeviceType.LEGACY_CP16: return "qrc:/Images/cp_16m.svg";
                case DeviceType.LEGACY_CP16PA: return "qrc:/Images/cp_16m_pa.svg";
                case DeviceType.MODERN_CP: return "qrc:/Images/cp_16m_modern.svg";
                case DeviceType.LA3: return "qrc:/Images/cp_16m_la3_light.svg";
                default: ""
            }
        }

        sourceSize.width:  width
        sourceSize.height: height
    }
}
