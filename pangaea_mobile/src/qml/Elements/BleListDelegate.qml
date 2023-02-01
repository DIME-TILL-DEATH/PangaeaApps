import QtQuick 2.15
import QtQuick.Controls 2.12

import StyleSettings 1.0
import Elements 1.0

Component
{
    Item
    {
        width: _list.width
        height: 50

        property string name: modelData.split("\n",2)[0]
        property string address: modelData.split("\n",2)[1]

        MText
        {
            color: "white"
            width: parent.width-40
            leftPadding: parent.width/20
            anchors.verticalCenter: parent.verticalCenter

            text: name + "\n" + address// modelData //name
        }


        MouseArea
        {
            anchors.fill: parent
            z: 1
            propagateComposedEvents: true

            onClicked:
            {
                _list.autoSelectedItem = index
                console.log("click, current index:", _list.currentIndex)
            }
            onDoubleClicked:
            {
                _list.autoSelectedItem = index
                _uiCore.doConnect(index, address)
            }
        }
    }
}
