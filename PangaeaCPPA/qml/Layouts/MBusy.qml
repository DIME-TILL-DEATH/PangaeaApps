import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import CppObjects 1.0

Item
{
    id : rWait

    anchors.fill: parent

    visible: false

    property double  pbValue: 0
    property string strSearching: qsTr("Loading application")

    Rectangle{
        id: background

        anchors.fill: parent
        opacity: 0.5
    }

    MouseArea{
        // block any mouse events
        anchors.fill: parent
    }

    ColumnLayout
    {
        id: column

        width:  parent.width/5
        height: parent.height*0.8
        anchors.centerIn: parent
        BusyIndicator
        {
            id : bI

            visible: rWait.visible
            running: rWait.visible // if always running, FilaDialog lags in Linux

            Layout.preferredWidth: parent.width/3
            Layout.preferredHeight: width
            Layout.fillWidth: true
        }

        ProgressBar
        {
            id: progressBar

            Layout.preferredWidth: parent.width
            Layout.preferredHeight: parent.height/8
            Layout.fillWidth: true

            visible: false

            value: pbValue

            background: Rectangle {
                     implicitWidth: 200
                     implicitHeight: 6
                     color: "black" //Style.colorFon
                     radius: 3

                     border.width: 1
                     border.color: "grey"
                 }

             contentItem:    Rectangle {
                     width: progressBar.visualPosition * parent.width
                     height: parent.height
                     radius: 3
                     color: "MediumSeaGreen" //Style.currentTheme.colorModulOn
                     border.width: 1
                }
        }

        Item {
            Layout.preferredWidth: parent.width
            Layout.preferredHeight: parent.height/4
            Layout.fillWidth: true
            Text{
                id: txt

                anchors.fill: parent

                text: rWait.strSearching

                wrapMode: Text.WordWrap
                horizontalAlignment: Text.AlignHCenter
            }
        }
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
                txt.text = qsTr("Sending commands to device");
                progressBar.visible = true
            }

            if(nameParam === ("data_uploading"))
            {
                rWait.visible = inValue;
                txt.text = qsTr("Uploading file data to device");
                progressBar.visible = true
            }

            if(nameParam === ("ir_downloading"))
            {
                rWait.visible = inValue;
                txt.text = qsTr("Downloading impulse data from device");
                progressBar.visible = true
            }

            if(nameParam === ("ir_upload_finished"))
            {
                rWait.visible = true;
                txt.text = qsTr("Applying impulse to device. Please wait...");
                progressBar.visible = false
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
