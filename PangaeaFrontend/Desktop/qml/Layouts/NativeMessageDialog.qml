import QtQuick 2.15
import QtQuick.Controls

import Elements
import StyleSettings

Window{
    id: root

    width: Screen.width / 4
    height: Screen.height / 8

    modality: Qt.ApplicationModal

    flags: Qt.Dialog | Qt.WindowStaysOnTopHintS

    color: Style.currentTheme.mainEnabledColor

    property alias text: _mText.text
    property alias buttons: dialogButtonBox.standardButtons

    function open() {
        show()
    }

    signal buttonClicked(var button)
    signal accepted()
    signal rejected()

    onVisibleChanged: {
        if(visible) {
            root.raise();
            root.requestActivate();
        }
    }

    Column{
        anchors.fill: parent

        Item{
            width: parent.width
            height: parent.height * 3/4

            MText{
                id: _mText

                font.bold: false

                anchors.fill: parent

                anchors.topMargin: 10
                anchors.bottomMargin: 10
                anchors.leftMargin: 20
                anchors.rightMargin: 20

                wrapMode: Text.WrapAtWordBoundaryOrAnywhere

                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter

                color: Style.currentTheme.textMain
            }
        }

        DialogButtonBox{
            id: dialogButtonBox

            standardButtons: DialogButtonBox.Ok

            width: parent.width
            height: parent.height * 1/5

            // buttonLayout: DialogButtonBox.WinLayout

            background: Rectangle{
                // width: dialogButtonBox.width
                // height: dialogButtonBox.height

                color: Style.currentTheme.backgroundColor
            }

            delegate: Button{
                id: _btnDelegate

                implicitWidth: root.width / 5
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

            onClicked: function(button){
                root.buttonClicked(button)
                root.close()
            }

            onAccepted: {
                root.accepted()
                root.close()
            }

            onRejected: {
                root.rejected()
                root.close()
            }
        }
    }
}
