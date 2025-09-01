import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import QtCore

import StyleSettings

import Elements
import Layouts

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

    color: Style.currentTheme.mainEnabledColor

    title: qsTr("Impulse response")

    property alias currentCabNum: _chooseCabCombo.currentIndex

    signal cabNumChanged(var cabNum)

    DialogTextInput{
        id: _folderNameDialog

        title: qsTr("Folder name")

        anchors.centerIn: Overlay.overlay

        modal: true
        Overlay.modal: Rectangle {
            color: "gray"
            opacity: 0.5
        }

        width: parent.width/2
        height: parent.height/3

        onAccepted: {
            UiCore.currentDevice.createDir(text)
        }
    }

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

                    font.bold: type === FileBrowserModel.Dir

                    color: type === FileBrowserModel.Dir ? Style.currentTheme.textSecondary
                                : _chooseCabCombo.currentIndex === 0 ?
                                    (UiCore.currentDevice.ir1Name === name ? Style.currentTheme.highlightColor : Style.currentTheme.textEnabled ) :
                                    (UiCore.currentDevice.ir2Name === name ? Style.currentTheme.highlightColor : Style.currentTheme.textEnabled )
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

                onClicked: {
                    _irFileDialog.open();
                }
            }

            Item{
                width: parent.width/3
                height: parent.height
            }

            MButton{
                width: parent.width/3
                height: parent.height

                text: qsTr("New folder")

                onClicked: {
                    _folderNameDialog.open();
                }
            }
        }
    }

    MessageDialog{
        id: msgIncorretIR

        title: qsTr("Incorrect wav format")

        buttons: MessageDialog.Yes | MessageDialog.No

        onButtonClicked: function (button, role) {
            switch(button){
            case MessageDialog.Yes:
                // TODO: переделать cleanPath на QUrl
                var cleanPath = _irFileDialog.currentFile.toString();
                cleanPath = (Qt.platform.os==="windows")?decodeURIComponent(cleanPath.replace(/^(file:\/{3})|(qrc:\/{2})|(http:\/{2})/,"")):decodeURIComponent(cleanPath.replace(/^(file:\/{2})|(qrc:\/{2})|(http:\/{2})/,""));
                UiCore.convertAndUploadIr(cleanPath);
                break;
            }
        }
    }

    MessageDialog{
        id: msgError

        title: qsTr("Error")
    }

    MessageDialog{
        id: _msgTrimFileDialog

        title: qsTr("Trim IR file")

        buttons: MessageDialog.Yes | MessageDialog.No

        property string srcPath
        property string dstPath

        onButtonClicked: function (button, role) {
            switch(button){
            case MessageDialog.Yes: {
                UiCore.currentDevice.startIrUpload(srcPath, dstPath, true);
                break;
            }

            case MessageDialog.No: {
                UiCore.currentDevice.startIrUpload(srcPath, dstPath, false);
                break;
            }
            }
        }
    }

    FileDialog
    {
        id: _irFileDialog

        title: qsTr("Select IR")
        nameFilters: [ "Wav files (*.wav)" ]

        onAccepted:
        {
            UiCore.uploadIr(_irFileDialog.currentFile);
        }

        // onRejected:
        // {
        //     if(InterfaceManager.connectedInterface.connectionType === DeviceConnectionType.USB)
        //         UiCore.currentDevice.escImpulse()
        // }

        // onSelectedFileChanged:
        // {
        //     if(InterfaceManager.connectedInterface.connectionType === DeviceConnectionType.USB){
        //         UiCore.currentDevice.previewIr(_irFileDialog.currentFile);
        //     }
        // }

        Settings
        {
            category: "Current_folder"
            property alias curFolder: _irFileDialog.currentFolder
        }
    }


    Connections{
        target: UiCore.currentDevice

        function onSgDeviceError(type, description, params)
        {
            switch(type)
            {
                case DeviceErrorType.IrFormatNotSupported:
                {
                    msgIncorretIR.text = qsTr("Pangaea doesn't support this wav format:") + "\n" +
                                         description + "\n" +
                                         qsTr("Do you want to convert it before upload?")
                    msgIncorretIR.open();
                    break;
                }

                case DeviceErrorType.NotIrFile:
                {
                    msgError.text = description;
                    msgError.open();
                    break;
                }
            }
        }
    }

    Connections
    {
        target: UiCore

        function onSgUiMessage(type, message, messageParams)
        {
            if(type === UiMessageType.PROPOSE_IR_TRIM)
            {
                _msgTrimFileDialog.text = qsTr("The length of the selected file is greater than what is used when processing the signal. Would you like to trim impulse to speed up uploading and save space in device memory?")
                _msgTrimFileDialog.srcPath = messageParams[1]
                _msgTrimFileDialog.dstPath = messageParams[2]
                _msgTrimFileDialog.open()
            }
        }

    }
}
