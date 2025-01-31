import QtQuick.Controls 2.12
import Tutorials


SimpleTutorialMessage{
        buttons: Dialog.Ok

        headerText: qsTr("Tremolo")

        text: qsTr("Tremolo is a modulation effect that rhythmically changes the volume of your signal.

RATE: period of modulation.

DEPTH: how strong volume attenuates on each period.");
}
