import QtQuick
import QtQuick.Layouts
import StyleSettings

RowLayout{
    id: root

    property string indicatorText
    property bool isOk

    Rectangle{
        id: indicator
        Layout.preferredHeight: root.height
        Layout.preferredWidth: height
        radius: width

        border.width: 2
        border.color: Style.backgroundColor

        color: root.isOk ? "green" : "red"
    }
    Item{
        Layout.preferredHeight: root.height
        Layout.preferredWidth: root.width * 0.6
        MText{
    //        anchors.left: indicator.right
            anchors.verticalCenter: parent.verticalCenter
            verticalAlignment: Text.AlignVCenter
            text: root.indicatorText
        }
    }
}
