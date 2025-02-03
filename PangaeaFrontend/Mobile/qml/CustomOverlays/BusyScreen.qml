import QtQuick
import QtQuick.Controls

import StyleSettings

import CppObjects
import PangaeaBackend

Item
{
    id : rWait

    property double  pbValue: 0

    width:  parent.width
    height: parent.height
    anchors.centerIn: parent

    visible: false


    Rectangle{
        id: background

        anchors.fill: parent
        opacity: 0.6
    }

    Column
    {
        anchors.centerIn: parent
        height: parent.height*0.8
        width: parent.width

        Item
        {
            width: parent.width
            height: parent.height*7/10
            BusyIndicator
            {
                id : bI

                width: parent.width/2
                height:parent.width/2

                anchors.centerIn: parent
            }
        }



        ProgressBar
        {
            id: progressBar

            width: parent.width/2
            height: parent.height*0.5/10
            anchors.horizontalCenter: parent.horizontalCenter

            visible: true
            value: pbValue

            background: Rectangle {
                     implicitWidth: 200
                     implicitHeight: 6
                     color: Style.colorFon
                     radius: 3

                     border.width: 1
                 }

             contentItem: Item {
                 implicitWidth: 200
                 implicitHeight: 4

                 Rectangle {
                     width: progressBar.visualPosition * parent.width
                     height: parent.height
                     radius: 2
                     color: Style.currentTheme.colorModulOn
                     border.width: 1
                }
            }
        }

        Text
        {
            id: txt

            width: parent.width
            height: parent.height*1/10

            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignBottom

            font.pixelSize: parent.width*0.05
            font.bold: true

            anchors.horizontalCenter: parent.horizontalCenter

            visible: true //txtVisible

            text: ""

            color: Style.colorFon
        }

    }

    MouseArea{
        id: mWait
        anchors.fill: parent
    }

    Connections
    {
        target: UiCore

        function onSgSetProgress(val)
        {
            rWait.pbValue = val;
        }

        function onSgSetUIParameter(nameParam, inValue)
        {
            if(nameParam === ("wait"))
            {
                rWait.visible = inValue;

                switch(UiCore.currentDevice.presetManager.currentState)
                {
                    case PresetState.UploadingIr:
                    {
                        txt.text = qsTr("Uploading file data to device");
                        break;
                    }

                    default: txt.text = qsTr("Sending commands to device");
                }
                progressBar.visible = true
            }
        }
    }

    Connections{
        target: UiCore.currentDevice.presetManager

        function onCurrentStateChanged()
        {
            switch(UiCore.currentDevice.presetManager.currentState)
            {
                // case PresetState.Changing:
                // {
                //     rWait.visible = inValue;
                //     txt.text = qsTr("Sending commands to device");
                //     progressBar.visible = true
                //     break;
                // }
                case PresetState.UploadingIr:
                {
                    rWait.visible = true;
                    txt.text = qsTr("Uploading file data to device");
                    progressBar.visible = true
                    break;
                }

                case PresetState.Exporting:
                case PresetState.Copying:
                {
                    rWait.visible = true;
                    txt.text = qsTr("Downloading impulse data from device");
                    progressBar.visible = true
                    break;
                }

                case PresetState.SavingIr:
                {
                    rWait.visible = true;
                    txt.text = qsTr("Applying impulse to device. Please wait...");
                    progressBar.visible = false
                    break;
                }

                // case PresetState.Idle:
                // {
                //     rWait.visible = false;
                //     break;
                // }
            }
        }
    }

    Connections{
        target: InterfaceManager

        function onSgConnectionStarted()
        {
            progressBar.visible = false;
            txt.text = qsTr("Connecting to device...");
            rWait.visible = true;
        }

        function onSgInterfaceError(errorDescription)
        {
            rWait.visible = false;
        }

        function onSgInterfaceDisconnected(deviceDescription)
        {
            rWait.visible = false;
        }
    }
}
