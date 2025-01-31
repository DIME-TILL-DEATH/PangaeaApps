import QtQuick.Controls 2.12
import Tutorials


SimpleTutorialMessage{
        buttons: Dialog.Ok

        headerText: qsTr("Phaser")

        text: qsTr("Phasers generate a sound that’s full of movement and intensity. Generally, phase shifters often evoke a feeling of the signal being sent to space, orbiting around some imaginary planet trapped inside the device and returning to the listener at a customizable BPM.

RATE: rate controls the speed of the LFO that modulates the chorus delay time. Remember: the processed signal in a chorus is constantly shifting in time (and pitch) thanks to this undulating LFO. With a lower rate, the change happens slowly, creating a gentle swaying feel. Faster rates create a more noticeable warbling effect.

WIDTH, CENTER: in mono processing 'width' and 'center' is using to alterate type of phaser processing.

FEEDBACK: will control the amount of processed signal that is added back into the Dimension module.

STAGES: how many 'notches' is used to process signal.

MIX: proportion of dry and wet signal.");
}
