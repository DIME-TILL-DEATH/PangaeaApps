import QtQuick 2.15
import QtQuick.Controls 2.12

import Elements 1.0
import StyleSettings 1.0

import CustomOverlays 1.0
import CppObjects
import PangaeaBackend

CustomMessageDialog {
    id: _root

    width: parent.width * 0.9
    height: parent.height * 0.6

    property bool isOnlineUpload: false

    property string firmwareFile: "*.ble"
    property string firmwareText: "Online update"

    closeOnDisconnect: true

    headerText: qsTr("Updating firmware")
    text: qsTr("WARNING: Do not power off device or exit from application while updating firmware!")

    contentItem:  Item {
           id: _content

           width: nameDialog.width
           height: nameDialog.height*0.5

           Column{
               id: _column

               height: parent.height
               width: parent.width*0.9

               spacing: parent.height/20
               anchors.centerIn: parent

               topPadding: _column.spacing
               Item{

                   width: parent.width
                   height: parent.height*3/10
                   MText{
                       id: _firstText

                       text: _root.text

                       horizontalAlignment: Text.AlignHCenter
                       verticalAlignment: Text.AlignVCenter

                       width: parent.width
                       wrapMode: Text.WordWrap

                       color: Style.currentTheme.colorTextEnabled
                   }
               }

               Item{
                   width: parent.width
                   height: isOnlineUpload ? parent.height*2/10 : parent.height*4/10
                   MText
                   {
                       id: _secondText

                       width: parent.width
                       //height: height.parent-_auxSecondText.height*_auxSecondText.font.pixelSize*0.5

                       text: isOnlineUpload ? firmwareText : qsTr("Firmware file: ") + firmwareFile

                       horizontalAlignment: Text.AlignHCenter
                       verticalAlignment: Text.AlignVCenter

                       wrapMode: Text.WrapAnywhere

                       elide: Text.ElideRight

                       color: Style.colorText
                   }

                   MText{
                       id: _auxSecondText
                       visible: _secondText.truncated

                       text: _secondText.text
                       width: parent.width
                       //height: textHeight
                       elide: Text.ElideLeft

                       anchors.top: _secondText.bottom
                       color: Style.colorText
                   }
               }

               CustomProgressBar
               {
                   id: downloadProgressBar

                   visible: isOnlineUpload
               }


               CustomProgressBar
               {
                   id: uploadProgressBar
               }

           }
    }

    footer: Item{}

    Connections{
        target: UiCore

        function onSgSetUIParameter(nameParam, inValue)
        {
            if(nameParam===("update_firmware_mode"))
            {
                _root.isOnlineUpload = inValue;
            }
        }

        function onSgSetUIText(nameParam, value)
        {

            if(nameParam===("new_firmware_avaliable"))
            {
                var versionArray = value.split(',');
                _root.firmwareText = qsTr("Updating to firmware version ") + versionArray[1];
            }
        }

        function onSgDownloadProgress(bytesReceived, bytesTotal)
        {
            console.log("Firmware download " + bytesReceived + " from " + bytesTotal);
            downloadProgressBar.progressBarValue = (bytesReceived/bytesTotal)
            downloadProgressBar.progressBarText = Math.round(bytesReceived/1024) + qsTr("kB from ") + Math.round(bytesTotal/1024) + "kB";
        }

        function onSgSetProgress(val)
        {
            uploadProgressBar.progressBarValue = val;
        }
    }

    Connections
    {
        target: UiCore.currentDevice

        function onSgDeviceMessage(type, value, params)
        {
            switch(type)
            {
                case DeviceMessageType.FirmwareFilePath:
                {
                    console.log("firmware path:" + value)
                    _root.firmwareFile = value
                    //decodeURIComponent(value.replace(/^(file:\/{2})|(qrc:\/{2})|(content:\/{2})|(http:\/{2})|(:)/,""));
                }
            }
        }
    }
}
