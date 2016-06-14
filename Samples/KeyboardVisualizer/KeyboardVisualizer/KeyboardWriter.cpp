#include "stdafx.h"

#include "KeyboardWriter.h"

#define _USE_MATH_DEFINES
#include <cmath>
#include <math.h>

using namespace std;

// Create the writer object and intialize the keyMap.
// KeyMap is an XY lookup table that allows the correlation of the FFT data.
KeyboardWriter::KeyboardWriter() 
	: backgroundRainbow(DEFAULT_BACKGROUND_RAINBOW), backgroundRed(DEFAULT_BACKGROUND_RED), backgroundGreen(DEFAULT_BACKGROUND_GREEN), backgroundBlue(DEFAULT_BACKGROUND_BLUE),
	visualizerRed(DEFAULT_VISUALIZER_RED), visualizerGreen(DEFAULT_VISUALIZER_GREEN), visualizerBlue(DEFAULT_VISUALIZER_BLUE), 
	isRainbow(DEFAULT_IS_RAINBOW), isColorized(DEFAULT_IS_COLORIZED) {
	BuildKeyMap();
}

// Builds a keyMap with the key positionMap and key sizeMap.
// The keyMap is used to correlate the FFT data.
void KeyboardWriter::BuildKeyMap() {
    // map is only for G910
	positionMap = {
		//          0x0000, 0x0000, 0x0000, 0x0000,
		0x0001,   0x003b, 0x003c, 0x003d, 0x003e,    0x003f, 0x0040, 0x0041, 0x0042,    0x0043, 0x0044, 0x0057, 0x0058,   0x0137, 0x0046, 0x0145,  
		0x0029, 0x0002, 0x0003, 0x0004, 0x0005, 0x0006, 0x0007, 0x0008, 0x0009, 0x000a, 0x000b, 0x000c, 0x000d, 0x000e,   0x0152, 0x0147, 0x0149,   0x0045, 0x0135, 0x0037, 0x004A,
		0x000f, 0x0010, 0x0011, 0x0012, 0x0013, 0x0014, 0x0015, 0x0016, 0x0017, 0x0018, 0x0019, 0x001a, 0x001b, 0x002b,   0x0153, 0x014f, 0x0151,   0x0047, 0x0048, 0x0049, 0x004e,
		0x003a, 0x001e, 0x001f, 0x0020, 0x0021, 0x0022, 0x0023, 0x0024, 0x0025, 0x0026, 0x0027, 0x0028, 0x001c, /*001c*/                            0x004b, 0x004c, 0x004d, 0x004e,
		0x002a, /*002a*/0x002c, 0x002d, 0x002e, 0x002f, 0x0030, 0x0031, 0x0032, 0x0033, 0x0034, 0x0035, 0x0036, /*0036*/          0x0148,           0x004f, 0x0050, 0x0051, 0x011c,
		0x001d, 0x015b, 0x0038, /*0039*//*0039*//*0039*/0x0039, /*0039*//*0039*//*0039*/0x0138, 0x015c, 0x015d, 0x011d,   0x014b, 0x0150, 0x014d,   /*0052*/0x0052, 0x0053, 0x011c,
	};
	sizeMap = {
		 //      -1.75,  1,      1,      1,      1, -17.25,
		     1, -.75,  1,      1,      1,      1, -.75,   1,      1,      1,      1, -.75,   1,      1,      1,      1,  -.5,  1,      1,      1, -4.25, 
			 1,      1,      1,      1,      1,      1,      1,      1,      1,      1,      1,      1,      1,      2,  -.5,  1,      1,      1,   -.5, 1,      1,      1,      1,
		   1.5,      1,      1,      1,      1,      1,      1,      1,      1,      1,      1,      1,      1,    1.5,  -.5,  1,      1,      1,   -.5, 1,      1,      1,      1,
		  1.75,      1,      1,      1,      1,      1,      1,      1,      1,      1,      1,      1,   2.25, /*2.25*/  -4,                            1,      1,      1,      1,
		   2.5,  /*2.5*/     1,      1,      1,      1,      1,      1,      1,      1,      1,      1,    2.5, /*2.5*/ -1.5,          1,          -1.5, 1,      1,      1,      1,
		   1.5,   1.25,   1.25, /*5.75*//*5.75*//*5.75*/   5.75,/*5.75*//*5.75*//*5.75*/  1.25,   1.25,   1.25,    1.5,  -.5,  1,      1,      1,   -.5, /*2*/   2,      1,      1,
	};

	unsigned int *keys = &positionMap[0];
	float *sizes = &sizeMap[0];
	for (int y = 0; y < ROWS; y++)	{
		unsigned int key;
		int size = 0;
		for (int x = 0; x < COLS; x++) {
			if (size == 0) {
				float sizef = *sizes++;
				if (sizef < 0) {
					key = 0x0000;
					size = -sizef * 4;
				}
				else {
					key = *keys++;
					size = sizef * 4;
				}
			}
			keyMap[y][x] = key;
			size--;
		}
	}
}

// Build a looping rainbow for all keys or whatever the current configuration is set to.
void KeyboardWriter::BuildBackgroundMap(int iteration) {
	for (int x = 0; x < COLS; x++) {
		for (int y = 0; y < ROWS; y++) {
			KeyRGB currentRGB;
			int currentKey = keyMap[y][(x + iteration) % COLS];

			if (isRainbow) {
				unsigned char rotatingRed = sin((((x % COLS) / static_cast<float>(COLS)) * 2 * 3.14f)) + 1;
				unsigned char rotatingGreen = sin((((x % COLS) / static_cast<float>(COLS)) * 2 * 3.14f) - (6.28f / 3)) + 1;
				unsigned char rotatingBlue = sin((((x % COLS) / static_cast<float>(COLS)) * 2 * 3.14f) + (6.28f / 3)) + 1;

				currentRGB = KeyRGB(
					rotatingRed * backgroundRainbow,
					rotatingGreen * backgroundRainbow,
					rotatingBlue * backgroundRainbow
				);
			}
			else {
				currentRGB = KeyRGB(backgroundRed, backgroundGreen, backgroundBlue);
			}
			writeMap[currentKey] = currentRGB;
		}
	}

}

// Build key lights from FFT data.
void KeyboardWriter::BuildFFTMap(unsigned char dataFFT[]){
	for (int i = 0; i < COLS; i++) {
		for (int k = 0; k < ROWS; k++) {
			if (dataFFT[(int)(i * 2.1 + 1)] >(255 / (15 + (i * 0.8))) * (ROWS - k)) {
				int currentKey = keyMap[k][i];
				KeyRGB currentRGB;
				if (isColorized) {
					int scaledRed = 0; 
					int scaledGreen = 0; 
					int percent = ((ROWS - k) / static_cast<float>(ROWS) * 100);
					// Scale the bottom half from green to yellow (prevents ugly brown)
					if (percent <= 50) {
						scaledRed = (percent * 2);
						scaledGreen = 100;
					}
					// Scale the top half from yellow to red
					else {
						scaledRed = 100;
						scaledGreen = 100 - ((percent - 50) * 2);
					}
					currentRGB = KeyRGB(scaledRed, scaledGreen, 0);
				}
				else {
					currentRGB = KeyRGB(visualizerRed, visualizerGreen, visualizerBlue);
				}
				writeMap[currentKey] = currentRGB;
			}
		}
	}
}


void KeyboardWriter::Write() {
	for (map<int, KeyRGB>::iterator iterator_ = writeMap.begin(); iterator_ != writeMap.end() ; ++iterator_) {
		LogiLedSetLightingForKeyWithScanCode(iterator_->first, iterator_->second.r, iterator_->second.g, iterator_->second.b);
	}
	writeMap.clear();
}