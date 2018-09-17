#include <Arduino.h>
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SerialFlash.h>
#include <cstring>
#include <array>

AudioSynthWaveformSine sine1;
AudioSynthWaveformSine sine2;
//AudioSynthWaveformModulated waveformMod1;
AudioOutputI2S i2s1;
AudioConnection patchCord1(sine1, 0, i2s1, 0);
AudioConnection patchCord2(sine2, 0, i2s1, 1);
//AudioConnection patchCord2(sine1, 0, waveformMod1, 0);
//AudioConnection patchCord3(sine2, 0, waveformMod1, 1);
//AudioConnection PatchCord4(waveformMod1, 0, i2s1, 0);
AudioControlSGTL5000 sgtl5000_1;

void setup()
{
    Serial.begin(9600);
    AudioMemory(30);
    sgtl5000_1.enable();
    sgtl5000_1.volume(0.2);
    //waveformMod1.frequency(261.63);
    //waveformMod1.amplitude(1.0);
    sine1.frequency(100);
    sine2.frequency(200);
    sine1.amplitude(1.0);
    sine2.amplitude(1.0);
    //waveformMod1.begin(WAVEFORM_SINE);
}

uint16_t mouse_x, mouse_y;

std::array<uint8_t, 64> input{}; // data from computer

void loop()
{
    int n = RawHID.recv(input.data(), 0);
    if (n > 0)
    {
        // two unsigned ints from python
        auto begin = input.begin();
        std::memcpy(&mouse_x, begin, sizeof(mouse_x));
        std::memcpy(&mouse_y, begin + sizeof(mouse_x), sizeof(mouse_y));
        // Serial.print(mouse_x);
        // Serial.print(" ");
        // Serial.println(mouse_y);
        AudioNoInterrupts();
        sine1.frequency(mouse_x);
        sine2.frequency(mouse_y);
        AudioInterrupts();
    }
}