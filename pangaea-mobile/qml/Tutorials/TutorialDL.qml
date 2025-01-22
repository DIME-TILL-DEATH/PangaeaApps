import QtQuick.Controls 2.12
import Tutorials


SimpleTutorialMessage{
        buttons: Dialog.Ok

        headerText: qsTr("Delay")

        text: qsTr("Delay records a portion of the audio signal coming from your instrument and plays it back multiple times after a predetermined time. This creates an echo-like effect, with the delayed sound typically being quieter than the original signal and decaying and fading out over time.

MIX: volume of resulting delayed signal.

TIME: controls the amount of time it takes for the delayed sound to kick in after you play a note.

FEEDBACK controls the number of times the delayed sound will repeat.  Lower settings result in fewer repeats.

HPF, LPF: lets you adjust the frequency range and shape the tone of the delayed sound.
");
}
