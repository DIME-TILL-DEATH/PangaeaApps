import QtQuick

QtObject {
    property string chTutorial: qsTr("Chorus is a type of guitar effect that belongs to the Modulation family of effects. It emulates the sound of multiple instruments playing together at the same time which can cause micro shifts in pitch and timing. Chorus works by duplicating your guitar signal and slightly detuning one side of it.

RATE: rate controls the speed of the LFO that modulates the chorus delay time. Remember: the processed signal in a chorus is constantly shifting in time (and pitch) thanks to this undulating LFO. With a lower rate, the change happens slowly, creating a gentle swaying feel. Faster rates create a more noticeable warbling effect.

WIDTH: in mono processing 'width' is using to alterate type of chorus processing.

HPF: high pass filter for processed signal.

MIX: proportion of dry and wet signal.");
    //----------------------------------------------------------------------------------------------------------
    property string cmTutorial: qsTr("Compressor reduces the volume of loud sounds and amplifies quiet sounds smoothening the dynamics. At the same time, it can be used as a sustainer making your single notes sound longer.
It’s a great tool to letting some parts stand out. It is as easy to find settings that work against you. Too much compression may bring up some hum and noise when used together with overdrive or distortion.

SUSTAIN: Combination of threshold and release parameters. the higher the value, the longer the notes will decay.

VOLUME: Sets the output volume. It can be used to compensate for signal attenuation caused by compression.");
    //----------------------------------------------------------------------------------------------------------
    property string prTutorial: qsTr("Represents EQ response of guitar preamp.

LOW, MID, HIGH: Corresponding frequency bands.

VOLUME: Output signal level.");
    //----------------------------------------------------------------------------------------------------------
    property string phTutorial: qsTr("Phasers generate a sound that’s full of movement and intensity. Generally, phase shifters often evoke a feeling of the signal being sent to space, orbiting around some imaginary planet trapped inside the device and returning to the listener at a customizable BPM.

RATE: rate controls the speed of the LFO that modulates the chorus delay time. Remember: the processed signal in a chorus is constantly shifting in time (and pitch) thanks to this undulating LFO. With a lower rate, the change happens slowly, creating a gentle swaying feel. Faster rates create a more noticeable warbling effect.

WIDTH, CENTER: in mono processing 'width' and 'center' is using to alterate type of phaser processing.

FEEDBACK: will control the amount of processed signal that is added back into the Dimension module.

STAGES: how many 'notches' is used to process signal.

MIX: proportion of dry and wet signal.");
    //----------------------------------------------------------------------------------------------------------
    property string paTutorial: qsTr("Module reproduce the compression, saturation and playing feel of a cranked up tube power amp. Alse represents frequency characteristic.

MASTER: Think of it as a Master volume of an amp. As you turn it up, you’ll notice how the sound gets more and more saturated and compressed. Just like with a real power amp, you can get some overdrive when Volume is set to maximum.

PRESENCE: Just like a real power amp this effect block has a Presence control. Use it to add some extra brightness to the tone.

LEVEL: When you crank up the Volume, use this parameter to attenuate the level to avoid clipping. If Volume is low, turn LEVRL up to boost the output.

TYPE: Model of output section");
    //----------------------------------------------------------------------------------------------------------
    property string ngTutorial: qsTr("Noise Gate is used to attenuate the signal in pauses when you don’t play guitar, helping you to hide some hiss, hum and other noise present in any guitar setup.

THRESHOLD: The Noise Gate allows the signal to pass through only when it is above the threshold.
When the input signal is below the threshold, the gate is ‘closed’ and no signal is allowed to pass.
Set it a little higher than the noise level in your system to achieve noise dampening. If the threshold is set too high, it may cut out your soft notes.

DECAY: Determines how fast the gate closes. Low values won’t let any noise to pass but may cut your notes short.");
    //----------------------------------------------------------------------------------------------------------
    property string irTutorial: qsTr("Impulse Response or IR is a sonic fingerprint of a certain speaker, cabinet, mic, mic preamp and room where the profile was captured.");
    //----------------------------------------------------------------------------------------------------------
    property string erTutorial: qsTr("This effect block emulates room acoustics. Unlike Reverb, Early Reflections are sounds that you hear right after the direct sound before full reverberation takes over. It’s more like a series of discrete delays rather than a smooth tail associated with Reverb.

VOLUME: Amount of reflections mixed to the direct signal.

TYPE: There are 3 types of the effect to choose from:
1. Short: Small room
2. Medium: Meduim size room
3. Long: Large room");
    //----------------------------------------------------------------------------------------------------------
    property string eqTutorial: qsTr("This effect block provides 5-band parametric equalizer for fine-tuning your tone.

Parameters for each band:

FREQUENCY: Central frequency of the band.

GAIN: Amplification/attenuation factor of the band.

Q(quality): Bandwidth - the area around the set frequency that the EQ will amplify or attenuate.");
    //----------------------------------------------------------------------------------------------------------
    property string dlTutorial: qsTr("Delay records a portion of the audio signal coming from your instrument and plays it back multiple times after a predetermined time. This creates an echo-like effect, with the delayed sound typically being quieter than the original signal and decaying and fading out over time.

MIX: volume of resulting delayed signal.

TIME: controls the amount of time it takes for the delayed sound to kick in after you play a note.

FEEDBACK controls the number of times the delayed sound will repeat.  Lower settings result in fewer repeats.

HPF, LPF: lets you adjust the frequency range and shape the tone of the delayed sound.
");
    //----------------------------------------------------------------------------------------------------------
    property string trTutorial: qsTr("Tremolo is a modulation effect that rhythmically changes the volume of your signal.

RATE: period of modulation.

DEPTH: how strong volume attenuates on each period.");
}
