import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.3
//import QtQuick.Dialogs 1.3

import CustomOverlays 1.0
import Elements 1.0
import StyleSettings 1.0

import CppObjects

CustomMessageDialog
{
    id: _root

    property alias contentText: _textField.text

    required property ControlValue controlValue

    property int precision: 0

    standardButtons: Dialog.Ok  | Dialog.Cancel

    closeOnDisconnect: true

    headerText: qsTr("Fine value")
    text: qsTr("Enter value in range: " + controlValue.minDisplayValue + "-" + controlValue.maxDisplayValue)
    contentText: controlValue.displayValue.toFixed(precision)

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

                   inputMethodHints: Qt.ImhFormattedNumbersOnly

                   color: acceptableInput ? Style.colorText : "red"

                   validator: DoubleValidator{
                       bottom: controlValue.minDisplayValue
                       top: controlValue.maxDisplayValue
                       decimals: precision

                       locale: "en"
                   }

                   // на яндекс клавиатуре не выдаёт сигнал
                   // onAccepted: {
                   //  _root.accept();
                   // }

                   onEditingFinished: {
                     _root.accept();
                   }
               }
           }
    }

    onOpened: {
        contentText = controlValue.displayValue.toFixed(precision);
        _textField.focus = true
        _textField.selectAll();
    }

    onAccepted: {
        if(parseFloat(contentText) > controlValue.maxDisplayValue) contentText = controlValue.maxDisplayValue
        if(parseFloat(contentText) < controlValue.minDisplayValue) contentText = controlValue.minDisplayValue

        controlValue.displayValue = contentText;
        controlValue.isModified = true;
    }
    onRejected:
    {
        contentText = controlValue.displayValue.toFixed(precision)
    }
}
