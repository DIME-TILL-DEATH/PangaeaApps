import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.3
//import QtQuick.Dialogs 1.3

import Elements 1.0
import StyleSettings 1.0

CustomMessageDialog
{
    id: _root

    property alias contentText: _textField.text

    standardButtons: Dialog.Ok  | Dialog.Cancel

    closeOnDisconnect: true

    contentItem:  Item {
           id: _content

           width: _root.width
           height: _root.height*0.5

           Column{
               spacing: parent.height/10
               anchors.centerIn: parent

               width: _root.width * 0.8
               MText{
                   id: _textLabel

                   text: _root.text
                   wrapMode: Text.WordWrap

                   width: parent.width
                   color: Style.colorText
               }
               TextField{
                   id: _textField

                   width: parent.width
               }
           }
    }
}
