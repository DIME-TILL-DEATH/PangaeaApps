import QtQuick 2.15
import QtQuick.Controls 2.15

import Elements
import StyleSettings

Column{
    id: _root

    height: parent.height
    width: parent.width / 8

    property alias text: _textLabel.text
    property alias currentIndex: _combo.currentIndex
    property alias model: _combo.model
    property alias comboText: _combo.currentText

    signal activated(index: int)

    MLabel{
        id: _textLabel

        height: parent.height/3
        width: parent.width

        text: "Controller"
        font.pixelSize: 5 * Style.dip
        font.bold: true

        elide: Text.ElideMiddle

        color: Style.textEnabled

        horizontalAlignment: TextInput.AlignHCenter
        verticalAlignment: TextInput.AlignVCenter
    }

    ComboBox
    {
        id: _combo


        height: parent.height / 3
        width: parent.width * 0.9

        anchors.horizontalCenter: parent.horizontalCenter

        currentIndex: 0

        onActivated: (index) => {
            _root.activated(index);
        }
    }
}
