pragma Singleton

import QtQuick
import QtQuick.Controls
import QtCore

import QtQuick.Controls.Material 2.12

QtObject {
    id: root

    property Theme currentTheme : themeDarkBlue

    property Theme themeClassicBlue: Theme{
        backgroundColor: "#EBECEC"
        mainEnabledColor: "#5E5971"
        mainDisabledColor: "#7E7991"

        highlightColor: "Salmon"
        headColor: "MediumSeaGreen"

        textMain: "#EBECEC"
        textSecondary: "lightsteelblue"
        textInverted: "black"
        textCombo: "black"

        textEnabled: "#EBECEC"
        textDisabled: "black"

        borderOn: "white"
        borderOff: "grey"

        barLow: "grey"
        barHigh: "steelblue"
        barText: "white"

        error: "red"
        spin: "red"
    }

    property Theme themeDarkOrange: Theme{
        backgroundColor: "#2B2A29"

        mainEnabledColor: "#5B5B5B"
        mainDisabledColor: "#404040"

        highlightColor: "sienna"
        headColor: "darkolivegreen"

        textMain: "burlywood"
        textSecondary: "lightsteelblue"
        textInverted: "burlywood"
        textCombo: "burlywood"

        textEnabled: "burlywood"
        textDisabled: "grey"

        borderOn: "burlywood"
        borderOff: "darkslategrey"

        barLow: "grey"
        barHigh: "darkorange"
        barText: "palegoldenrod"

        error: "red"
        spin: "orange"
    }

    property Theme themeDarkBlue: Theme{
        backgroundColor: "#2B2A29"

        mainEnabledColor: "#5B5B5B"
        mainDisabledColor: "#404040"

        highlightColor: "darkblue"
        headColor: "darkcyan"

        textMain: "#EBECEC"
        textSecondary: "burlywood"
        textInverted: "deepskyblue"
        textCombo: "deepskyblue"

        textEnabled: "#2a8ada"
        textDisabled: "grey"

        borderOn: "deepskyblue"
        borderOff: "darkslategrey"

        barLow: "grey"
        barHigh: "#769897"
        barText: "#EBECEC"

        error: "red"
        spin: "deepskyblue"
    }

    property Theme themeDarkGreen: Theme{
        backgroundColor: "#2B2A29"

        mainEnabledColor: "#5B5B5B"
        mainDisabledColor: "#404040"

        highlightColor: "orange"
        headColor: "olive"

        textMain: "#EBECEC"
        textSecondary: "lightsteelblue"
        textInverted: "teal"
        textCombo: "#00D046"

        textEnabled: "#00D046"
        textDisabled: "grey"

        borderOn: "#FFF005"
        borderOff: "darkslategrey"

        barLow: "grey"
        barHigh: "#46A847"
        barText: "#EBECEC"

        error: "red"
        spin: "green"
    }
}
