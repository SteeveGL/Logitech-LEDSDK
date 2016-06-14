#ifndef LOMONTFFT_H
#define LOMONTFFT_H

#include "stdafx.h"

// Code to implement decently performing FFT for complex and real valued                                         
// signals. See www.lomont.org for a derivation of the relevant algorithms                                       
// from first principles. Copyright Chris Lomont 2010-2012.                                                      
// This code and any ports are free for all to use for any reason as long                                        
// as this header is left in place.                                                                              
// Version 1.1, Sept 2011   

enum {
	DEFAULT_A = 0,
	DEFAULT_B = 1,
};

class LomontFFT {
public:
	// Determine how phase works on the forward and inverse transforms.                                     
	// For size N=2^n transforms, the forward transform uses an                                             
	// exp(B*2*pi/N) term and the inverse uses an exp(-B*2*pi/N) term.                                      
	// Common values for (A,B) are                                                                          
	//     ( 0, 1)  - default                                                                               
	//     (-1, 1)  - data processing                                                                       
	//     ( 1,-1)  - signal processing                                                                     
	// Abs(B) should be relatively prime to N.                                                              
	// Setting B=-1 effectively corresponds to conjugating both input and                                   
	// output data.                                                                                         
	// Usual values for B are 1 or -1.         
	int A;
	int B;

	double *cosTable;
	double *sinTable;

	LomontFFT() : A(DEFAULT_A), B(DEFAULT_B) {};
	~LomontFFT();

	void FFT(float data[], int data_length, bool forward);
	void TableFFT(float data[], int data_length, bool forward);
	void RealFFT(float data[], int data_length, bool forward);

private:
	void Scale(float data[], int data_length, int n, bool forward);
	void Initialize(int size);
	void Reverse(float data[], int data_length, int n);
};

#endif // LOMONTFFT_H