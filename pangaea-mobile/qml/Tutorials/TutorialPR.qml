import QtQuick.Controls 2.12
import Tutorials

SimpleTutorialMessage{
        buttons: Dialog.Ok

        headerText: qsTr("Preamp")

        text: qsTr("Represents EQ response of guitar preamp.

LOW, MID, HIGH: Corresponding frequency bands.

VOLUME: Output signal level.");

}
