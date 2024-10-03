import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Window 2.0
//import QtQuick.Dialogs 1.3

import StyleSettings 1.0
import Elements 1.0

import CppObjects

Item
{
    id: main

    property bool on: true
    property int paramType: DeviceParameter.CABINET_ENABLE

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
                        UiCore.setImpuls("");
                    }
                }
            }
        }
    }

    CustomMessageDialog{
        id: _msgNotSupportedIrFormat

        closeOnDisconnect: true

        headerText: qsTr("Not supported format")

        buttons: Dialog.Yes | Dialog.No

        onAccepted:
        {
            UiCore.convertAndUploadImpulse("");
        }
    }

    Connections
    {
        target: UiCore
        function onSgSetUIText(nameParam, value)
        {
            if( nameParam === "impulse_name" )
            {
                _baseModule.impulseName = (value==="") ? qsTr("empty") : value;
            }

            if(nameParam === "not_supported_ir")
            {
                _msgNotSupportedIrFormat.text = qsTr("Pangaea doesn't support this wav format:") + "\n" +
                                     value + "\n" +
                                     qsTr("Do you want to convert it before upload?")
                _msgNotSupportedIrFormat.open();
            }
        }

        function onSgSetUiDeviceParameter(paramType, value)
        {
            if(paramType === main.paramType)
            {
                main.on=value
            }
        }
    }

    Connections{
        target: _baseModule
        function onSgModuleOnOf()
        {
            main.on = (!main.on);
            UiCore.setDeviceParameter(main.paramType, main.on)
        }
    }
}
