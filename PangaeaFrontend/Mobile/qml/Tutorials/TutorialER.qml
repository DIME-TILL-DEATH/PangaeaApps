import QtQuick.Controls 2.12
import Tutorials

SimpleTutorialMessage{
        buttons: Dialog.Ok

        headerText: qsTr("Early reflections")

        text: qsTr("This effect block emulates room acoustics. Unlike Reverb, Early Reflections are sounds that you hear right after the direct sound before full reverberation takes over. It’s more like a series of discrete delays rather than a smooth tail associated with Reverb.

VOLUME: Amount of reflections mixed to the direct signal.

TYPE: There are 3 types of the effect to choose from:
1. Short: Small room
2. Medium: Meduim size room
3. Long: Large room");
}
