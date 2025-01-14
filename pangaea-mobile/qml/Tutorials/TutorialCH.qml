import QtQuick.Controls 2.12
import Tutorials


SimpleTutorialMessage{
        buttons: Dialog.Ok

        headerText: qsTr("Chorus")

        text: qsTr("Chorus is a type of guitar effect that belongs to the Modulation family of effects. It emulates the sound of multiple instruments playing together at the same time which can cause micro shifts in pitch and timing. Chorus works by duplicating your guitar signal and slightly detuning one side of it.

RATE: rate controls the speed of the LFO that modulates the chorus delay time. Remember: the processed signal in a chorus is constantly shifting in time (and pitch) thanks to this undulating LFO. With a lower rate, the change happens slowly, creating a gentle swaying feel. Faster rates create a more noticeable warbling effect.

WIDTH: in mono processing 'width' is using to alterate type of chorus processing.

HPF: high pass filter for processed signal.

MIX: proportion of dry and wet signal.");
}
