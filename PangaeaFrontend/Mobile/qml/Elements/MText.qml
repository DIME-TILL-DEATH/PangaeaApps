import QtQuick 2.15
import QtQuick.Controls 2.12

import StyleSettings 1.0

Text{
    id: _root

    font.pixelSize: 10 * Style.dip
    font.bold: true

    property alias textWidth: _tMetrics.boundingRect.width
    property alias textHeight: _tMetrics.boundingRect.height // _tMetrics.height

    TextMetrics{
        id: _tMetrics

        font: _root.font
        text: _root.text
        elide: _root.elide
    }
}

