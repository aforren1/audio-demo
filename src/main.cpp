#include <Arduino.h>
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SerialFlash.h>
#include <cstring>
#include <array>

AudioSynthWaveformSine sine0; // base frequency, always 165 Hz
AudioSynthWaveformSine sine1; // F1 (decreasing), middle freq of 650 Hz (min 400, max 900 over 0.4 meters)
AudioSynthWaveformSine sine2; // F2 (increasing), middle freq of 1350 Hz (min 1100, max 1600 over 0.4 meters)

AudioMixer4 mixer0;
// so the relation is 1250Hz/meter.
// We'll take the input as meters relative to the center
// The original (van Vugt & Ostry 2017) paper
AudioOutputI2S i2s1;
// play in both ears. We *could* play cursor & target in different ears?
AudioConnection patchCord1(sine0, 0, mixer0, 0);
AudioConnection patchCord2(sine1, 0, mixer0, 1);
AudioConnection patchCord3(sine2, 0, mixer0, 2);
// connection to output
AudioConnection patchCord4(mixer0, 0, i2s1, 0);
AudioConnection patchCord5(mixer0, 0, i2s1, 1);
AudioControlSGTL5000 sgtl5000_1;

void setup()
{
    AudioMemory(30);
    sgtl5000_1.enable();
    sgtl5000_1.volume(0.3);

    sine0.frequency(165.0);
    sine1.frequency(500.0);
    sine2.frequency(1450.0);
    // normalize to prevent clipping (though we don't control for perceived loudness yet, unlike the original)
    sine0.amplitude(1.0 / 3.0);
    sine1.amplitude(1.0 / 3.0);
    sine2.amplitude(1.0 / 3.0);
}

// position in meters, where (0, 0) is the center of the screen
// +x will decrease frequency of sine1,
// +y will increase frequency of sine2
float mouse_x, mouse_y;

std::array<uint8_t, 64> input{}; // data from computer

void loop()
{
    int n = RawHID.recv(input.data(), 0);
    if (n > 0)
    {
        // two floats representing meters
        auto begin = input.begin();
        std::memcpy(&mouse_x, begin, sizeof(mouse_x));
        std::memcpy(&mouse_y, begin + sizeof(mouse_x), sizeof(mouse_y));
        AudioNoInterrupts();
        sine1.frequency(-mouse_x * 1250.0 + 650.0);
        sine2.frequency(mouse_y * 1250.0 + 1350.0);
        AudioInterrupts();
    }
}
