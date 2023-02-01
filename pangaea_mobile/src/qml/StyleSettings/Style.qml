pragma Singleton

import QtQuick 2.15
import QtQuick.Window 2.15

import QtQuick.Controls.Material 2.12

QtObject {

    // device independent pixel
    readonly property real dip: Screen.pixelDensity / (96 / 25.4) // DPI norm in mm

    property Theme currentTheme : themeBlue

    property Theme themeOrange: Theme{
        colorSlider: "#769897"

        colorModulOn: "darkorange"

        colorModulOff: "#0E0E0E"

        colorTextEnabled: "burlywood" //"peachpuff"
        colorTextDisabled: "#8C8C8C"
        colorTextDisabledLight: "palegoldenrod"

        colorBorderOn: "burlywood"
        colorBorderOff: "darkslategrey"

        colorLedOn: "darkorange"
        colorLedOff: "#8C8C8C"

        materialAccent: Material.Orange
    }

    property Theme themeGreen: Theme{
        colorSlider: "#769897"

        colorModulOn: "#009846"
        colorModulOff: "#0E0E0E"

        colorTextEnabled: "#00D046"
        colorTextDisabled: "#8C8C8C"
        colorTextDisabledLight: "#8CA08C"

        colorBorderOn: "#FFF005"
        colorBorderOff: "#F0F0F0"

        colorLedOn: "springgreen"
        colorLedOff: "#8C8C8C"

        materialAccent: Material.Green
    }

    property Theme themeBlue: Theme{
        colorSlider: "#769897"

        colorModulOn: "skyblue"
        colorModulOff: "#0E0E0E"

        colorTextEnabled: "#2a8ada"// "steelblue"
        colorTextDisabled: "#8C8C8C"
        colorTextDisabledLight: "lightsteelblue"

        colorBorderOn: "lightsteelblue"//"mediumspringgreen"
        colorBorderOff: "darkslategrey"

        colorLedOn: "skyblue"
        colorLedOff: "#8C8C8C"

        materialAccent: Material.Blue
    }

    readonly property color colorItemHighlight: "steelblue"


    readonly property color colorFon: "#2B2A29"
    readonly property color colorModul: "#5B5B5B"

    readonly property color colorText: "#FFFFFF"

    readonly property color colorTextHighLight: "#5050FF"

    readonly property color colorBtnEnabled: "#BABABA"
    readonly property color colorBtnDisabled: "#000000"
    readonly property color colorBtnEffect: "#BABABA"

    readonly property color devColor: "#5E5971"
    readonly property color devColorDis: "#7E7991"

    readonly property int headerFontSize: 10
    readonly property int baseRadius: 6
    readonly property int mainSpacing: 2
}
