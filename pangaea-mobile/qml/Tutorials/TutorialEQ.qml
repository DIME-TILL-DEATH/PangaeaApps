import QtQuick.Controls 2.12
import Tutorials

SimpleTutorialMessage{
        buttons: Dialog.Ok

        headerText: qsTr("Parametric EQ")

        text: qsTr("This effect block provides 5-band parametric equalizer for fine-tuning your tone.

Parameters for each band:

FREQUENCY: Central frequency of the band.

GAIN: Amplification/attenuation factor of the band.

Q(quality): Bandwidth - the area around the set frequency that the EQ will amplify or attenuate.");
}
