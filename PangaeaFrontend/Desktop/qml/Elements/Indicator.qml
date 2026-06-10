import QtQuick
import QtQuick.Layouts
import StyleSettings

import Qt5Compat.GraphicalEffects

Item {
    id: root

    property url sourceImage
    property string indicatorText
    property bool isOk
    
    implicitHeight: 40
    implicitWidth: 300

    Row {
        anchors.fill: parent
        spacing: 8

        Item {
            width: root.height
            height: root.height

            Image {
                id: image
                source: root.sourceImage
                anchors.fill: parent
                fillMode: Image.PreserveAspectFit
                transformOrigin: Item.Center
            }

            ColorOverlay {
                anchors.fill: image
                source: image
                color: Style.currentTheme.textEnabled
            }
        }

        Rectangle {
            id: indicator
            width: root.height
            height: root.height
            radius: width / 2

            border.width: 2
            border.color: Style.currentTheme.backgroundColor

            color: root.isOk ? "green" : "red"
        }

        Item {
            width: parent.width - root.height * 2 - 16
            height: root.height

            MText {
                anchors.verticalCenter: parent.verticalCenter
                verticalAlignment: Text.AlignVCenter
                text: root.indicatorText
            }
        }
    }
}
