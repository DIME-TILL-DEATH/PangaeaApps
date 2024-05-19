import QtQuick
import QtQuick.Controls

import QtQuick.Templates as T
import QtQuick.Controls.Material
import QtQuick.Controls.Material.impl

import StyleSettings 1.0
import Elements 1.0

ComboBox
{
    id: control
    property bool on: true

    topInset: 2
    bottomInset: 2

    indicator: ColorImage {
        x: control.width - width - control.padding
        y: control.topPadding + (control.availableHeight - height) / 2
        color: control.on ? Style.colorText : Style.currentTheme.colorTextDisabled
        source: "qrc:/qt-project.org/imports/QtQuick/Controls/Material/images/drop-indicator.png"
    }

    contentItem: MText{
        text: control.displayText
        color: on ? Style.colorText : Style.currentTheme.colorTextDisabled

        leftPadding: 12

        verticalAlignment: Text.AlignVCenter
    }

    background: Rectangle {
        implicitWidth: 120
        implicitHeight: control.Material.buttonHeight

        radius: control.flat ? 0 : 2
        color: Style.colorFon

        layer.enabled: control.enabled && control.Material.background.a > 0
        layer.effect: ElevationEffect {
            elevation: control.Material.elevation
        }

        Rectangle {
            y: parent.y + control.baselineOffset
            width: parent.width
            height: control.activeFocus ? 2 : 1
            color: control.activeFocus ? control.Material.accentColor : control.Material.hintTextColor
        }

        Ripple {
            clip: control.flat
            clipRadius: control.flat ? 0 : 2
            x: control.indicator ? control.indicator.x : 0
            width: control.indicator ? control.indicator.width : parent.width
            height: parent.height
            pressed: control.pressed
            anchor: control.indicator ? control.indicator : control
            active: enabled && (control.pressed || control.visualFocus || control.hovered)
            color: control.Material.rippleColor
        }
    }

    delegate: MenuItem {
        width: ListView.view.width

        font.pixelSize: 10 * Style.dip
        font.bold: true

        text: control.textRole ? (Array.isArray(control.model) ? modelData[control.textRole] : model[control.textRole]) : modelData
        Material.foreground: control.currentIndex === index ? ListView.view.contentItem.Material.accent : ListView.view.contentItem.Material.foreground
        highlighted: control.highlightedIndex === index
        hoverEnabled: control.hoverEnabled
    }

    popup: T.Popup {
        y: control.height - 5
        width: control.width
        height: Math.min(contentItem.implicitHeight, control.Window.height - topMargin - bottomMargin)
        transformOrigin: Item.Top
        topMargin: 12
        bottomMargin: 12

        Material.theme: control.Material.theme
        Material.accent: control.Material.accent
        Material.primary: control.Material.primary

        enter: Transition {
            // grow_fade_in
            NumberAnimation { property: "scale"; from: 0.9; easing.type: Easing.OutQuint; duration: 220 }
            NumberAnimation { property: "opacity"; from: 0.0; easing.type: Easing.OutCubic; duration: 150 }
        }

        exit: Transition {
            // shrink_fade_out
            NumberAnimation { property: "scale"; to: 0.9; easing.type: Easing.OutQuint; duration: 220 }
            NumberAnimation { property: "opacity"; to: 0.0; easing.type: Easing.OutCubic; duration: 150 }
        }

        contentItem: ListView {
            clip: true
            implicitHeight: contentHeight
            model: control.delegateModel
            currentIndex: control.highlightedIndex
            highlightMoveDuration: 0

            T.ScrollIndicator.vertical: ScrollIndicator { }
        }

        background: Rectangle {
            radius: 2
            color: parent.Material.dialogColor

            layer.enabled: control.enabled
            layer.effect: ElevationEffect {
                elevation: 8
            }
        }
    }
}
