import QtQuick
import QtQuick.Controls

import Qt5Compat.GraphicalEffects

import StyleSettings
import Elements
import Tutorials

import CustomOverlays
import CppObjects
import PangaeaBackend

Item
{
    id: _main
    property bool findBT: true
    property bool isConnected: false

    Column
    {
        id: _column

        width: parent.width*0.98
        height: parent.height
        anchors.horizontalCenter: parent.horizontalCenter

        spacing: height/65

        Rectangle{
            id: _scanContainerRect

            width: parent.width
            height: parent.height*1/15//-parent.spacing*3

            color: "blue"
            border.width: 1
            border.color:  Style.currentTheme.colorBorderOn
            radius: Style.baseRadius

            SequentialAnimation on color{
                running: !isConnected
                loops: Animation.Infinite
                alwaysRunToEnd: true
                ColorAnimation {from: Style.colorModul; to: "gray"; duration: 1000}
                ColorAnimation {from: "gray"; to: Style.colorModul; duration: 1000}
            }

            Item{
                id: _imgContainer

                width:  _scanContainerRect.height*0.6
                height: _scanContainerRect.height*0.6
                anchors.right: _textColumn.left
                anchors.rightMargin: _scanContainerRect.height/10
                anchors.verticalCenter: parent.verticalCenter
                Image
                {
                    id: _scanImg
                    source: "qrc:/images/Arrow Gear new.svg"

                    anchors.fill: parent

                    visible: !isConnected

                    fillMode: Image.PreserveAspectFit
                    transformOrigin: Item.Center
                }
                Image
                {
                    id: _connectedImg
                    source: "qrc:/images/Connected.svg"

                    anchors.fill: parent

                    visible: isConnected

                    fillMode: Image.PreserveAspectFit
                    transformOrigin: Item.Center
                }

                SequentialAnimation on rotation{
                    running: !isConnected
                    loops: Animation.Infinite
                    alwaysRunToEnd: true
                    NumberAnimation {
                        from: 0
                        to: 360
                        duration: 1500
                    }
                }
            }

            Column{
                id: _textColumn

                x: (_scanContainerRect.width - width + _imgContainer.width)/2


                width: (_mainText.textWidth > _auxText.textWidth) ?
                           _mainText.textWidth : _auxText.textWidth

                anchors.verticalCenter: parent.verticalCenter
                MText
                {
                    id: _mainText
                    anchors.horizontalCenter: parent.horizontalCenter
                    color: Style.colorText
                    text: isConnected ? qsTr("CONNECTED") : qsTr("SCANNING...")
                }
                MText
                {
                    id: _auxText
                    anchors.horizontalCenter: parent.horizontalCenter
                    color: Style.colorText
                    font.pixelSize: _mainText.font.pixelSize*0.8
                    text: isConnected ? qsTr("(TAP to disconnect)") : ""
                    visible: isConnected
                }
            }

            MouseArea{
                anchors.fill: parent
                onClicked: {
                    if(isConnected)
                    {
                        messageAcceptDisconnect.open()
                    }
                }
            }
        }

        Item{
            width: parent.width
            height: parent.height*0.2/10
            MText{
                color: Style.colorText
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter

                text: isConnected ? qsTr("All avaliable AMT PANGAEA devices:")
                                    : qsTr("TAP TWICE to connect your AMT PANGAEA:")
            }
        }

        Rectangle{
            width: parent.width
            height: parent.height*6.6/10

            gradient: Gradient{
                GradientStop{
                    position: 0.0
                    color: Style.colorModul
                }
                GradientStop{position: 1.0; color: "black"}
            }

            border.width: 1
            border.color: Style.currentTheme.colorBorderOn
            radius: Style.baseRadius

            ListView
            {
                id: _list

                width: parent.width-50
                height: parent.height*0.95

                property int autoSelectedItem

                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter

                enabled: !isConnected

                currentIndex: (autoSelectedItem<count) ? autoSelectedItem : "0"

                focus: true

                model: DevicesListModel
                delegate: BleListDelegate{}

                highlight: BleListHighlight{
                    iconWidth: _scanContainerRect.height*0.6
                    iconHeight: _scanContainerRect.height*0.6
                    iconVisible: isConnected
                }

                // Transitions
                add: Transition {
                    NumberAnimation { properties: "y"; from: _list.height; duration: 250 }
                }

                displaced: Transition{
                    NumberAnimation { properties: "y"; duration: 300 }
                }

                remove: Transition {
                    NumberAnimation { properties: "x"; to: _list.width; duration: 250 }
                }
            }

            MouseArea{
                anchors.fill: parent

                visible: isConnected

                onClicked: {
                    if(isConnected)
                    {
                        messageAcceptDisconnect.open()
                    }
                }
            }
        }

        Rectangle{
            id: _boxAuto

            width: parent.width
            height: parent.height*1/10 - _column.spacing

            border.color: Style.currentTheme.colorBorderOn
            border.width: 1
            radius: Style.baseRadius

            gradient: Gradient{
                GradientStop{ position: 0.0; color: Style.colorModul}
                GradientStop{ position: 1.0; color: "black"}
            }

            CheckBox{
                id: _checkBox

                x: _boxAuto.width/2 - _contentText.contentWidth - _checkBox.indicator.width/2

                anchors.verticalCenter: parent.verticalCenter

                contentItem: MText{
                    id: _contentText

                    color: Style.colorText
                    text: qsTr("Autoconnect")
                    anchors.left: _checkBox.indicator.right
                    verticalAlignment: Text.AlignVCenter
                    leftPadding: _checkBox.indicator.width/5
                }
                onCheckStateChanged: {
                    UiCore.saveSetting("autoconnect_enable", _checkBox.checked);
                }
            }

            MouseArea{
                anchors.fill: parent
                onClicked: {
                    _checkBox.checked = !_checkBox.checked
                }
            }
        }

        Rectangle{
            id: _boxOffline

            width: parent.width
            height: parent.height*1/10 - _column.spacing

            border.color: Style.currentTheme.colorBorderOn
            border.width: 1
            radius: Style.baseRadius

            gradient: Gradient{
                GradientStop{ position: 0.0;color: Style.colorModul}
                GradientStop{position: 1.0; color: "black"}
            }

            MText{
                anchors.centerIn: parent
                color: Style.colorText
                text: qsTr("Show offline(virtual) devices")
            }

            MouseArea{
                anchors.fill: parent

                onClicked: {
                    _tutorialVirtualDevice.process()
                }
            }
        }
    }

    TutorialVirtualDevice{
        id: _tutorialVirtualDevice
    }


    CustomMessageDialog
    {
        id: messageAcceptDisconnect

        headerText: qsTr("Continue?")
        text: qsTr("Do you want to disconnect from AMT PANGAEA?")
        standardButtons: Dialog.Ok  | Dialog.Cancel

        onAccepted:
        {
            UiCore.disconnectFromDevice();
            isConnected = false;
        }
    }

    Connections
    {
        target: UiCore

        function onSgSetUIParameter(nameParam, inValue)
        {
            if(nameParam === "autoconnect_enable")
            {
                _checkBox.checked = inValue;
            }
        }
    }


    Connections
    {
        target: InterfaceManager

        function onSgInterfaceConnected(deviceDescription)
        {
            isConnected = true;
        }

        function onSgInterfaceDisconnected()
        {
            isConnected = false;
            InterfaceManager.startScanning(DeviceConnectionType.BLE);
        }
    }
}
