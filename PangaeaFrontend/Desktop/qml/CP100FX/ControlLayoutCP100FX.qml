import QtQuick 2.15

import CP100FX 1.0

import CppObjects

Column
{
    id: _mainUi
    anchors.fill: parent
    focus: true
    spacing: 2


    HeadCP100FX
    {
        id: _headCP100FX

        width:  parent.width
        height: parent.height/1000*200 - _mainUi.spacing

        onSelectMenu: (menuNum) =>
        {
            _presetView.visible = !menuNum
            _systemView.visible = menuNum
        }
    }


    PresetViewFx{
        id: _presetView

        width: parent.width
        height: _mainUi.height - _headCP100FX.height
    }

    SystemViewFx{
        id: _systemView

        width: parent.width
        height: _mainUi.height - _headCP100FX.height

        visible: false
    }

}
