import QtQuick 2.15
import QtQuick.Controls 2.12
import QtQuick.Window 2.0

import StyleSettings 1.0

import CppObjects

ToolBar
{
    id: _root

    property string devName: ""
    property string version: Qt.application.version//UiCore.appVersion()
    property string firmwareVersion: ""

    property string devNameStr: devName==""?"":devName
    property string versionStr: version==""?"":"/Application ver: " + version
    property string firmwareVersionStr: firmwareVersion=="" ? "":" Firmware ver." + firmwareVersion

//    property string  poVersion: "Pangaea " + devNameStr + versionStr + firmwareVersionStr
    property string  poVersion: "Pangaea " + devNameStr + firmwareVersionStr + versionStr

    height:  Screen.desktopAvailableHeight / 75

    signal menuClicked() // Сигнал, который сообщит о клике по кнопке меню

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
        function onSgSetUIParameter(nameParam, inValue)
        {
            // TODO: свести все названия в одно место class Device/DeviceDescription
            if(nameParam === ("type_dev"))
            {
                switch (inValue)
                {
                case 0: devName = "";  break;
                case 1: devName = "CP-100";  break;
                case 2: devName = "CP-16M Blue";  break;
                case 3: devName = "CP-16M-PA Green"; break;
                case 4: devName = "CP-100PA"; break;
                }
            }
        }

        function onSgSetUIText(nameParam, value)
        {
            if(nameParam === ("devVersion"))
            {
                firmwareVersion = value;
            }
        }
    }
}
