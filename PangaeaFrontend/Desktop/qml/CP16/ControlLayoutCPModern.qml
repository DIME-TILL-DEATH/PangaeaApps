import QtQuick 2.15
import QtQuick.Window 2.15

import PangaeaFrontend
import PangaeaBackend

Column
{
    id: mainUi
    anchors.fill: parent
    focus: true
    spacing: 2

    ModulesConfigWindow{
        id: _modulesConfigWindow

    }

    IrManagerWindow{
        id: _irManagerWindow
        x: mainUi.Window.window.x + mainUi.width
        y: mainUi.Window.window.y

        height: mainUi.height
    }

    Loader{
        id: _headLoader

        source: UiCore.currentDevice.deviceType === DeviceType.LA3 ?
                    "HeadLA3.qml" : "HeadCPModern.qml"

        onLoaded: {
            _headLoader.item.width = Qt.binding(function() { return parent.width })
            _headLoader.item.height = Qt.binding(function() { return parent.height/1000*150 })
        }

        Connections{
            target: _headLoader.item

            function onOpenIrManagerWindow(){
                _irManagerWindow.show();
            }

            function onOpenModulesConfigWindow(){
                _modulesConfigWindow.show();
            }
        }
    }

    ModulesListModern{
        id: _mainRow

        width: parent.width
        height: parent.height/1000*850
    }
}
