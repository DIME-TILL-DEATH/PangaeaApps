import QtQuick
import QtQuick.Controls

import CppObjects
import CppEnums

Item
{
    id: main
    property int deviceType: 0

    Image
    {
        //QPen error on resize. В QT5 не было, в Qt6 появилось
        anchors.centerIn: parent
        width:  Math.min(parent.width, parent.height)*0.8
        height: Math.min(parent.width, parent.height)*0.8

        fillMode: Image.PreserveAspectFit
        mipmap: true
        smooth: true

        source: {
            switch(deviceType)
            {
                case DeviceType.CP100: return "qrc:/qml/Images/cp_100.svg";
                case DeviceType.CP100PA: return "qrc:/qml/Images/cp_100_pa.svg";
                case DeviceType.CP16: return "qrc:/qml/Images/cp_16m.svg";
                case DeviceType.CP16PA: return "qrc:/qml/Images/cp_16m_pa.svg";
                default: ""
            }
        }

        sourceSize.width:  width
        sourceSize.height: height
    }

    Connections
    {
        target: UiCore

        function onSgSetUiDeviceParameter(paramType, value)
        {
            if(paramType === DeviceParameter.DEVICE_TYPE)
            {
               main.deviceType = value
            }
        }
    }
}
