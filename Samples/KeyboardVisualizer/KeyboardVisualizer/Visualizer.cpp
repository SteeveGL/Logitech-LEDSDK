#include "stdafx.h"

// Logitech LED
#pragma comment(lib, "LogitechLEDLib.lib")
#include "LogitechLEDLib.h"

#include "Visualizer.h"

#define _USE_MATH_DEFINES
#include <chrono>
#include <thread>
#include <cmath>
#include <math.h>

using namespace std;

Visualizer::Visualizer() 
		: targetDevice(LOGI_DEVICETYPE_ALL), avgFFT(0), amplitude(DEFAULT_AMPTITUDE), lomontFFT(), keyboardWriter(),
		isActive(false), isPaused(false), redrawMS(DEFAULT_REDRAW_MS) {
}

// Destroy the object and shutdown the LED library.
Visualizer::~Visualizer() {
	isActive = false;
	alcCaptureStop(device);

	LogiLedShutdown();
}

bool Visualizer::Init() {
	if (!LogiLedInit())
		return false;

	LogiLedSetTargetDevice(targetDevice);
	LogiLedSaveCurrentLighting();

	device = alcCaptureOpenDevice(NULL, 16000, AL_FORMAT_MONO8, BUFFER_SIZE);
	
	return true;
}

void Visualizer::Start() {
	if (!isActive) {
		isActive = true;
		isPaused = false;

		alcCaptureStart(device);

		thread drawThread(&Visualizer::Draw, this);
		drawThread.detach();
	}
	else {
		isPaused = false;
	}
}

void Visualizer::Stop() {
	isActive = false;
	isPaused = false;

	alcCaptureStop(device);
	LogiLedRestoreLighting();
}

void Visualizer::Pause() {
	isPaused = true;
}

void Visualizer::Resume() {
	isPaused = false;
}

void Visualizer::Draw() {
	while (isActive) {
		while (isActive && !isPaused) {
			for (int j = 0; j < COLS; j++) {
				// reset mem
				for (int i = 0; i < BUFFER_SIZE; i++) {
					bufferAudio[i] = 0;
					bufferFFT[i] = 0;
					FFT[i] = 0;
				}

				// Capture the audio input into the bufferAudio.
				// Input is zero'd at 128, but sometimes comes in at 127 and 129 with no sound.
				alcCaptureSamples(device, (ALCvoid *)bufferAudio, BUFFER_SIZE);

				for (int i = 0; i < BUFFER_SIZE; i++) {
					// Don't accept 127, 128, and 129 because they are generated with no sound.
					if (bufferAudio[i] != 127 && bufferAudio[i] != 128 && bufferAudio[i] != 129) {
						FFT[i] = (bufferAudio[i] - 128) * amplitude;
					}
					else {
						FFT[i] = 0;
					}
				}

				lomontFFT.TableFFT(FFT, BUFFER_SIZE, true);

				for (int i = 0; i < BUFFER_SIZE; i += 2) {
					float fftmag = sqrt((FFT[i] * FFT[i]) + (FFT[i + 1] * FFT[i + 1]));
					avgFFT += fftmag;
					bufferFFT[i] = (unsigned char)fftmag;
					bufferFFT[i + 1] = bufferFFT[i];
				}

				avgFFT /= 10;

				keyboardWriter.BuildBackgroundMap(j);
				keyboardWriter.BuildFFTMap(bufferFFT);

				// Check a stop didn't happen while we were getting the data.
				if (isActive && !isPaused) {
					keyboardWriter.Write();
					this_thread::sleep_for(chrono::milliseconds(redrawMS));
				}
			}
		}
	}
}
