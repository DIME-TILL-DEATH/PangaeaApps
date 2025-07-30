import QtQuick
import QtQuick.Controls

import Qt5Compat.GraphicalEffects

import Elements
import StyleSettings

Rectangle{
    id: _root
    implicitWidth: 100
    implicitHeight: 50

    color: "transparent"

    property alias imageSource: _img.source
    property alias imageColor: _overlay.color

    signal clicked();

    opacity: _ma.pressed ? 0.3 : 1.0

    Image{
        id: _img

        anchors.centerIn: parent
        width: parent.width
        height: parent.height

        fillMode: Image.PreserveAspectFit
        smooth: true


        sourceSize.width:  width
        sourceSize.height: height


        MouseArea{
            id: _ma
            anchors.fill: parent

            onClicked: {
                _root.clicked();
            }
        }
    }
    ColorOverlay{
        id: _overlay

        anchors.fill: _img
        source: _img
        color: Style.mainEnabledColor
    }
}
