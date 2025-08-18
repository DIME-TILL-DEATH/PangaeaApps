import QtQuick
import QtQuick.Layouts
import StyleSettings

import Qt5Compat.GraphicalEffects

RowLayout{
    id: root

    property url sourceImage
    property string indicatorText
    property bool isOk    

    Item{
        Layout.preferredHeight: root.height
        Layout.preferredWidth: height
        Image
        {
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

    Rectangle{
        id: indicator
        Layout.preferredHeight: root.height
        Layout.preferredWidth: height
        radius: width

        border.width: 2
        border.color: Style.currentTheme.backgroundColor

        color: root.isOk ? "green" : "red"
    }
    Item{
        Layout.preferredHeight: root.height
        Layout.preferredWidth: root.width * 0.6
        MText{
            anchors.verticalCenter: parent.verticalCenter
            verticalAlignment: Text.AlignVCenter
            text: root.indicatorText
        }
    }
}
