import QtQuick 2.15
import QtQuick.Controls 2.15

Item
{
    id: main
    property int value: 0

    Image
    {
        anchors.centerIn: parent
        width:  Math.min(parent.width, parent.height)*0.8
        height: Math.min(parent.width, parent.height)*0.8

        source: (value==4) ? "qrc:/qml/Images/cp_100_pa.svg" : (value==3)
                           ? "qrc:/qml/Images/cp_16m_pa.svg" :(value==2)
                           ? "qrc:/qml/Images/cp_16m.svg" : (value==1)
                           ? "qrc:/qml/Images/cp_100.svg" : ""
        sourceSize.width:  width
        sourceSize.height: height
    }

    Connections
    {
        target: _uiCore
        function onSgSetUIParameter(nameParam, value)
        {
            if(nameParam===("type_dev"))
                main.value = value
        }
    }
}