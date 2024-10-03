import QtQuick 2.15
import QtQuick.Controls 2.12
import QtQuick.Window 2.0

import StyleSettings 1.0

import CppObjects
import CppEnums

ToolBar
{
    id: _root

    property string firmwareVersion: ""

    property string versionStr: Qt.application.version == "" ? "" : "Application ver: " + Qt.application.version
    property string firmwareVersionStr: firmwareVersion == "" ? "":" Firmware ver." + firmwareVersion

    property string  poVersion: (connected ? "Pangaea " +  (DeviceProperties.firmwareName + firmwareVersionStr + "/") : "")
                                + versionStr

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


    Connections
    {
        target: UiCore

        function onSgSetUIText(nameParam, value)
        {
            if(nameParam === ("devVersion"))
            {
                firmwareVersion = value;
            }
        }
    }
}
