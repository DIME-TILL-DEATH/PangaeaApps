import QtQuick
import QtQuick.Controls

import CppObjects

Item
{
    id: main
    property int value: 0

    Image
    {
        //QPen error on resize. В QT5 не было, в Qt6 появилось
        anchors.centerIn: parent
        width:  Math.min(parent.width, parent.height)*0.8
        height: Math.min(parent.width, parent.height)*0.8

        fillMode: Image.PreserveAspectFit
        mipmap: true
        smooth: true

        source: (value==4) ? "qrc:/qml/Images/cp_100_pa.svg" : (value==3)
                           ? "qrc:/qml/Images/cp_16m_pa.svg" :(value==2)
                           ? "qrc:/qml/Images/cp_16m.svg" : (value==1)
                           ? "qrc:/qml/Images/cp_100.svg" : ""
        sourceSize.width:  width
        sourceSize.height: height
    }

    Connections
    {
        target: UiCore
        function onSgSetUIParameter(nameParam, value)
        {
            if(nameParam===("type_dev"))
                main.value = value
        }
    }
}
