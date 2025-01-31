import QtQuick.Controls 2.12
import Tutorials


SimpleTutorialMessage{
        buttons: Dialog.Ok

        headerText: qsTr("Noise gate")

        text: qsTr("Noise Gate is used to attenuate the signal in pauses when you don’t play guitar, helping you to hide some hiss, hum and other noise present in any guitar setup.

THRESHOLD: The Noise Gate allows the signal to pass through only when it is above the threshold.
When the input signal is below the threshold, the gate is ‘closed’ and no signal is allowed to pass.
Set it a little higher than the noise level in your system to achieve noise dampening. If the threshold is set too high, it may cut out your soft notes.

DECAY: Determines how fast the gate closes. Low values won’t let any noise to pass but may cut your notes short.");
}
