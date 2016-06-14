#ifndef VISUALIZER_H
#define VISUALIZER_H

#include "stdafx.h"

// Open AL
#pragma comment(lib, "../Dependencies/OpenAL/libs/Win32/OpenAL32.lib")
#include "../Dependencies/OpenAL/include/al.h"
#include "../Dependencies/OpenAL/include/alc.h"

#include "LomontFFT.h"
#include "KeyboardWriter.h"

enum {
	BUFFER_SIZE = 256,
	DEFAULT_AMPTITUDE = 10,
	DEFAULT_REDRAW_MS = 100
};

class Visualizer {
public:
	// FFT variables
	LomontFFT lomontFFT;
	KeyboardWriter keyboardWriter;
	ALCdevice *device;
	unsigned char bufferAudio[BUFFER_SIZE];
	unsigned char bufferFFT[BUFFER_SIZE];
	float FFT[BUFFER_SIZE];
	float avgFFT;

	// Logitech variables
	int targetDevice;

	// Control variables
	float amplitude;
	int redrawMS;
	bool isActive;
	bool isPaused;

	Visualizer();
	~Visualizer();

	bool Init();
	void Start();
	void Stop();
	void Draw();
	void Pause();
	void Resume();
};

#endif // VISUALIZER_H