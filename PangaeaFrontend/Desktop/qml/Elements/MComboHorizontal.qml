import QtQuick
import QtQuick.Controls

import Elements

Row{
    id: _root

    property alias text: _label.text
    property alias currentIndex: _combo.currentIndex
    property alias model: _combo.model

    signal activated(index: int)

    MLabel{
        id: _label
        width: parent.width/2
        // height: parent.height
        anchors.verticalCenter: parent.verticalCenter
    }

    ComboBox{
        id: _combo

        width: parent.width/2
        // height: parent.height
        anchors.verticalCenter: parent.verticalCenter

        onActivated: (index) => {
            _root.activated(index);
        }
    }

}
