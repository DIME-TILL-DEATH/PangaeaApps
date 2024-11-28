import QtQuick
import QtQuick.Controls.Material

import QtQuick.Layouts
import QtQuick.Window

import StyleSettings 1.0
import Elements 1.0

import CppObjects

Dialog
{
    id: _root

    property alias text: _text.text
    property alias wrapMode: _text.wrapMode

    property alias headerText: _titleLabel.text

    property alias buttons: _dialogButtonBox.standardButtons

    modal: true
    closePolicy: Popup.NoAutoClose

    width: parent.width * 0.9
    height: (_text.textHeight+3)*_text.font.pixelSize*0.6
            +_title.height+_dialogButtonBox.height

    property int parentHeight: parent.height*0.34
    property int headerHeight: parentHeight*0.2
    property int footerHeight: parentHeight*0.25

    property bool closeOnDisconnect: false

    anchors.centerIn: Overlay.overlay
//    anchors.centerIn: parent

    background: Rectangle {
       id: _backRect

        width: _root.width
        height: _root.height

        color: Style.colorModul

        radius: Style.baseRadius
        border.color: Style.currentTheme.colorBorderOn
        border.width: 1
    }

    header: Item{
               id: _title

               width: _root.width
               height: (_titleLabel.textHeight*2) //headerHeight

               Rectangle{
                   id: _headerDivider
                   width: parent.width
                   height: 1
                   anchors.bottom: _title.bottom
                   color: Style.currentTheme.colorBorderOn
               }

               MText{
                   id: _titleLabel
                   color: Style.colorText
                   padding: _root.height/25
                   anchors.horizontalCenter: parent.horizontalCenter
                   anchors.verticalCenter: parent.verticalCenter
               }
        }


    contentItem:  Item {
           id: _textLabel

           width: _root.width
           height: _root.height-headerHeight-footerHeight

           MText{
               id: _text

               wrapMode: Text.WordWrap
               width: parent.width

               padding: 5

               color: Style.colorText
               anchors.verticalCenter: parent.verticalCenter

           }
        }

    footer: DialogButtonBox
    {
        id: _dialogButtonBox

        width: _root.width
        height: _title.height*1.5 //footerHeight

        alignment: Qt.AlignHCenter

        background: Rectangle{
            color: "transparent"
            radius: Style.baseRadius
            border.color: Style.currentTheme.colorBorderOn
            border.width: 1
        }
    }

    Connections
    {
        target: InterfaceManager

        function onSgInterfaceDisconnected(deviceDescription)
        {
                if(closeOnDisconnect)
                    _root.close();
        }
    }
}
