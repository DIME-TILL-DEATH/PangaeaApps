import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Window 2.0
//import QtQuick.Dialogs 1.3

import StyleSettings 1.0
import Elements 1.0

Item
{
    id: main

    property bool on: true
    property string nameValue: "cabinet_enable"

    BaseModule{
        id: _baseModule

        property string impulseName: "FILENAME.WAV"
        property int textWidth: _baseModule.width

        moduleName: qsTr("IR")
        moduleDescription: qsTr("Impulse response")

        showDescription: false

        contentItem:Item{
            MText{
                id: _impulseName

                width: parent.width

                text: _baseModule.impulseName
                color: on ? Style.colorText : Style.currentTheme.colorTextDisabled

                elide: Text.ElideMiddle

                horizontalAlignment: Qt.AlignHCenter
                verticalAlignment: Qt.AlignVCenter

                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter

                MouseArea
                {
                    anchors.fill: parent
                    onClicked:
                    {
                        _uiCore.setImpuls("");
                    }
                }
            }
        }
    }

    CustomMessageDialog
    {
        id: _msgNotSupportedIrFormat

        buttons: Dialog.Ok
        closeOnDisconnect: true

        text: qsTr("Not supported IR Format!\nPANGAEA supports only 48kHz/24bits/1channel files");

        headerText: qsTr("Not supported format")
    }

    Connections
    {
        target: _uiCore
        function onSgSetUIText(nameParam, nameValue)
        {
            if( nameParam === "impulse_name" )
            {
                _baseModule.impulseName = nameValue;

            }

            if(nameParam === "not_supported_ir")
            {
                _msgNotSupportedIrFormat.open();
            }
        }

        function onSgSetUIParameter(nameParam, nameValue)
        {
            if((nameParam === main.nameValue))
            {
                main.on=nameValue

            }
        }
    }

    Connections{
        target: _baseModule
        function onSgModuleOnOf()
        {
            main.on = (!main.on);
        }
    }
}
