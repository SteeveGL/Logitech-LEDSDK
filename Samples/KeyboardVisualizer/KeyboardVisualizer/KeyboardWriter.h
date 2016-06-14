#ifndef KEYBOARDWRITER_H
#define KEYBOARDWRITER_H

#include "stdafx.h"

#include <vector>
#include <map>

// Logitech LED
#pragma comment(lib, "LogitechLEDLib.lib")
#include "LogitechLEDLib.h"

// Simple structure to represent the RGB percents for a key.
struct KeyRGB {
	unsigned char r;
	unsigned char g;
	unsigned char b;

	KeyRGB() {};
	KeyRGB(unsigned char rPercent, unsigned char gPercent, unsigned char bPercent)
		: r(rPercent), g(gPercent), b(bPercent) {};
};

enum {
	// Actual rows on the keyboard.
	ROWS = 6,

	// Exaggerated fake columns to create a better visual effect.
	// Note: This corresponds to the FFT data which is out of 100.
	COLS = 92,  // 23 * 4 = 92

	// UI Defaults
	DEFAULT_BACKGROUND_RAINBOW = 50,
	DEFAULT_BACKGROUND_RED = 0,
	DEFAULT_BACKGROUND_GREEN = 0,
	DEFAULT_BACKGROUND_BLUE = 0,
	DEFAULT_VISUALIZER_RED = 100,
	DEFAULT_VISUALIZER_GREEN = 100,
	DEFAULT_VISUALIZER_BLUE = 100,
	DEFAULT_IS_RAINBOW = 1,
	DEFAULT_IS_COLORIZED = 0,
};

class KeyboardWriter {
public:
	// Positional layout map for each keycode on the keyboard.
	// Warning: There is no error checking so the keyboard needs to be correct and complete.
	std::vector<unsigned int> positionMap;

	// Size layout map for each spacing on the keyboard.
	// Warning: There is no error checking so the keyboard needs to be correct, complete and total to absolute value of 23 on each row.
	std::vector<float> sizeMap;

	// Mapped keyboard by combining the position and size maps.
	// Note: X is rows and Y is the width. (ex: 23 * 4)
	unsigned int keyMap[ROWS][COLS];

	// Keep a map buffer of LED values to write.
	std::map<int, KeyRGB> writeMap;

	// UI Options
	int backgroundRainbow;
	int backgroundRed;
	int backgroundGreen;
	int backgroundBlue;
	int visualizerRed;
	int visualizerBlue;
	int visualizerGreen;
	bool isRainbow;
	bool isColorized;

	KeyboardWriter();

	void BuildKeyMap();
	void BuildBackgroundMap(int iteration);
	void BuildFFTMap(unsigned char dataFFT[]);
	void Write();
};

#endif // KEYBOARDWRITER_H