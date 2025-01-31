import QtQuick.Controls 2.12
import Tutorials

SimpleTutorialMessage{
        buttons: Dialog.Ok

        headerText: qsTr("Power amp")

        text: qsTr("Module reproduce the compression, saturation and playing feel of a cranked up tube power amp. Alse represents frequency characteristic.

MASTER: Think of it as a Master volume of an amp. As you turn it up, you’ll notice how the sound gets more and more saturated and compressed. Just like with a real power amp, you can get some overdrive when Volume is set to maximum.

PRESENCE: Just like a real power amp this effect block has a Presence control. Use it to add some extra brightness to the tone.

LEVEL: When you crank up the Volume, use this parameter to attenuate the level to avoid clipping. If Volume is low, turn LEVRL up to boost the output.

TYPE: Model of output section");
}
