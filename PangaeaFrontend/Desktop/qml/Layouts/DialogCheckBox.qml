import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs

import StyleSettings
import Elements

Dialog {
    id: _root

    standardButtons: Dialog.Ok | Dialog.Cancel

    property alias text: _textMain.text
    property alias chkBoxText: _chkBox.text
    property alias checked: _chkBox.checked

    modal: true
    Overlay.modal: Rectangle {
        color: "gray"
        opacity: 0.5
    }

    background: Rectangle{
        width: _root.width
        height: _root.height

        radius: 4

        color: Style.currentTheme.mainEnabledColor
    }

    contentItem: Item{
        width: _root.width
        height: _root.height

        Column{
            width: parent.width * 0.8
            height: parent.height * 0.9
            anchors.centerIn: parent

            MText{
                id: _textMain

                width: parent.width

                horizontalAlignment: Text.AlignHCenter

                wrapMode: Text.WordWrap

                font.bold: false

                color: Style.currentTheme.textMain

                anchors.horizontalCenter: parent.horizontalCenter
                anchors.leftMargin: width/20
                anchors.rightMargin: width/20
            }

            MCheckBox{
                id: _chkBox

                anchors.horizontalCenter: parent.horizontalCenter

                checked: true
            }
        }
    }

    footer: DialogButtonBox{
        id: dialogButtonBox

        standardButtons: DialogButtonBox.Yes | DialogButtonBox.No

        width: _root.width
        height: _root.height * 1/5

        // buttonLayout: DialogButtonBox.WinLayout

        background: Rectangle{
            // width: dialogButtonBox.width
            // height: dialogButtonBox.height

            color: Style.currentTheme.backgroundColor
        }

        delegate: Button{
            id: _btnDelegate

            implicitWidth: _root.width / 5
            implicitHeight: 20

            // width: dialogButtonBox.width/5
            height: dialogButtonBox.height * 0.8

            opacity: _btnDelegate.down ? 0.3 : 1

            background: Rectangle{
                anchors.fill: parent
                // border.width: 1
                border.color: Style.currentTheme.borderOff

                radius: _btnDelegate.height/2
                color: Style.currentTheme.backgroundColor
                clip: true
            }

            contentItem: Text{
                text: _btnDelegate.text

                horizontalAlignment: Text.AlignHCenter
                verticalAlignment:   Text.AlignVCenter

                color: Style.currentTheme.textInverted
            }
        }

        onAccepted: {
            _root.accepted()
            _root.close()
        }

        onRejected: {
            _root.rejected()
            _root.close()
        }
    }
}
