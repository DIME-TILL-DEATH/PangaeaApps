import QtQuick
import QtQuick.Controls

import CppObjects
import CppEnums

Item
{
    id: main

    Image
    {
        id: _image
        //QPen error on resize. В QT5 не было, в Qt6 появилось
        anchors.centerIn: parent
        width:  Math.min(parent.width, parent.height)*0.8
        height: Math.min(parent.width, parent.height)*0.8

        fillMode: Image.PreserveAspectFit
        mipmap: true
        smooth: true

        source: {
            switch(UiCore.currentDevice.deviceType)
            {
                case DeviceType.LEGACY_CP100: return "qrc:/qml/Images/cp_100.svg";
                case DeviceType.LEGACY_CP100PA: return "qrc:/qml/Images/cp_100_pa.svg";
                case DeviceType.LEGACY_CP16: return "qrc:/qml/Images/cp_16m.svg";
                case DeviceType.LEGACY_CP16PA: return "qrc:/qml/Images/cp_16m_pa.svg";
                case DeviceType.MODERN_CP16: return "qrc:/qml/Images/cp_16m_pa.svg";
                default: ""
            }
        }

        sourceSize.width:  width
        sourceSize.height: height
    }
}
