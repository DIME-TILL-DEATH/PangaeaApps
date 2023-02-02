import QtQuick 2.15
import QtQuick.Controls 2.12

import StyleSettings 1.0

ComboBox {
    id: _root

    model: ["PHONES", "LINE", "BALANCED"]

//    font.bold: true
//    font.pixelSize: _root.width*0.125

    property int delegateHeight: font.pixelSize*4

    delegate: ItemDelegate {
        id: _delegate

        width: _root.width
        height: delegateHeight

        contentItem: MText {
            text: modelData
            color: Style.currentTheme.colorTextEnabled
//            font: _root.font
            elide: Text.ElideRight
            verticalAlignment: Text.AlignVCenter
        }
        highlighted: _root.highlightedIndex === index

        background: Rectangle {
                 implicitWidth: 100
                 implicitHeight: 20
                 radius: _root.width/20

                 opacity: enabled ? 1 : 0.3
                 color: (highlighted) ? Style.colorTextHighLight : Style.colorModul
        }
    }

    indicator: Canvas {
        id: canvas
        x: _root.width - width - _root.rightPadding
        y: _root.topPadding + (_root.availableHeight - height) / 2
        width: _root.width/10
        height: _root.height/3
        contextType: "2d"

        Connections {
            target: _root
            function onPressedChanged() { canvas.requestPaint(); }
        }

        onPaint: {
            context.reset();
            context.moveTo(0, 0);
            context.lineTo(width, 0);
            context.lineTo(width / 2, height);
            context.closePath();
            context.fillStyle = _root.pressed ? Style.colorText : Style.currentTheme.colorTextEnabled;
            context.fill();
        }
    }

    contentItem: MText {
        leftPadding: _root.width/10
        rightPadding: _root.indicator.width + _root.spacing

        text: _root.displayText
//        font: _root.font

        color: _root.pressed ? Style.colorText : Style.currentTheme.colorTextEnabled
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
    }

    background: Rectangle {
        implicitWidth: 120
        implicitHeight: 20
        radius: _root.width/10

        color: Style.colorModul

        border.color: _root.pressed ? Style.colorText : Style.currentTheme.colorTextEnabled
        border.width: _root.visualFocus ? 2 : 1
    }

    popup: Popup {
        y: _root.height
        width: _root.width

        height: delegateHeight * 3 + 2 //border * 2

        implicitHeight: contentItem.implicitHeight
        padding: 1

        contentItem: ListView {
            clip: true
            implicitHeight: contentHeight
            model: _root.popup.visible ? _root.delegateModel : null
            currentIndex: _root.highlightedIndex

            ScrollIndicator.vertical: ScrollIndicator { }
        }

        background: Rectangle {
            color: Style.colorModul
            border.color: Style.currentTheme.colorBorderOn
            radius: _root.width/20
        }
    }
}
