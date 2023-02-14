pragma Singleton

import QtQuick
import QtQuick.Controls

QtObject {
    id: root

    property bool modulesRightAligned: true

    readonly property color backgroundColor: "#EBECEC"
    readonly property color mainEnabledColor: "#5E5971"
    readonly property color mainDisabledColor: "#7E7991"

    readonly property color highlightColor: "Salmon"
    readonly property color headColor: "MediumSeaGreen"
}
