import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs

Dialog {
    id: _root

    standardButtons: Dialog.Ok | Dialog.Cancel

    property alias label: _textLabel.text
    property alias text: _textInput.text

    header: Item{
        height: _root.height

        Column{
            width: parent.width * 0.75
            height: parent.height * 0.5
            anchors.centerIn: parent

            Text{
                id: _textLabel

                text: qsTr("Enter folder name")

                horizontalAlignment: Text.AlignHCenter

                width: parent.width
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.leftMargin: width/20
                anchors.rightMargin: width/20
            }

            TextField{
                id: _textInput

                text: "NewFolder"

                anchors.horizontalCenter: parent.horizontalCenter
            }
        }
    }

    footer: DialogButtonBox{

    }
}
