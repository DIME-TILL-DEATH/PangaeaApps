import QtQuick
import QtQuick.Controls

import StyleSettings

Rectangle {
    id: root

    anchors.centerIn: parent

    color: Style.mainEnabledColor

    width: parent.width/2
    height: parent.height

    radius: width/20
    border.width: 2
}
