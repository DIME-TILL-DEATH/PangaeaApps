import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Window 2.15

import StyleSettings 1.0

import CustomOverlays 1.0
import ControlGroups 1.0
import Modules 1.0
import Elements 1.0

import CppObjects

CustomMessageDialog {
    id: _root

    property int topHeaderSize: 0

    closeOnDisconnect: true

    width: parent.width*0.9
    height: parent.height-topHeaderSize

    anchors.centerIn: null

    x: parent.width/2-width/2
    y: parent.height-height

    background: Rectangle {
       id: _backRect

        width: _root.width
        height: _root.height

        gradient: Gradient{
            GradientStop{position: 0.0; color: Style.colorModul}
            GradientStop{position: 1.0; color: "black"}
        }

        radius: Style.baseRadius
        border.color: Style.currentTheme.colorBorderOn
        border.width: 1
    }

    headerText: qsTr("Documentation")

    footer: Rectangle {
        id: _dialogButtonBox

        width: _root.width
        height: parentHeight*0.2
        color: "transparent"
        radius: Style.baseRadius
        border.color: Style.currentTheme.colorBorderOn
        border.width: 1

        MButton{
            textButton: qsTr("BACK")

            width: parent.width*0.3
            height: parent.height*0.7

            anchors.centerIn: parent

            onMbPressed: {
                _root.close()
            }
        }
    }

    contentItem: ColumnLayout
    {
        anchors.fill: parent
        anchors.topMargin: parent.height/10
        anchors.bottomMargin: parent.height/10

        MButton
        {
            Layout.preferredWidth: parent.width*2/3
            Layout.preferredHeight: parent.height*3/20
            Layout.alignment: Qt.AlignHCenter

            //: Button text. Format device
            textButton: qsTr("Web site")
            onMbPressed:
            {
               Qt.openUrlExternally("https://amtelectronics.com/");
            }
        }

        MButton
        {
            Layout.preferredWidth: parent.width*2/3
            Layout.preferredHeight: parent.height*3/20
            Layout.alignment: Qt.AlignHCenter

            //: Button text. Format device
            textButton: qsTr("Media center")
            onMbPressed:
            {
               Qt.openUrlExternally("https://media.amt-sales.com/cat/cab-emulation/pangaea-series/");
            }
        }

        MButton
        {
            Layout.preferredWidth: parent.width*2/3
            Layout.preferredHeight: parent.height*3/20
            Layout.alignment: Qt.AlignHCenter

            //: Button text. Format device
            textButton: qsTr("Application manual")
            onMbPressed:
            {
               UiCore.openManualExternally("pangaea-mobile-app_user-manual");
            }
        }

        MButton
        {
            Layout.preferredWidth: parent.width*2/3
            Layout.preferredHeight: parent.height*3/20
            Layout.alignment: Qt.AlignHCenter

            //: Button text. Format device
            textButton: qsTr("Device manual")
            onMbPressed:
            {
                UiCore.openManualExternally("pangaea-VC-16-user-manual");
            }
        }
    }

}
