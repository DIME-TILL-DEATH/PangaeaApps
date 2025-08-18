import QtQuick 2.15
import QtQuick.Controls

import StyleSettings

Label{
    font.bold: true
    font.family: "Arial Black"
    color: Style.currentTheme.textMain

    elide: Label.ElideRight
}
