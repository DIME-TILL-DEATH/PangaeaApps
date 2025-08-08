import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import StyleSettings

import Elements

import CP100FX
import CppObjects
import PangaeaBackend

Window{
    id: _root

    width: Screen.width * 0.2
    height: width * 0.75

    minimumWidth: Screen.width * 0.3 * 1
    minimumHeight: Screen.width * 0.3 * 0.5 * 0.75
    maximumWidth: Screen.width * 0.3 * 1
    maximumHeight: Screen.width * 0.3 * 0.5 * 2

    color: Style.mainEnabledColor

    property alias currentCabNum: _chooseCabCombo.currentIndex

    signal cabNumChanged(var cabNum)

    ColumnLayout{
        anchors.fill: parent

        Row{
            // Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            Layout.preferredHeight: parent.height/14
            Layout.fillWidth: true
            Layout.leftMargin: width/25
            Layout.rightMargin: width/20

            MText{
                id: _curFolderHeader

                width: contentWidth
                height: parent.height

                text: qsTr("Current folder: ")

                verticalAlignment: Text.AlignVCenter

                visible: UiCore.currentDevice.systemSettings.cabNumber === 2
            }

            MText{
                width: parent.width - _curFolderHeader.width
                height: parent.height

                text: UiCore.currentDevice.fileBrowser.currentFolder

                elide: Text.ElideMiddle

                verticalAlignment: Text.AlignVCenter
            }
        }

        MComboHorizontal{
            id: _chooseCabCombo

            visible: UiCore.currentDevice.systemSettings.cabNumber === 2

            Layout.preferredHeight: parent.height/14
            Layout.preferredWidth: parent.width/3
            Layout.leftMargin: width/10
            Layout.rightMargin: width/20

            text: "Cab num:"

            model: ["1", "2"]

            onActivated: {
                _root.cabNumChanged(_chooseCabCombo.currentIndex)
            }
        }

        Rectangle{
            Layout.fillWidth: true
            Layout.preferredHeight: 2
        }

        ListView{
            id: _fsObjectList


            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.leftMargin: width/20
            Layout.rightMargin: width/20

            model: UiCore.currentDevice.fileBrowser

            clip: true

            ScrollBar.vertical: ScrollBar{
                policy: _fsObjectList.contentHeight > _fsObjectList.height ?
                            ScrollBar.AlwaysOn : ScrollBar.AlwaysOff
                }

            delegate: Item{
                width: _fsObjectList.width
                height: _nameText.font.pixelSize * 1.5 //_fsObjectList.height/10

                MText{
                    id: _nameText

                    text: name

                    width: parent.width

                    color: type === FileBrowserModel.Dir ? "lightsteelblue"
                                : _chooseCabCombo.currentIndex === 0 ?
                                    (UiCore.currentDevice.ir1Name === name ? Style.highlightColor : Style.textEnabled ) :
                                    (UiCore.currentDevice.ir2Name === name ? Style.highlightColor : Style.textEnabled )
                    elide: Text.ElideMiddle
                }

                MouseArea{
                    anchors.fill: parent

                    onClicked: {
                        UiCore.currentDevice.selectFsObject(name, type, _chooseCabCombo.currentIndex);
                    }
                }
            }
        }
        Rectangle{
            Layout.fillWidth: true
            Layout.preferredHeight: 2
        }

        Row{
            Layout.preferredWidth: parent.width
            Layout.preferredHeight: parent.height/10

            MButton{
                width: parent.width/3
                height: parent.height

                text: qsTr("Upload IR")
            }

            Item{
                width: parent.width/3
                height: parent.height
            }

            MButton{
                width: parent.width/3
                height: parent.height

                text: qsTr("New folder")
            }
        }
    }
}
