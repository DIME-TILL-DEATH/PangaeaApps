import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.impl 2.15
import QtQuick.Layouts 1.15
import QtQuick.Window 2.15
import QtQuick.Dialogs
import QtCore

import StyleSettings 1.0

import Elements 1.0

import CppObjects
import PangaeaBackend

Window{
    id: _root

    width: 300
    height: 500

    property string dstIrPath: (_bar.currentIndex === 0) ? "ir_library/"
                                                         : "bank_" + UiCore.currentDevice.bank + "/preset_" + UiCore.currentDevice.preset+ "/"

    // signal uploadFileReq();


    title: qsTr("IR management")

    Rectangle{

        width: parent.width
        height: parent.height

        enabled: UiCore.currentDevice.presetManager.currentState === PresetState.Idle
        opacity: enabled ? 1 : 0.5

        color: Style.currentTheme.mainEnabledColor

        Column{
            width: parent.width*0.95
            height: parent.height// - _dialogButtonBox.height

            anchors.horizontalCenter: parent.horizontalCenter

            spacing: parent.height * 0.01

            Button{
                width: parent.width * 0.9
                height: parent.height * 0.07

                anchors.horizontalCenter: parent.horizontalCenter

                text: qsTr("UPLOAD IR")

                onClicked: {
                    // uploadFileReq();
                    irFileDialog.open();
                }
            }

            ListView{
                id: _irListView

                width: parent.width * 0.9
                height: parent.height * 0.7

                anchors.horizontalCenter: parent.horizontalCenter

                model: (_bar.currentIndex === 0) ? UiCore.currentDevice.irsInLibrary
                                                 : UiCore.currentDevice.irsInFolder

                boundsBehavior: Flickable.StopAtBounds
                // clip: true

                ScrollBar.vertical: ScrollBar{
                    active: true
                    visible: true
                }

                spacing: 4

                snapMode: ListView.SnapToItem

                delegate: Item{
                    id: _item

                    width: _irListView.width
                    height: _irListView.height/25

                    property var irFile: modelData

                    MText{
                        text: modelData.irName

                        anchors.fill: parent

                        font.pixelSize: _item.height

                        color: isCurrentIr() ? Style.currentTheme.highlightColor : Style.currentTheme.textEnabled

                        elide: Text.ElideMiddle

                        horizontalAlignment: Qt.AlignHCenter
                        verticalAlignment: Qt.AlignVCenter
                    }

                    MouseArea{
                        id: _mouseAreaDrag

                        anchors.fill: parent

                        drag.target: _item
                        drag.axis: Drag.XAndYAxis
                        // drag.minimumY: 2
                        // drag.maximumY: _listConfig.y + _listConfig.height + _rectDelete.height

                        onClicked: {
                            UiCore.currentDevice.currentIrFile = modelData
                        }

                        onReleased: _item.Drag.drop()
                    }

                    Drag.active: _mouseAreaDrag.drag.active
                    Drag.source: _item
                    Drag.hotSpot.x: width / 2
                    Drag.hotSpot.y: height / 2

                    states: State {
                            when: _mouseAreaDrag.drag.active
                            ParentChange {
                                target: _item
                                parent: _irListView
                            }

                            AnchorChanges {
                                target: _item
                                anchors.horizontalCenter: undefined
                                anchors.verticalCenter: undefined
                            }
                        }

                    function isCurrentIr()
                    {
                        if((_bar.currentIndex === 1) & UiCore.currentDevice.currentIrFile.irLinkPath === "")
                        {
                            return (UiCore.currentDevice.currentIrFile.irName.indexOf(modelData.irName) !== -1)
                        }
                        else
                        {
                            return (UiCore.currentDevice.currentIrFile.irName === modelData.irName) & (UiCore.currentDevice.currentIrFile.irLinkPath === modelData.irLinkPath)
                        }
                    }
                }
            }

            Rectangle{
                width: parent.width
                height: _root.height * 0.07

                border.width: 1
                border.color: Style.currentTheme.borderOn

                color: _dropDelete.containsDrag ? "#60FFFFFF" : "transparent"

                ColorImage
                {
                    id: _scanImg
                    source: "qrc:/Images/bin.svg"

                    anchors.centerIn: parent

                    height: parent.height * 0.5

                    fillMode: Image.PreserveAspectFit

                    color: Style.currentTheme.borderOn
                }

                DropArea{
                    id: _dropDelete

                    anchors.fill: parent

                    onDropped: function (drag){
                        UiCore.currentDevice.deleteIrFile((drag.source as Item).irFile);
                    }
                }
            }

            TabBar{
                id: _bar

                width: parent.width
                height: parent.height * 0.07

                currentIndex: 0

                TabButton {
                    height: parent.height
                    anchors.top: parent.top
                    text: qsTr("Library")
                 }
                 TabButton {
                     height: parent.height
                     anchors.top: parent.top
                     text: qsTr("Preset folder")
                 }
            }
        }
    }

    FileDialog
    {
        id: irFileDialog

        title: qsTr("Select IR")
        nameFilters: [ "Wav files (*.wav)" ]

        onAccepted:
        {
            // TODO: переделать cleanPath на QUrl
            var cleanPath = irFileDialog.currentFile.toString();
            cleanPath = (Qt.platform.os==="windows")?decodeURIComponent(cleanPath.replace(/^(file:\/{3})|(qrc:\/{2})|(http:\/{2})/,"")):decodeURIComponent(cleanPath.replace(/^(file:\/{2})|(qrc:\/{2})|(http:\/{2})/,""));

            UiCore.currentDevice.escImpulse();
            UiCore.uploadIr(cleanPath, _irManagerWindow.dstIrPath);

        }

        onRejected:
        {
            if(InterfaceManager.connectedInterface.connectionType === DeviceConnectionType.USB)
                UiCore.currentDevice.escImpulse()
        }

        onSelectedFileChanged:
        {
            // TODO: переделать cleanPath на QUrl
            var cleanPath = irFileDialog.currentFile.toString();
            cleanPath = (Qt.platform.os==="windows")?decodeURIComponent(cleanPath.replace(/^(file:\/{3})|(qrc:\/{2})|(http:\/{2})/,"")):decodeURIComponent(cleanPath.replace(/^(file:\/{2})|(qrc:\/{2})|(http:\/{2})/,""));

            if(InterfaceManager.connectedInterface.connectionType === DeviceConnectionType.USB){
                UiCore.currentDevice.previewIr(cleanPath);
            }
        }

        Settings
        {
            category: "Current_folder"
            property alias curFolder: irFileDialog.currentFolder
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
                var cleanPath = irFileDialog.currentFile.toString();
                cleanPath = (Qt.platform.os==="windows")?decodeURIComponent(cleanPath.replace(/^(file:\/{3})|(qrc:\/{2})|(http:\/{2})/,"")):decodeURIComponent(cleanPath.replace(/^(file:\/{2})|(qrc:\/{2})|(http:\/{2})/,""));
                UiCore.convertAndUploadIr(cleanPath, _root.dstIrPath);
                break;
            }
        }
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

    MessageDialog
    {
        id: _msgInfo

        title: qsTr("File already on device")
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

                case DeviceErrorType.FileExists:
                {
                    _msgInfo.text = qsTr("File ") + params + qsTr(" already on device.")
                    _msgInfo.open();
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
