import QtQuick 2.15
import QtQuick.Controls.Material

import PangaeaFrontend
import PangaeaBackend

Item {
    id: _root

    anchors.fill: parent

    function process()
    {
        _tutorialDialog.open()
    }

    SimpleTutorialMessage
    {
        id: _tutorialDialog

        buttons: Dialog.Ok

        text: qsTr("This option appends virtual(offline) devices to the list. These devices completely simulate the interaction of the application with a real Pangea device.
On the phone/tablet, along the path AMT/pangae-mobile/, a directory is created in which the file structure corresponding to the real device. After editing offline, you can copy it to your device.
(And vice versa, by copying files from the device to the desired folders, you can view all the presets).");

        headerText: qsTr("Virtual device")
        onAccepted: {
            InterfaceManager.startScanning(DeviceConnectionType.Offline);
        }
    }

}
