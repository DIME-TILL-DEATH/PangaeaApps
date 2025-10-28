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

    property alias checkBoxText: _checkBox.text
    property alias checkBoxChecked: _checkBox.checked
    property alias checkBoxVisible: _checkBox.visible

    modal: true
    closePolicy: Popup.NoAutoClose

    width: parent.width * 0.9
    height: (_text.textHeight+3)*_text.font.pixelSize*0.6
            +_title.height+_dialogButtonBox.height

    property int parentHeight: parent.height*0.34
    property int headerHeight: _title.height
    property int footerHeight: _dialogButtonBox.height

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


    contentItem:  Column {
           id: _textLabel

           width: _root.width
           height: _root.height-_root.headerHeight-_root.footerHeight

           MText{
               id: _text

               wrapMode: Text.WordWrap
               width: parent.width

               padding: 5

               color: Style.colorText

           }

           CheckBox{
               id: _checkBox
               anchors.horizontalCenter: parent.horizontalCenter

               visible: false
               checked: true

               contentItem: MText{
                   id: _checkBoxText

                   text: _checkBox.text

                   color: Style.colorText
                   anchors.left: _checkBox.indicator.right
                   verticalAlignment: Text.AlignVCenter
                   leftPadding: _checkBox.indicator.width/5
               }
           }
        }

    footer: DialogButtonBox
    {
        id: _dialogButtonBox

        width: _root.width
        height: _title.height*2 //footerHeight

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
