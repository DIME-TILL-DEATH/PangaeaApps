import QtQuick 2.15
import QtQuick.Controls 2.12
import QtQuick.Window 2.0

import StyleSettings 1.0

import CppObjects
import PangaeaBackend

ToolBar
{
    id: _root

    property string appVersionStr: Qt.application.version == "" ? "" : "Application ver: " + Qt.application.version
    property string  poVersion: (connected ? UiCore.currentDevice.firmwareName : "") + "/" + appVersionStr

    height:  Screen.desktopAvailableHeight / 75

    signal menuClicked()

    Rectangle
    {
        anchors.fill: parent
        color: Style.colorFon

        Row
        {
            anchors.fill: parent

            Item
            {
                height: parent.height
                width: parent.width / 100 * 5
            }

            Item
            {
                height: parent.height
                width: parent.width / 100 * 95
                Text
                {
                    anchors.fill: parent
                    anchors.verticalCenter: parent.verticalCenter
                    text: poVersion
                    font.pixelSize: _root.height *0.7
                    color: Style.colorText
                }
            }
        }
    }
}
